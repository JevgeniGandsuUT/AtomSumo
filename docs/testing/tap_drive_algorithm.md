# Tap-Drive Algorithm

This document describes the current tap-drive behavior implemented in the main web UI.

## Implementation Files

- UI algorithm: [firmware/main/01_WebUi.ino](../../firmware/main/01_WebUi.ino)
- Movement endpoint: [firmware/main/03_WebHandlers.ino](../../firmware/main/03_WebHandlers.ino)
- Motor direction logic: [firmware/main/02_Motion.ino](../../firmware/main/02_Motion.ino)

## Manual Tap-To-Drive

The browser UI receives the live Cam 1 stream from:

```text
http://192.168.4.20/stream
```

When `TAP` mode is enabled and the operator clicks on the Cam 1 image:

1. The click position is converted into image-relative coordinates:
   - `xNorm = (x / image_width) * 2 - 1`, range `-1..1`;
   - `yNorm = y / image_height`, range `0..1`.
2. The horizontal click position is converted into a turn angle:
   - `angleDeg = xNorm * (tapHorizontalFovDeg / 2)`;
   - default `tapHorizontalFovDeg = 50`, adjustable in the UI.
3. The turn time is estimated from the angle:
   - `turnMs = abs(angleDeg) / tapTurnDegPerSec * 1000`;
   - current `tapTurnDegPerSec = 193`.
4. The vertical click position is converted into drive time:
   - `verticalOffset = 0.5 - yNorm`;
   - `driveMs = tapCenterDriveMs + verticalOffset * tapCenterDriveMs`;
   - default `tapCenterDriveMs = 5000 ms`, adjustable in the UI;
   - clamped to `600..12000 ms`.
5. If the target is more than `4 deg` away from center:
   - stop;
   - turn left/right at `tapTurnSpeed = 150`;
   - stop;
   - drive forward at `tapDriveSpeed = 70`;
   - stop.
6. If the target is near the image center:
   - drive forward directly;
   - stop after the calculated drive time.

The UI sends the actual movement commands through:

```text
GET /move?dir=left
GET /move?dir=right
GET /move?dir=forward
GET /move?dir=stop
```

The main controller maps those commands to motor directions in `handleMove()` and `applyDirection()`.

## Calibration Click Mode

The UI also has a `CALIB` button for collecting homography points safely.

In `CALIB` mode:

1. the robot is stopped;
2. `TAP` and `AUTO` are disabled;
3. clicking Cam 1 only shows `PX x,y` and normalized coordinates in the target-state line;
4. no `/move` command is sent by the click.

Use this mode to fill `tests/results/camera_homography_points_template.csv`.

## Disabled Experimental Red Servo

An experimental red-target visual servo loop existed in the UI code, but it is disabled because it is not reliable enough on the current camera/lighting setup. It is not used as project evidence.

## Current Validation Status

The manual tap-drive algorithm is implemented and usable from the UI. The remaining work is measurement, not basic implementation:

- run tap-drive target trials;
- record final position error in millimeters;
- record heading error in degrees;
- save results to `tests/results/tap_drive_accuracy_test.csv`.

Until that measured accuracy file exists, the requirement should be described as `implemented, accuracy not yet measured`.
