# Auto Behavior Logger

The main UI includes a browser-side CSV logger for the `AUTO` behavior.

## Controls

Open the `AUTO` panel and use:

- `Start log` / `Stop log` - record automatic behavior rows.
- `CSV` - download `auto_behavior_log.csv` from the browser.
- `Clear log` - reset collected rows.
- `Mark pass` - add an operator pass row and increment the trial number.

## Logged Fields

```text
ts_ms,trial,tof_ready,tof_valid,distance_mm,threshold_mm,action,result,note
```

The logger records every `autoTick()` decision while logging is enabled:

- `forward` when the path is clear;
- `left` or `right` when an obstacle is below the turn threshold;
- `forward-left` or `forward-right` near the threshold;
- `stop` when ToF is not ready.

## Test Procedure

1. Open the robot UI.
2. Open the `AUTO` panel.
3. Press `Clear log`.
4. Press `Start log`.
5. Press `Auto mode`.
6. Run one obstacle/clear trial.
7. Press `Mark pass` if the behavior was correct.
8. Repeat until at least five trials are marked.
9. Press `Stop log`.
10. Press `CSV` and save the downloaded file into `tests/results/`.
