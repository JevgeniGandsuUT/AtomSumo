#!/usr/bin/env python3
"""Run a repeatable power/brownout smoke test against the ATOM Sumo robot.

The script sends movement commands at several PWM values and records whether
the main controller, camera streams, or Cam 2 color endpoint become unreachable.

It cannot directly measure current or driver temperature; add those observations
to the notes column after the run if measured externally.
"""

from __future__ import annotations

import argparse
import csv
import datetime as dt
import time
import urllib.error
import urllib.request
from pathlib import Path


DEFAULT_COMMANDS = ["forward", "backward", "left", "right", "stop"]
DEFAULT_PWMS = [80, 150, 180, 255]


def fetch(url: str, timeout: float = 1.0, max_bytes: int = 256) -> tuple[bool, str]:
    try:
        with urllib.request.urlopen(url, timeout=timeout) as response:
            data = response.read(max_bytes)
            return True, f"HTTP {response.status}, {len(data)} bytes"
    except (urllib.error.URLError, TimeoutError, OSError) as exc:
        return False, exc.__class__.__name__


def send(base_url: str, path: str, timeout: float = 1.0) -> tuple[bool, str]:
    return fetch(base_url.rstrip("/") + path, timeout=timeout)


def yes_no(value: bool) -> str:
    return "yes" if value else "no"


def run_test(args: argparse.Namespace) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    today = dt.date.today().isoformat()

    main_ok, main_note = fetch(args.base_url, timeout=args.timeout)
    if not main_ok:
        raise SystemExit(f"Robot main page is not reachable at {args.base_url}: {main_note}")

    for pwm in args.pwms:
        speed_ok, speed_note = send(args.base_url, f"/speed?value={pwm}", timeout=args.timeout)
        time.sleep(args.settle)

        for command in args.commands:
            for trial in range(1, args.trials + 1):
                notes: list[str] = []
                if not speed_ok:
                    notes.append(f"speed_set_failed={speed_note}")

                cmd_ok, cmd_note = send(args.base_url, f"/move?dir={command}", timeout=args.timeout)
                time.sleep(args.move_time if command != "stop" else args.stop_time)

                stop_ok, stop_note = send(args.base_url, "/move?dir=stop", timeout=args.timeout)
                time.sleep(args.rest)

                tof_ok, tof_note = fetch(args.base_url.rstrip("/") + "/tof", timeout=args.timeout)
                cam1_ok, cam1_note = fetch(args.cam1_capture_url, timeout=args.camera_timeout) if args.cam1_capture_url else (True, "skipped")
                cam2_ok, cam2_note = fetch(args.cam2_capture_url, timeout=args.camera_timeout) if args.cam2_capture_url else (True, "skipped")
                color_ok, color_note = fetch(args.cam2_color_url, timeout=args.timeout) if args.cam2_color_url else (True, "skipped")

                main_after_ok, main_after_note = fetch(args.base_url, timeout=args.timeout)
                wifi_disconnect = not (cmd_ok and stop_ok and tof_ok and main_after_ok)
                camera_loss = not (cam1_ok and cam2_ok and color_ok)
                reset_observed = "suspected" if wifi_disconnect else "no"

                if not cmd_ok:
                    notes.append(f"cmd_failed={cmd_note}")
                if not stop_ok:
                    notes.append(f"stop_failed={stop_note}")
                if not tof_ok:
                    notes.append(f"tof_failed={tof_note}")
                if not main_after_ok:
                    notes.append(f"main_after_failed={main_after_note}")
                if not cam1_ok:
                    notes.append(f"cam1_failed={cam1_note}")
                if not cam2_ok:
                    notes.append(f"cam2_failed={cam2_note}")
                if not color_ok:
                    notes.append(f"color_failed={color_note}")

                rows.append(
                    {
                        "date": today,
                        "pwm": str(pwm),
                        "command": command,
                        "trial": str(trial),
                        "power_source": args.power_source,
                        "reset_observed": reset_observed,
                        "wifi_disconnect": yes_no(wifi_disconnect),
                        "camera_loss": yes_no(camera_loss),
                        "notes": "; ".join(notes) if notes else "automatic test passed",
                    }
                )
                print(f"pwm={pwm:3d} command={command:8s} trial={trial}: {rows[-1]['notes']}")

    send(args.base_url, "/move?dir=stop", timeout=args.timeout)
    return rows


def append_rows(path: Path, rows: list[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fieldnames = [
        "date",
        "pwm",
        "command",
        "trial",
        "power_source",
        "reset_observed",
        "wifi_disconnect",
        "camera_loss",
        "notes",
    ]
    exists = path.exists() and path.stat().st_size > 0
    with path.open("a", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fieldnames)
        if not exists:
            writer.writeheader()
        writer.writerows(rows)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run ATOM Sumo power brownout test.")
    parser.add_argument("--base-url", default="http://192.168.4.1")
    parser.add_argument("--cam1-capture-url", default="http://192.168.4.20/capture")
    parser.add_argument("--cam2-capture-url", default="http://192.168.4.21/capture")
    parser.add_argument("--cam2-color-url", default="http://192.168.4.21:81/color")
    parser.add_argument("--output", default="tests/results/power_brownout_test.csv")
    parser.add_argument("--power-source", default="2S_18650")
    parser.add_argument("--pwms", type=int, nargs="+", default=DEFAULT_PWMS)
    parser.add_argument("--commands", nargs="+", default=DEFAULT_COMMANDS)
    parser.add_argument("--trials", type=int, default=2)
    parser.add_argument("--move-time", type=float, default=0.8)
    parser.add_argument("--stop-time", type=float, default=0.2)
    parser.add_argument("--rest", type=float, default=0.5)
    parser.add_argument("--settle", type=float, default=0.3)
    parser.add_argument("--timeout", type=float, default=1.0)
    parser.add_argument("--camera-timeout", type=float, default=2.0)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    rows = run_test(args)
    append_rows(Path(args.output), rows)
    print(f"Wrote {len(rows)} rows to {args.output}")


if __name__ == "__main__":
    main()
