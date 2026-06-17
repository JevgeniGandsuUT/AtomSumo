# Tap-Drive Accuracy Summary

Source data:
- [tap_drive_accuracy_test.csv](../../tests/results/tap_drive_accuracy_test.csv)

## Test Setup

The robot was returned to the same start position before each trial. The operator enabled `TAP`, clicked the target in the Cam 1 view, let the robot complete the timed tap-drive sequence, and measured the final position.

The current UI did not display click pixel coordinates during the test, so `tap_x_px` and `tap_y_px` are left blank. The physical target and final positions are recorded in millimeters.

## Results

| Target | Target position, mm | Final position, mm | Position error, mm | Heading error, deg | Result |
| --- | ---: | ---: | ---: | ---: | --- |
| T1 | `500, 300` | `550, 300` | `50` | `-5` | pass |
| T2 | `0, 300` | `-5, 300` | `5` | `-5` | pass |
| T3 | `-500, 300` | `-500, 300` | `0` | `0` | pass |
| T4 | `0, 150` | `0, 170` | `20` | `0` | pass |
| T5 | `0, 500` | `-100, 400` | `141.4` | `-10` | pass |

## Summary

| Metric | Value |
| --- | ---: |
| Trials | `5` |
| Pass rows | `5 / 5` |
| Median position error | `20 mm` |
| Mean position error | `43.3 mm` |
| Maximum position error | `141.4 mm` |
| Median absolute heading error | `5 deg` |

## Interpretation

The timed tap-drive algorithm is implemented and physically measured. Accuracy is good for centered and moderate side targets, with the largest error on the far center target. This is sufficient evidence that click/tap-to-drive works, while future improvement should add on-screen tap pixel logging and camera homography calibration.
