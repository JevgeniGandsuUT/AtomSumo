#!/usr/bin/env python3
"""Collect VL53L0X accuracy rows from the robot /tof endpoint."""

from __future__ import annotations

import argparse
import csv
import datetime as dt
import json
import time
import urllib.error
import urllib.request
from pathlib import Path


def read_tof(url: str, timeout: float) -> tuple[bool, int, int, str]:
    try:
        with urllib.request.urlopen(url, timeout=timeout) as response:
            data = json.loads(response.read().decode("utf-8"))
    except (urllib.error.URLError, TimeoutError, OSError, json.JSONDecodeError) as exc:
        return False, 0, -1, exc.__class__.__name__

    ready = bool(data.get("ready"))
    valid = bool(data.get("valid"))
    status = int(data.get("status", -1))
    measured = int(data.get("mm", 0))
    note = "valid" if ready and valid else f"ready={ready}, valid={valid}"
    return ready and valid, measured, status, note


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Collect ToF accuracy measurements.")
    parser.add_argument("--tof-url", default="http://192.168.4.1/tof")
    parser.add_argument("--output", default="tests/results/tof_accuracy_test.csv")
    parser.add_argument("--distances", type=int, nargs="+", default=[50, 100, 200, 300, 500])
    parser.add_argument("--trials", type=int, default=3)
    parser.add_argument("--timeout", type=float, default=1.0)
    parser.add_argument("--delay", type=float, default=0.25)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    fieldnames = ["date", "actual_mm", "trial", "measured_mm", "status", "error_mm", "notes"]
    exists = output.exists() and output.stat().st_size > 0

    with output.open("a", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fieldnames)
        if not exists:
            writer.writeheader()

        for actual in args.distances:
            input(f"Place flat obstacle at {actual} mm, then press Enter...")
            for trial in range(1, args.trials + 1):
                ok, measured, status, note = read_tof(args.tof_url, args.timeout)
                error = measured - actual if ok else ""
                writer.writerow(
                    {
                        "date": dt.date.today().isoformat(),
                        "actual_mm": actual,
                        "trial": trial,
                        "measured_mm": measured if ok else "",
                        "status": status,
                        "error_mm": error,
                        "notes": note,
                    }
                )
                print(f"{actual} mm trial {trial}: measured={measured if ok else 'NA'} status={status} {note}")
                time.sleep(args.delay)

    print(f"Wrote ToF rows to {output}")


if __name__ == "__main__":
    main()
