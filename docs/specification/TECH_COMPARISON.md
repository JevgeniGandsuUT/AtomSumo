# Technical Comparison and Design Rationale

This document records the main technical tradeoffs behind the `ATOM Sumo` prototype.

It supports the Verstapost 1 specification and the final Verstapost 5 handover by explaining why the current architecture was selected and what alternatives were considered.

Related Estonian comparative analysis:

- [Vordlev_analyys.docx](../analysis/Vordlev_analyys.docx)
- [Vordlev_analyys_summary.md](../analysis/Vordlev_analyys_summary.md)

## 1. Main Controller Architecture

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| Android phone as robot brain | camera, display, battery, high-level compute already included | harder low-level motor wiring, less reproducible mechanical integration, needs phone-specific app path | not selected |
| ESP32 / M5 ATOM main controller | simple motor control, Wi-Fi AP, Arduino workflow, compact, reproducible | less compute for heavy vision | selected |
| Raspberry Pi / Linux SBC | strong OpenCV support, easy camera processing | higher power, slower boot, more fragile power setup, heavier | not selected for current prototype |

Decision:
- The robot uses an `M5 ATOM` / ESP32 as the onboard controller.
- Phone/laptop remains the browser client, not the robot brain.

Reason:
- The repository already contains working ESP32 firmware, browser control, motor control, ToF telemetry, and camera-node integration.
- This is easier to reproduce for a course prototype than a phone-as-brain build.

Evidence:
- [main firmware](../../firmware/main/main.ino)
- [main web UI](../../firmware/main/01_WebUi.ino)

## 2. Camera Architecture

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| Single onboard camera | fewer parts, simpler power | cannot easily serve both operator view and downward sensing | not enough |
| Two AtomS3R-CAM nodes | separate front and downward views, independent static IPs, browser-visible streams | more Wi-Fi/power load, two boards to flash | selected |
| USB camera to SBC | good software ecosystem | requires Linux SBC and stronger power budget | not selected |

Decision:
- Use two `M5 AtomS3R-CAM` nodes:
  - Cam 1: front/operator view at `192.168.4.20`;
  - Cam 2: downward view and color sensor telemetry at `192.168.4.21`.

Reason:
- The dual-node design lets the ESP32 main controller stay focused on motor/sensor control while cameras serve video over HTTP.

Evidence:
- [camera firmware](../../firmware/camera/sketch_jan17a/sketch_jan17a.ino)
- [main web UI](../../firmware/main/01_WebUi.ino)

## 3. Drive System

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| Two driven wheels + passive support wheels | simple, low cost, easy to test and measure | lower pushing force with small N20 motors | considered, not final |
| Four driven wheels with two motors per side wired in parallel | better traction and roughly doubles available motor torque with the same two-channel control model | higher current per TB6612 channel, must be validated | selected |
| Tracks / SMARS-style treads | good contact area, sumo-friendly | more friction, harder mechanical sourcing, more current | possible future revision |

Decision:
- Current build uses `4WD`: two left motors are wired in parallel to one TB6612 channel, and two right motors are wired in parallel to the other TB6612 channel.

Reason:
- With small `N20 / GA12-N20` class motors, the system benefits from summing motor torque across four wheels.
- The firmware can still use differential two-channel control because the motors are grouped by side.
- Movement measurements already exist for speed, drift, turning, and ToF stop behavior.
- The tradeoff is higher channel current, so brownout and driver-heating tests are required.

This matches the separate Estonian `Vordlev analyys` document: small N20/GA12-N20 class motors are close to the motor-limited region, so adding motors can increase usable pushing force. Competition robots with much stronger motors are usually traction-limited instead, so adding more motor power does not help unless grip or mass also increases.

Evidence:
- [movement_speed_test.csv](../../tests/results/movement_speed_test.csv)
- [movement_drift_test.csv](../../tests/results/movement_drift_test.csv)
- [movement_turn_test.csv](../../tests/results/movement_turn_test.csv)

## 4. Motor Driver and Power

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| TB6612FNG | common, compact, supports two DC motors, good Arduino support | current margin must be checked | selected |
| L298N | common and robust-looking | inefficient, voltage drop, bulky | not selected |
| Direct transistor/MOSFET per motor | flexible | more design risk and PCB effort | not selected |

