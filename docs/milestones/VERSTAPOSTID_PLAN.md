# Verstapost Milestone Plan and Critical Status

This document is the current working milestone status for `ATOM Sumo`.

It replaces older draft notes where ToF, tap-drive, and color telemetry were still listed as missing. Those features are now implemented in firmware, but several measurement and production-readiness tasks remain open.

## 1. Current Repository Evidence

| Area | Current evidence | Status |
| --- | --- | --- |
| Main control firmware | [firmware/main/main.ino](../../firmware/main/main.ino), [01_WebUi.ino](../../firmware/main/01_WebUi.ino), [02_Motion.ino](../../firmware/main/02_Motion.ino), [03_WebHandlers.ino](../../firmware/main/03_WebHandlers.ino) | implemented and compiles |
| Camera firmware | [firmware/camera/sketch_jan17a/sketch_jan17a.ino](../../firmware/camera/sketch_jan17a/sketch_jan17a.ino) | implemented and compiles |
| Camera streams | UI links to `192.168.4.20` and `192.168.4.21`; [demo video](../../media/video/MicrosoftTeams-video.mp4) | implemented |
| Front ToF | `VL53L0X` in main firmware and `/tof` endpoint | implemented |
| Cam 2 color sensor | `TCS34725` on Cam 2, color endpoint on port `81`, UI color panel | implemented |
| Manual drive | browser buttons, speed slider, arc-turn control | implemented |
| Tap-drive | timed tap-to-drive prototype in UI | implemented, not calibrated |
| Simple auto mode | ToF-based forward/turn/arc behavior in UI | implemented, not match-ready |
| Baseline CAD | [hardware/cad/ver1](../../hardware/cad/ver1) | present |
| Functional enclosure CAD | [hardware/cad/ver3](../../hardware/cad/ver3) | present |
| Real Fusion 360 CAD | [hardware/cad/real/README.md](../../hardware/cad/real/README.md) | present |
| PCB evidence | [hardware/pcb/README.md](../../hardware/pcb/README.md) | present |
| Movement measurements | [tests/results](../../tests/results) | partially complete |
| Latency notebook | [http_latency_analysis.ipynb](../../tests/notebooks/http_latency_analysis.ipynb) | present |
| Full test-results notebook | [test_results_analysis.ipynb](../../tests/notebooks/test_results_analysis.ipynb) | present |

## 2. Verstapost 1: Specification

### Required Package

| Item | Status | Evidence |
| --- | --- | --- |
| Selected project and architecture | complete | [V1_specification.md](../specification/V1_specification.md) |
| Functional requirements | complete | [V1_specification.md](../specification/V1_specification.md) |
| Performance targets | complete | [V1_specification.md](../specification/V1_specification.md) |
| BOM | complete | [BOM.csv](../specification/BOM.csv) |
| Power architecture | complete | [power_scheme.md](../specification/power_scheme.md) |
| System/power diagrams | complete | [drawio](../../hardware/diagrams) |
| Test plan | updated | [test_plan.md](../testing/test_plan.md) |
| Submission tracker | complete | [VERSTAPOST_REQUIREMENTS_TRACKER.xlsx](VERSTAPOST_REQUIREMENTS_TRACKER.xlsx) |

### Critical Notes

- V1 is strong enough for review.
- The main remaining V1 weakness is that the tradeoff analysis is a document rather than a dedicated notebook.
- This is partly covered by [TECH_COMPARISON.md](../specification/TECH_COMPARISON.md) and the Estonian comparative analysis in [Vordlev_analyys_summary.md](../analysis/Vordlev_analyys_summary.md).

## 3. Verstapost 2: Key Technology Prototype

### Evidence Already Present

| Requirement | Status | Evidence |
| --- | --- | --- |
| Web UI to robot control chain | implemented | [main](../../firmware/main) |
| Motor control | implemented and measured | [movement_speed_test.csv](../../tests/results/movement_speed_test.csv) |
| Camera stream | implemented | [camera firmware](../../firmware/camera/sketch_jan17a/sketch_jan17a.ino), [video](../../media/video/MicrosoftTeams-video.mp4) |
| HTTP latency measurement | measured | [http_latency_analysis.ipynb](../../tests/notebooks/http_latency_analysis.ipynb) |
| ToF integration | implemented | `/tof` endpoint, [tof_stop_test.csv](../../tests/results/tof_stop_test.csv) |
| PCB evidence | present | [hardware/pcb/README.md](../../hardware/pcb/README.md) |

### Still Weak for V2

- no direct current-draw measurement under motor load;
- power brownout table is filled, but camera endpoint failures require retest;
- no formal PCB DRC screenshot/export;
- ToF static accuracy is measured, but the raw sensor values show a large offset and need calibration or explanation.

