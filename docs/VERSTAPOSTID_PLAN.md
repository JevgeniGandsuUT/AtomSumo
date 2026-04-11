# SMARS Sumo Robot: milestone completion document

## 1. What has been verified in the repository

Below is the actual project status based on the files that already exist in the repo.

| File | What exists now | What it covers |
| --- | --- | --- |
| `sketch_mar21a.ino` | ESP32/ATOM controller starts the `Robot-Control` Wi-Fi AP, serves the control web page, shows two camera streams, controls two motors, includes speed control and a captive portal | baseline for `V2`, part of `V3` |
| `cam/sketch_jan17a.ino` | firmware for `AtomS3R-CAM`: MJPEG stream over HTTP, static IPs `192.168.4.20` and `192.168.4.21`, separate `/stream` and `/capture` endpoints | camera pipeline, web video, basis for positioning and telemetry |
| `cad/chassis_n20.scad` | compact `150 x 108 mm` chassis, N20 motor mounts, front camera opening, side/front opening for ToF, lower opening for the second camera, `60 x 60 mm` PCB area, XL6009 mounting | mechanical concept for `V1`, baseline for `V3-V4` |
| `cad/chassis_n20.stl` | exported chassis model | proof of a ready CAD artifact |
| `PCB/Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08.zip` | Gerber package for PCB ordering | `V2` PCB status: Gerber generated |
| `PCB/Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08-2.zip` | EasyEDA source zip with JSON schematic/PCB | proves that the schematic and layout already exist |
| `video/MicrosoftTeams-video.mp4` | video artifact with approximately `32 s` duration | evidence of a demonstrated camera stream |
| `.tmp_index_html_lines.txt`, `.tmp_ui_script.js` | temporary UI exports | should not be treated as primary deliverable files |

## 2. Selected system baseline

### 2.1. Effective architecture based on the repo

Based on the current files, the most logical architecture is:

- main board: `ESP32/ATOM` with web server and motor control;
- camera 1: front-facing `AtomS3R-CAM` for the operator;
- camera 2: downward-facing `AtomS3R-CAM` for zone detection and arena positioning;
- front distance sensor: `VL53L0X` over `I2C`;
- motor driver: `TB6612FNG`;
- power source: `2S 18650`;
- the operator opens the robot web page over Wi-Fi and sees the camera streams.

This matches the idea of `Variant B`, although your implementation uses `AtomS3R-CAM` instead of `M5-CAM`.

### 2.2. What already exists functionally

Already implemented in code:

- forward/backward/left/right/stop movement;
- speed control;
- Wi-Fi web control without an external router;
- dual-camera streaming architecture;
- baseline chassis design;
- PCB artifacts ready for ordering.

Not yet implemented in code:

- `click-to-drive`;
- returning sensor data to the UI;
- `VL53L0X` integration;
- arena boundary / color sensing;
- arena positioning;
- autonomous mode.

## 3. Power decision: `2S 18650`

### 3.1. Fixed variant

The main project power source is `2 x 18650 Samsung 2500 mAh 20A` cells connected in series.

Baseline parameters:

- nominal pack voltage: `7.2-7.4 V`;
- maximum voltage after charging: `8.4 V`;
- capacity: `2.5 Ah`;
- theoretical energy: approximately `18.5 Wh`.

### 3.2. Recommended power scheme

```text
USB-C PD Trigger 12V
    -> 1-2S Li-Ion charger
    -> 2S 18650 pack
    -> 2S BMS
    -> main power switch
        -> Buck #1 -> 5.0V logic rail -> M5 ATOM + 2 x M5 AtomS3R-CAM + VL53L0X
        -> Buck #2 -> 6.0V motor rail -> TB6612 VM + 2 x N20 6V motors
```

Add:

- `1000 uF` capacitor near the motor rail / `TB6612`;
- common `GND` for all modules;
- separate power wiring for cameras/logic and motors.

### 3.3. Critical note

The `2S BMS 3A` may be too weak if both motors generate a large startup or stall current.

This must be checked during `V2`:

- startup current;
- current during sharp turning;
- current when pushing against an obstacle;
- whether the BMS cuts power.

### 3.4. Very important note about motor voltage

Do not apply raw `8.4 V` directly to the motors until their voltage rating is confirmed.