Decision:
- Use `TB6612FNG` with a regulated `6.0V` motor rail. Each output channel drives two motors in parallel.

Reason:
- The motors are treated as `N20 6V`; raw `2S` battery voltage can reach `8.4V` and should not be applied directly.
- The parallel-pair wiring keeps the control architecture simple, but current draw must be measured.

Evidence:
- [power_scheme.md](power_scheme.md)
- [BOM.csv](BOM.csv)

## 5. Battery Architecture

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| USB powerbank | easy bench power, integrated protection | bulky, weak motor-current story, poor enclosure fit | legacy only |
| 1S Li-Ion | simpler charging | needs boost for motors, higher current for same power | not selected |
| 2S 18650 | good energy, motor rail can be buck-regulated, common cells | needs BMS and validation | selected |

Decision:
- Use `2S 18650` with:
  - `5.0V` logic buck;
  - `6.0V` motor buck;
  - common ground;
  - BMS and master switch.

Remaining risk:
- the `2S 3A` BMS must still be checked under motor startup and stall-like load.

Evidence:
- [power_scheme.md](power_scheme.md)
- [power_tree.drawio.png](../../hardware/diagrams/power_tree.drawio.png)

## 6. Obstacle Sensing

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| VL53L0X ToF | direct distance in mm, simple I2C, fast enough for close obstacle logic | reflective/angle limitations, needs threshold tuning | selected |
| Camera-only obstacle detection | flexible, can detect shapes | needs calibration and vision processing | not primary |
| Ultrasonic sensor | cheap, simple | large, slower, poor for small angled surfaces | not selected |

Decision:
- Use `VL53L0X` as the front safety and obstacle sensor.

Evidence:
- [main firmware](../../firmware/main/main.ino)
- [tof_stop_test.csv](../../tests/results/tof_stop_test.csv)

Remaining work:
- fill [tof_accuracy_test.csv](../../tests/results/tof_accuracy_test.csv).

## 7. Boundary / Color Sensing

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| Downward camera color detection | no extra sensor, flexible | needs image processing and stable lighting | secondary/future |
| TCS34725 color sensor | direct RGB/clear values, simpler thresholds, fast polling | sees only a local spot | selected for Cam 2 |
| Reflectance sensor array | good line/boundary detection | needs extra wiring and placement | possible future improvement |

Decision:
- Add a `TCS34725` color sensor to Cam 2 Grove I2C.

Reason:
- It gives fast local color telemetry without forcing full image processing on the main controller.

Evidence:
- [camera firmware](../../firmware/camera/sketch_jan17a/sketch_jan17a.ino)
- [main web UI](../../firmware/main/01_WebUi.ino)

Remaining work:
- measure repeatability on actual arena surfaces;
- implement closed-loop retreat or marked-area behavior.

## 8. Tap-Drive / Positioning

| Option | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- |
| Timed tap-drive | quick to implement, demonstrates UI workflow | not geometrically accurate | current prototype |
| Calibrated camera raycast / homography | measurable target accuracy | requires calibration images/notebook | next step |
| ArUco arena positioning | global pose awareness, strong V5 story | requires marker setup and vision processing | future work |

Decision:
- Current implementation uses timed tap-drive from camera image position.
- It is acceptable as a prototype but not as final calibrated positioning.

Evidence:
- [main web UI](../../firmware/main/01_WebUi.ino)

Remaining work:
- add calibration notebook;
- measure click-to-drive target error;
- implement ArUco or document a simpler approved alternative.

## 9. Summary

The selected architecture favors fast, reproducible integration:

- ESP32/M5 main controller for control and web UI;
- two AtomS3R-CAM nodes for video;
- VL53L0X for front distance;
- TCS34725 for downward color telemetry;
- TB6612FNG and regulated `6.0V` motor rail;
- 4WD using two parallel motor pairs on the two TB6612 channels;
- `2S 18650` battery architecture.

This is a realistic choice for a course prototype because most subsystems are already implemented and measured. The remaining work is mainly calibration, repeatability measurements, production documentation, and final autonomous arena behavior.
