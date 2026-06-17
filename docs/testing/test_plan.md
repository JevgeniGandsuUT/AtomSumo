# Test Plan

This document summarizes the practical verification plan for the current `ATOM Sumo` prototype.

It is intentionally split into:

- tests already supported by repository evidence;
- tests implemented in firmware but still needing stronger measured evidence;
- tests still required before a true production-ready claim.

## Current Verified Tests

### 1. Wi-Fi Access Point and Web UI

Objective:
- confirm that the robot starts its own Wi-Fi network and serves the control page.

Method:
- power on the main controller;
- verify that SSID `Robot-Control` appears;
- connect a client device;
- open `http://192.168.4.1/`.

Pass condition:
- the AP is visible;
- the web page opens successfully;
- the UI loads without external infrastructure.

Evidence:
- [main firmware](../../firmware/main/main.ino)
- [main web UI](../../firmware/main/01_WebUi.ino)

Status:
- implemented and used as the baseline operator interface.

### 2. Camera Stream Test

Objective:
- confirm that the camera nodes stream video through the robot network.

Method:
- power the `AtomS3R-CAM` node;
- connect it to the robot AP;
- open the stream from the web page or direct stream endpoint;
- visually verify live video.

Pass condition:
- the camera stream loads and updates continuously.

Evidence:
- [camera firmware](../../firmware/camera/sketch_jan17a/sketch_jan17a.ino)
- [camera stream demo video](../../media/video/MicrosoftTeams-video.mp4)

Status:
- camera streaming is implemented for static IP camera nodes `192.168.4.20` and `192.168.4.21`.

### 3. Manual Drive Command Test

Objective:
- verify that the UI can send motion commands to the robot.

Method:
- send `forward`, `backward`, `left`, `right`, diagonal arc commands, and `stop` from the web UI;
- confirm motor response on the robot or raised test stand;
- verify speed control using the UI speed slider.

Pass condition:
- all direction commands are accepted;
- both motor channels respond as expected;
- stop command always works.

Evidence:
- [main firmware](../../firmware/main/main.ino)
- [motion logic](../../firmware/main/02_Motion.ino)
- [web handlers](../../firmware/main/03_WebHandlers.ino)

Status:
- implemented and used for movement measurements.

### 4. HTTP Latency Test

Objective:
- measure command-path responsiveness of the robot control interface.

Method:
- run the latency script against `http://192.168.4.1`;
- alternate `/move?dir=forward` and `/move?dir=stop`;
- collect `120` requests per run;
- compare `speed=0` and `speed=180` runs.

Pass condition:
- median latency is within a usable range for manual control;
- stop latency remains stable;
- results are recorded for reporting.

Evidence:
- [latency test script](../../tests/scripts/http_latency_test.py)
- [power brownout test script](../../tests/scripts/power_brownout_test.py)
- [ToF accuracy collection script](../../tests/scripts/tof_accuracy_collect.py)
- [latency analysis notebook](../../tests/notebooks/http_latency_analysis.ipynb)
- [latency result write-up](latency_test_result.md)
- [analysis results folder](../../tests/results)

Status:
- measured and documented.

### 5. Movement Speed Test

Objective:
- measure how fast the robot travels at different PWM values.

Method:
- mark a straight `1.0 m` distance;
- command `forward`;
- measure travel time;
- calculate `speed_m_s = distance_m / time_s`.

Evidence:
- [movement_speed_test.csv](../../tests/results/movement_speed_test.csv)

Status:
- measured for PWM `80`, `150`, `180`, and `255`.

### 6. Straightness / Drift Test

Objective:
- measure whether the robot drives straight or pulls left/right.

Method:
- drive forward over `1.0 m`;
- measure lateral offset at the finish.

Evidence:
- [movement_drift_test.csv](../../tests/results/movement_drift_test.csv)

Status:
- measured; drift is mostly left and should be used for motor trim calibration.

### 7. Turn-In-Place Test

Objective:
- measure turning response for left and right commands.

Method:
- command `left` or `right`;
- measure time for `180 deg` turns;
- repeat for both directions.

Evidence:
- [movement_turn_test.csv](../../tests/results/movement_turn_test.csv)

Status:
- measured for PWM `150`, `180`, and `255`.

### 8. ToF Stop Behavior Test

Objective:
- verify that the `VL53L0X` safety stop can stop the robot before an obstacle.

Method:
- set ToF stop threshold values such as `400 mm`, `300 mm`, and `250 mm`;
- drive toward an obstacle at several PWM values;
- record the actual stop distance and result.

Evidence:
- [tof_stop_test.csv](../../tests/results/tof_stop_test.csv)