Practical rule:

- the selected motors are `N20 6V`, so `Buck #2` is fixed at `6.0 V`;
- if the motors are `3V`, set it correspondingly lower;
- apply raw `2S` voltage to `TB6612 VM` only if the motors are rated for it.

## 4. What the verified files say about the design

### 4.1. `sketch_mar21a.ino`

This file already works as a demonstration of a working `key technology prototype`:

- the controller starts the `Robot-Control` AP by itself;
- the web page already shows two camera images;
- `/move` and `/speed` already work as platform controls;
- a protective motor `start boost` has already been added.

This is a good basis for defending `V2`, but not yet for `V3+`, because there is no sensor fusion or automation.

### 4.2. `cam/sketch_jan17a.ino`

This file shows that the camera is not only "on paper", but is already designed as a real network node:

- the camera connects to `Robot-Control` as a station;
- it has a static IP;
- it provides an MJPEG stream over HTTP.

This is a strong point for the milestone defense, because it shows a working pipeline:

`operator -> ESP32 AP -> web page -> camera stream`

### 4.3. `cad/chassis_n20.scad`

The CAD already includes:

- front camera placement;
- `ToF` window;
- lower camera placement;
- PCB mounting;
- step-down mounting;
- a compact sumo chassis.

However, there is one important mismatch: the CAD still contains a `powerbank bay`.

Since the project has moved to `18650`, the next CAD revision should:

- remove the dependency on powerbank geometry;
- add a `2x18650 holder` or a custom battery tray;
- review the center of mass so the weight stays lower and closer to the driven wheels.

### 4.4. `PCB/*`

The repo already contains both `Gerber` files and `EasyEDA source`.

This can already be counted as:

- `schematic complete`;
- `component placement started/completed`;
- `Gerber generated`.

But before production, the following must still be checked:

- power input for `2S 18650`;
- motor rail / logic rail routing;
- location for the `1000uF` capacitor;
- connectors for cameras, ToF, and motor driver;
- main power switch.

## 5. Status by milestone

### V1. Specification

#### What can already be shown

- the system is not abstract: real control firmware already exists;
- camera firmware exists;
- chassis CAD exists;
- PCB artifacts exist;
- the `2S 18650` power architecture has been selected.

#### What still needs to be done for a complete `V1`

- [x] write a formal system specification document;
- [x] create `10+` functional requirements with `MoSCoW`;
- [x] add a performance targets table;
- [x] create an electrical block diagram;
- [x] prepare the BOM with prices and suppliers;
- [x] write a testing plan for each requirement;
- [ ] create `Jupyter` analyses for `V1`.

#### What is recommended to submit in the repo

- `docs/V1_specification.md`
- `docs/BOM.csv`
- `docs/power_scheme.md`
- `docs/latency_test_result.md`
- `docs/test_plan.md`
- `drawio/system_block.drawio`
- `drawio/power_tree.drawio`
- `analysis/http_latency_analysis.ipynb`

Still missing if the team wants a stronger `V1` package:

- `analysis/V1_motor_selection.ipynb`
- `analysis/V1_tradeoff.ipynb`
- `analysis/V1_tech_survey.ipynb`

### V2. Key technology prototype

#### What is already supported by the repo

- manual web control is working;
- camera streaming is working;
- there is video proof of the stream in `video/MicrosoftTeams-video.mp4`;
- the drive-system baseline is ready;
- PCB Gerbers already exist;
- the chassis can already be printed and assembled as a test platform.

#### What must be completed for `V2`

- [ ] connect `VL53L0X` and expose its data;
- [ ] assemble the drivetrain using real motors and `TB6612`;
- [ ] measure thrust / speed / current;
- [ ] measure latency over `100+` iterations;
- [ ] update the specification based on the test results;
- [ ] export DRC results/screenshots for the PCB.

#### Minimal real demo for the `14.04.2026` defense

- the robot starts `Robot-Control`;
- the operator opens the web page;
- at least one real camera stream is visible;
- driving by buttons works;
- power comes from `2S 18650` or a bench-equivalent setup with the same scheme;
- first latency and motor test measurements are shown.

### V3. Integrated technical prototype

#### What already exists as a baseline

