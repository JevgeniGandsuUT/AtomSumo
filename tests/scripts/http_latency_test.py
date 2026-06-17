import argparse
import csv
import sys
import time
import urllib.request
from pathlib import Path
from statistics import median


BASE_URL = "http://192.168.4.1"
COUNT = 120
DELAY_S = 0.05
DEFAULT_SPEED = 0


def http_get(url: str) -> int:
    with urllib.request.urlopen(url, timeout=5) as response:
        response.read()
        return response.status


def percentile(sorted_values, p):
    if not sorted_values:
        return None
    index = max(0, min(len(sorted_values) - 1, int((len(sorted_values) - 1) * p)))
    return sorted_values[index]


def parse_args():
    parser = argparse.ArgumentParser(description="Measure HTTP control latency against the robot AP.")
    parser.add_argument("--speed", type=int, default=DEFAULT_SPEED, help="Motor speed value to set before the test (0-255).")
    parser.add_argument("--count", type=int, default=COUNT, help="Number of HTTP requests to send.")
    parser.add_argument("--delay", type=float, default=DELAY_S, help="Delay between requests in seconds.")
    parser.add_argument(
        "--label",
        default="",
        help="Optional label appended to the result filenames, e.g. medium_speed.",
    )
    return parser.parse_args()


def main():
    args = parse_args()
    speed = max(0, min(255, args.speed))

    repo_root = Path(__file__).resolve().parents[1]
    results_dir = repo_root / "analysis" / "results"
    results_dir.mkdir(parents=True, exist_ok=True)

    stamp = time.strftime("%Y-%m-%d_%H-%M-%S")
    label_suffix = f"_{args.label}" if args.label else ""
    csv_path = results_dir / f"http_latency_{stamp}{label_suffix}.csv"
    summary_path = results_dir / f"http_latency_{stamp}{label_suffix}_summary.txt"

    setup_url = f"{BASE_URL}/speed?value={speed}"
    http_get(setup_url)

    rows = []
    for i in range(args.count):
        endpoint = "/move?dir=forward" if i % 2 == 0 else "/move?dir=stop"
        url = f"{BASE_URL}{endpoint}"
        t0 = time.perf_counter()
        status = http_get(url)
        dt_ms = (time.perf_counter() - t0) * 1000.0
        rows.append(
            {
                "n": i + 1,
                "endpoint": endpoint,
                "status_code": status,
                "latency_ms": f"{dt_ms:.3f}",
            }
        )
        time.sleep(args.delay)

    values = [float(row["latency_ms"]) for row in rows]
    values_sorted = sorted(values)
    summary_lines = [
        f"base_url={BASE_URL}",
        f"count={args.count}",
        f"speed_value={speed}",
        f"delay_s={args.delay}",
        f"median_ms={median(values):.3f}",
        f"p95_ms={percentile(values_sorted, 0.95):.3f}",
        f"max_ms={max(values):.3f}",
        "notes=measures HTTP request-response latency while alternating /move?dir=forward and /move?dir=stop",
    ]

    with csv_path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=["n", "endpoint", "status_code", "latency_ms"])
        writer.writeheader()
        writer.writerows(rows)

    summary_path.write_text("\n".join(summary_lines) + "\n", encoding="utf-8")

    print(csv_path)
    print(summary_path)
    print("\n".join(summary_lines))


if __name__ == "__main__":
    try:
        main()
    except Exception as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        sys.exit(1)