## 4. Verstapost 3: Integrated Technical Prototype

### Evidence Already Present

| Requirement | Status | Evidence |
| --- | --- | --- |
| Unified web control surface | implemented | [01_WebUi.ino](../../firmware/main/01_WebUi.ino) |
| Motors + sensors + cameras in one UI | implemented | [main](../../firmware/main), [cam](../../firmware/camera/sketch_jan17a) |
| Tap/click-to-drive | prototype implemented | [01_WebUi.ino](../../firmware/main/01_WebUi.ino) |
| ToF telemetry and stop behavior | implemented and partly measured | [tof_stop_test.csv](../../tests/results/tof_stop_test.csv) |
| Color telemetry | implemented | Cam 2 `TCS34725` endpoint and UI panel |
| Simple autonomous behavior | prototype implemented | UI auto mode |

### Still Weak for V3

- tap-drive is timed steering, not calibrated camera geometry;
- no OpenCV calibration notebook;
- no homography/raycast calibration file;
- no measured click-to-drive target accuracy;
- boundary/color sensing is diagnostic only, not closed-loop retreat behavior.

## 5. Verstapost 4: Functional Prototype with Enclosure

### Evidence Already Present

| Requirement | Status | Evidence |
| --- | --- | --- |
| Enclosure/body CAD | present | [hardware/cad/ver3](../../hardware/cad/ver3) |
| Real mechanical CAD package | present | [hardware/cad/real](../../hardware/cad/real) |
| Integrated firmware | present | [main](../../firmware/main), [cam](../../firmware/camera/sketch_jan17a) |
| Movement and sensor evidence | partially present | [tests/results](../../tests/results) |
| Functional prototype status | tracked | [VERSTAPOST_REQUIREMENTS_TRACKER.xlsx](VERSTAPOST_REQUIREMENTS_TRACKER.xlsx) |

### Still Weak for V4

- no photo evidence of assembled enclosure in repository;
- print settings guide exists, but final material-specific validation is still pending;
- no impact test table;
- no heat test table;
- no battery replacement/service time;
- no repeated match endurance log;
- red-area behavior is not yet match-scoring/autonomous.

## 6. Verstapost 5: Production-Ready System

### Evidence Already Present

| Requirement | Status | Evidence |
| --- | --- | --- |
| Repository structure | present | [README.md](../../README.md) |
| Milestone requirement tracker | present | [VERSTAPOST_REQUIREMENTS_TRACKER.xlsx](VERSTAPOST_REQUIREMENTS_TRACKER.xlsx) |
| Supporting documentation | present | [docs index](../README.md), [README](../../README.md) |
| CAD/hardware/pcb/Firmware package | present | [cad](../../hardware/cad), [PCB](../../hardware/pcb), [main](../../firmware/main), [cam](../../firmware/camera) |
| Known limitations | documented | [VERSTAPOST_REQUIREMENTS_TRACKER.xlsx](VERSTAPOST_REQUIREMENTS_TRACKER.xlsx), this plan |

### Still Weak for V5

- assembly guide exists, but still needs final photo evidence;
- flashing/setup guide exists with board/FQBN/COM notes, but should be validated once more on all boards;
- print/material guide exists, but final material-specific validation is still pending;
- no final release BOM with exact sources/prices for every part;
- final integrated measurement notebook is present, but should be updated after new tests;
- no ArUco/global arena positioning;
- no final endurance evidence;
- power robustness has an initial brownout CSV, but camera endpoint behavior needs retest.

## 7. Priority Fix Order

1. Calibrate or explain the measured ToF accuracy offset in [tof_accuracy_test.csv](../../tests/results/tof_accuracy_test.csv).
2. Re-run [power_brownout_test.csv](../../tests/results/power_brownout_test.csv) with Cam1/Cam2 confirmed online, because the main controller stayed online but camera endpoints failed.
3. Add color repeatability and tap-drive accuracy CSV templates or measurements.
4. Add final endurance, enclosure heat, impact, and battery-service tests.
5. Add final photo evidence to [ASSEMBLY_GUIDE.md](../guides/ASSEMBLY_GUIDE.md).
6. Update [test_results_analysis.ipynb](../../tests/notebooks/test_results_analysis.ipynb), README, and the Excel tracker after each new measurement.

## 8. Honest Defense Narrative

The project has moved beyond a paper concept. It contains working ESP32/M5 firmware, browser control, camera streams, ToF telemetry, Cam 2 color telemetry, movement measurements, CAD, PCB evidence, and one Excel milestone tracker that links the evidence.

The remaining work is not basic bring-up. It is calibration, repeatable measurement, mechanical validation, production documentation, and final autonomous arena behavior.