- the dual-camera architecture is already planned;
- the chassis already reserves space for sensors and cameras;
- the controller and web UI already exist.

#### What is still missing and must be completed

- [ ] integration of all modules on one chassis;
- [ ] front ToF during real movement;
- [ ] arena boundary / color detection;
- [ ] arena positioning;
- [ ] `click-to-drive`;
- [ ] sensor telemetry in the UI.

#### Most realistic path without a major new component order

- front camera = operator view;
- lower camera = software color sensor + local positioning by arena markers / grid;
- `VL53L0X` = front obstacle sensing;
- main ESP32 = motor control + UI.

This matches the existing repo best.

### V4. Functional prototype with enclosure

#### What must be achieved

- [ ] final enclosure for `18650`, not a powerbank;
- [ ] cable management;
- [ ] fast battery replacement;
- [ ] autonomous behavior;
- [ ] impact tests and consecutive match tests.

#### Main technical risk

Without reliable boundary detection, the robot cannot be considered ready for sumo matches.

Therefore the boundary logic must be completed no later than `V3`, not left until the very end.

### V5. Production-ready system

#### What is still missing from the repo

- [x] README with repo structure;
- [ ] assembly guide with photos;
- [ ] validated print settings;
- [ ] final BOM;
- [ ] flashing/setup guide;
- [ ] maintenance guide;
- [ ] integrated measurement notebook from `V2-V4`.

## 6. Main gaps that should be acknowledged honestly

1. The current web UI only supports manual driving, not `click-to-drive`.
2. The repo does not yet include `VL53L0X` integration, although the hardware has already been ordered.
3. The repo does not yet contain a finished boundary/color algorithm.
4. Arena positioning is not yet implemented.
5. The CAD still relies on a `powerbank bay`, not a `2S 18650` battery layout.
6. There is still a risk related to the `2S BMS 3A` for the `18650` setup.

These points should not be hidden during the defense. It is better to frame them as:

- "working baseline achieved";
- "critical subsystems identified";
- "next milestone closes integration and measurement".

## 7. Priority work plan for passing the next milestones

### By `14.04.2026` (1st defense)

- [x] fix the `2S 18650` power scheme;
- [ ] assemble a minimal drivable test stand;
- [x] confirm at least one camera stream in a live demo;
- [x] write `V1_specification.md`;
- [x] create `BOM.csv`;
- [ ] perform the first `latency` and `motor test` measurements.
- [x] perform repeated HTTP `latency` measurements after fixing motor channel A: median `31.538 ms`, p95 `53.042 ms`, max `68.883 ms`; full `motor test` still requires a physical test stand
- [x] perform an HTTP `latency` test at medium motor speed (`speed=180`): median `87.373 ms`, p95 `158.018 ms`, max `186.038 ms`; `forward` is slower than `stop`, but control is still operational

### By `18.04.2026` (3rd meeting / V3 integration start)

- [ ] add `VL53L0X` to the firmware;
- [ ] start real power operation from `2S 18650` with buck converters;
- [ ] rebuild the chassis for the battery holder;
- [ ] expose a telemetry endpoint from the ESP32.

### By `26.05.2026` (2nd defense)

- [ ] boundary detection;
- [ ] lower camera positioning strategy;
- [ ] basic `click-to-drive`;
- [ ] complete robot assembly;
- [ ] notebooks for `V2-V4` tests.

### By `30.05.2026`

- [ ] final enclosure;
- [ ] durability tests;
- [ ] assembly/manual documentation;
- [ ] final repo cleanup.

## 8. Strongest wording for the defense

The following honest narrative can be used:

> We selected an ESP32-based architecture with two cameras. The repository already contains working web control, camera streaming, chassis CAD, and PCB artifacts. After fixing the `2S 18650` power architecture, the next step is to complete power integration, ToF, and boundary/positioning logic in order to move from a key-technology prototype to an integrated technical prototype.

## 9. Important conclusion about the current state

The project is already strong enough for `V1` and a basic `V2`, because it contains real implementations rather than only an idea.

To pass all milestones successfully, the team should focus not on new features but on these four areas:

1. stable `2S 18650` power;
2. `VL53L0X` integration;
3. boundary/positioning logic using the lower camera;
4. documentation and measurable tests.