Status:
- measured; works at moderate speeds, but high-speed PWM `255` requires tuning.

### 9. Cam 2 Color Sensor Integration Test

Objective:
- verify that the downward `TCS34725` sensor connected to Cam 2 returns live color data.

Method:
- flash Cam 2 with `CAMERA_SLOT=2`;
- open `http://192.168.4.21:81/color`;
- verify that JSON updates;
- verify that the main UI color panel updates.

Pass condition:
- endpoint returns `ready:true`;
- UI shows color name and RGB/clear values;
- stream on port `80` remains usable while color telemetry is served on port `81`.

Evidence:
- [camera firmware](../../firmware/camera/sketch_jan17a/sketch_jan17a.ino)
- [main web UI](../../firmware/main/01_WebUi.ino)

Status:
- implemented, flashed, and measured on red, light, and dark surfaces. Closed-loop retreat/avoid behavior is not claimed in this test.

### 10. Tap-Drive and Simple Auto Behavior

Objective:
- verify that the UI contains higher-level control modes beyond manual driving.

Method:
- enable tap-drive and tap the front camera view;
- enable auto mode and verify ToF-based movement decisions.

Evidence:
- [main web UI](../../firmware/main/01_WebUi.ino)
- [auto behavior logger](auto_behavior_logger.md)
- [auto behavior observation](auto_behavior_observation.md)
- [tap-drive algorithm](tap_drive_algorithm.md)
- [camera calibration and tap-drive plan](camera_calibration_tap_drive_plan.md)
- [camera calibration notebook](../../tests/notebooks/camera_calibration_tap_drive.ipynb)
- [tap-drive accuracy template](../../tests/results/tap_drive_accuracy_test_template.csv)

Status:
- implemented as a timed camera-coordinate tap-drive algorithm.
- not yet calibrated with full OpenCV/homography.
- not yet measured against target accuracy requirements.

## Open Tests Required for Stronger V3-V5 Claims

### A. Static ToF Accuracy

Current file:
- [tof_accuracy_test.csv](../../tests/results/tof_accuracy_test.csv)

Current status:
- measured rows exist in [tof_accuracy_test.csv](../../tests/results/tof_accuracy_test.csv);
- readings show a large offset/error at several distances, so calibration or mounting explanation is still required.

Required measurements:
- actual distances: `50`, `100`, `200`, `300`, and `500 mm`;
- at least `3` trials per distance;
- record `measured_mm`, `status`, and `error_mm`.

### B. Power Brownout / Reset Test

Current file:
- [power_brownout_test.csv](../../tests/results/power_brownout_test.csv)

Current status:
- measured rows exist in [power_brownout_test.csv](../../tests/results/power_brownout_test.csv);
- no main-controller reset or Wi-Fi disconnect was observed in the current run;
- Cam1/Cam2 endpoint failures were recorded, so the test must be repeated with both camera nodes confirmed online.

Required measurements:
- repeat PWM `80`, `150`, `180`, and `255`;
- repeat commands `forward`, `backward`, `left`, `right`, and `stop`;
- record reset, Wi-Fi disconnect, camera loss, and notes for both camera nodes.

### C. Color / Red-Area Repeatability

Current file:
- [color_repeatability_test.csv](../../tests/results/color_repeatability_test.csv)

Current status:
- measured rows exist for red, light, and dark surfaces;
- dark threshold was corrected and flashed to Cam 2;
- UI confirmed `COLOR DARK` after the fix.

### D. Click-To-Drive Accuracy

Required measurements:
- mark target points on the arena;
- tap camera image;
- measure final target error in `mm` and heading error in `deg`;
- repeat at least `5-10` trials.

Suggested output:
- template: [tap_drive_accuracy_test_template.csv](../../tests/results/tap_drive_accuracy_test_template.csv)
- measured output after testing: `tests/results/tap_drive_accuracy_test.csv`

### E. Enclosure and Service Tests

Required measurements:
- motor/enclosure heat after `5 min` driving;
- `3-5` controlled impact checks;
- battery replacement/service time;
- repeated `90 s` match-like endurance runs.

Suggested outputs:
- `tests/results/enclosure_heat_test.csv`
- `tests/results/enclosure_impact_test.csv`
- `tests/results/battery_service_test.csv`
- `tests/results/match_endurance_test.csv`

## Recommended Defense Wording

The project has verified the control path, web UI, camera streaming, movement, ToF stop behavior, and Cam 2 color telemetry. The remaining validation work is no longer basic bring-up; it is calibration, repeatability, enclosure validation, power robustness, and production documentation.
