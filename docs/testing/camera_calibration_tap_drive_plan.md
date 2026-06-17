# Camera Calibration and Tap-Drive Test Plan

This plan closes the documentation gap for Verstapost 3 camera calibration and tap-to-drive validation.
It defines the files and measurements needed before the feature can be marked as fully verified.

## Goal

The front camera must be calibrated well enough that a tap on the camera image can be converted into a ground-plane target. The final validation is a measured tap-drive accuracy test, not just a working UI control.

## Required Calibration Outputs

| Output | Target path | Purpose |
| --- | --- | --- |
| Intrinsic calibration JSON | `tests/results/camera_intrinsics_front.json` | camera matrix and lens distortion |
| Ground-plane homography JSON | `tests/results/camera_homography_front.json` | pixel-to-arena coordinate mapping |
| Calibration notebook | `tests/notebooks/camera_calibration_tap_drive.ipynb` | repeatable calculation and plots |
| Tap-drive accuracy CSV | `tests/results/tap_drive_accuracy_test.csv` | measured final position error |

## Camera Intrinsic Calibration

1. Print or display a chessboard calibration target.
2. Measure and record the chessboard square size in millimeters.
3. Capture at least `10-15` front-camera images from different positions and angles.
4. Run OpenCV chessboard corner detection.
5. Save camera matrix, distortion coefficients, image size, reprojection error, and date.

Suggested pass condition:
- mean reprojection error is documented;
- the calibration JSON exists;
- the notebook can re-run without errors.

## Ground-Plane Homography

1. Place at least `4` visible markers on the arena floor.
2. Measure each marker position in arena coordinates, in millimeters from a fixed origin.
3. Open the robot UI and press `CALIB`.
4. Click each marker in the Cam 1 image.
5. Record the shown `PX x,y` coordinate for each marker.
6. The robot does not move in `CALIB` mode; clicks only display pixel coordinates.
7. Estimate homography with OpenCV.
8. Save the homography matrix and marker table.

Suggested pass condition:
- at least `4` points are used;
- more than `4` points are preferred so residual error can be checked;
- residual error is documented in millimeters.

## Tap-Drive Accuracy Test

1. Mark `5-10` target points on the arena.
2. Tap each target in the camera UI.
3. Let the robot drive using the tap-drive behavior.
4. Measure final position error in millimeters and final heading error in degrees.
5. Record result as `pass`, `miss`, or `unsafe_stop`.

Suggested pass condition for the current prototype:
- measured results exist for at least `5` targets;
- median position error is reported;
- any misses or unsafe stops are explained.

## Current Status

The UI has a safe `CALIB` mode for collecting Cam 1 pixel coordinates without moving the robot. Six real homography points were collected and used to generate `camera_homography_front.json`.

Related result:
- [camera_homography_calibration_summary.md](camera_homography_calibration_summary.md)
