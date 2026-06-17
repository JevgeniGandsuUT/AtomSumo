# Camera Homography Calibration Summary

Source files:

- [camera_homography_points.csv](../../tests/results/camera_homography_points.csv)
- [camera_homography_front.json](../../tests/results/camera_homography_front.json)
- [camera_homography_residuals.csv](../../tests/results/camera_homography_residuals.csv)
- [camera_calibration_tap_drive.ipynb](../../tests/notebooks/camera_calibration_tap_drive.ipynb)

## Method

The robot UI `CALIB` mode was used to collect Cam 1 pixel coordinates without moving the robot. Six physical marker points were measured in arena coordinates and paired with the clicked camera pixel coordinates.

The homography was calculated with a direct linear transform least-squares fit:

```text
pixel_x, pixel_y -> arena_x_mm, arena_y_mm
```

## Calibration Points

| Point | Pixel coordinate | Arena coordinate, mm |
| --- | ---: | ---: |
| P1 | `325, 153` | `150, 500` |
| P2 | `100, 131` | `-220, 760` |
| P3 | `196, 151` | `0, 550` |
| P4 | `34, 164` | `-200, 250` |
| P5 | `322, 181` | `60, 200` |
| P6 | `143, 127` | `-150, 1000` |

## Residual Error

| Metric | Value |
| --- | ---: |
| Points | `6` |
| Mean reprojection error | `43.6 mm` |
| Median reprojection error | `45.0 mm` |
| Maximum reprojection error | `68.3 mm` |

## Interpretation

This is a practical ground-plane camera calibration for the current robot setup. It is sufficient evidence that Cam 1 pixel coordinates can be mapped into arena millimeter coordinates. The remaining improvement would be a more precise calibration target, more widely distributed points, and optional chessboard intrinsic calibration.
