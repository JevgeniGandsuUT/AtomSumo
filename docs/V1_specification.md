# V1 System Specification

## Document Control

| Field | Value |
| --- | --- |
| Project | SMARS-inspired Sumo Robot |
| Internal name | ATOM Sumo |
| Version | 1.0 |
| Date | 2026-04-11 |
| Status | Draft for Verstapost 1 |
| Primary architecture | M5 ATOM main controller + 2 x M5 AtomS3R-CAM + 2S 18650 power system + N20 6V drive motors |

## 1. Purpose and Scope

This document defines the baseline system specification for a compact sumo robot intended for the University of Tartu prototyping course. The specification is written so that another team could reproduce the reference robot architecture, understand the current design decisions, and continue development through Verstapost 2-5.

The reference design standardizes on:

- an M5 ATOM controller as the robot's main brain;
- two M5 AtomS3R-CAM modules as network cameras;
- a `2S 18650` battery pack as the main energy source;
- differential drive with two N20 6V DC gearmotors and four-wheel baseline packaging;
- a browser-based operator interface over the robot's own Wi-Fi network.

This document describes the target system behavior, the current implementation baseline already present in the repository, and the required validation work still needed before full arena deployment.

## 2. Repository Baseline

The current specification is grounded in the following repository artifacts:

- [sketch_mar21a/sketch_mar21a.ino](../sketch_mar21a/sketch_mar21a.ino)
- [cam/sketch_jan17a.ino](../cam/sketch_jan17a.ino)
- [cad/chassis_n20.scad](../cad/chassis_n20.scad)
- [cad/chassis_n20.stl](../cad/chassis_n20.stl)
- [PCB/Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08.zip](../PCB/Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08.zip)
- [PCB/Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08-2.zip](../PCB/Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08-2.zip)
- [video/MicrosoftTeams-video.mp4](../video/MicrosoftTeams-video.mp4)

Observed baseline from those files:

- the main controller already creates Wi-Fi AP `Robot-Control`;
- the browser UI already supports manual drive and speed control;
- the UI already embeds two camera streams at `192.168.4.20` and `192.168.4.21`;
- the camera firmware already serves `/stream` and `/capture`;
- the CAD model already reserves space for front camera, downward camera, ToF window, PCB area, and a DC/DC converter;
- PCB Gerber and EasyEDA source files already exist.
- a recorded video artifact in `video/MicrosoftTeams-video.mp4` provides practical confirmation that streaming was demonstrated; the file is approximately `32 s` long.

## 3. System Overview

### 3.1 Mission

The robot shall operate inside a `3000 x 3000 mm` arena assembled from `1000 x 1000 mm` OSB modules with a `100 mm` hole grid, colored surface inserts, and `100 mm` high perimeter walls. The robot shall support both operator-controlled and semi-autonomous sumo behavior.

### 3.2 Top-Level Functions

The system shall:

- move using differential drive;
- stream live video to a browser UI;
- measure front obstacle distance;
- detect arena surface and boundary conditions;
- estimate robot position on the arena at least coarsely;
- accept manual drive commands and later click-to-drive commands;
- run from an onboard rechargeable `2S 18650` battery pack.

### 3.3 Selected Architecture

The selected architecture is a practical refinement of course Variant B:

- main controller: M5 ATOM module;
- front operator camera: M5 AtomS3R-CAM;
- downward arena camera: M5 AtomS3R-CAM;
- front distance sensor: `VL53L0X` ToF over `I2C`;
- motor driver: dual `TB6612FNG`;
- power system: `2S 18650` pack with charger, BMS, master switch, and separate logic and motor regulators.

This choice is based on the actual code and CAD already present in the repository.

## 4. Operating Environment and Constraints

### 4.1 Arena

The robot is designed for the course arena with the following properties:

- arena size: `3000 x 3000 mm`;
- module size: `1000 x 1000 mm`;
- hole pitch: `100 mm`;
- hole diameter: `5 mm`;
- surface: OSB with removable colored inserts no higher than `2 mm` above the floor;
- wall height: `100 mm`.

### 4.2 Project Constraints

- total robot budget target: `<= 300 EUR`;
- build shall be reproducible using off-the-shelf parts plus 3D-printed parts;
- system shall be documented inside the repository;
- the V1-V2 baseline may use wheels for rapid iteration, but the motor and chassis layout shall remain compatible with a later SMARS-style tracked revision if that becomes advantageous.

### 4.3 Assumptions

- The M5 ATOM main controller and the two M5 AtomS3R-CAM boards are assumed to be available, because working firmware for them already exists in the repository.
- The exact motor gearbox ratio is not yet frozen and shall be confirmed through motor sizing and push testing, but the motor voltage class is fixed at `6V`.
- The current CAD model still contains a legacy powerbank bay and must be revised for the final `2S 18650` battery holder.

## 5. Functional Requirements

The following functional requirements are prioritized using the `MoSCoW` method.

| ID | Requirement | Priority | Verification |
| --- | --- | --- | --- |
| FR-01 | The robot shall create its own Wi-Fi access point and allow the operator to connect without an external router. | Must | Bench test |
| FR-02 | The operator shall control the robot from a browser-based web UI running on a phone or laptop. | Must | Bench test |
| FR-03 | The web UI shall provide `forward`, `backward`, `left`, `right`, `stop`, and speed control. | Must | Bench test |
| FR-04 | The system shall provide a live front camera stream for operator vision. | Must | Bench test |
| FR-05 | The system shall provide a live downward-facing camera stream for arena observation, calibration, and positioning. | Must | Bench test |
| FR-06 | The robot shall use differential drive with two independently controlled motors. | Must | Bench test |
| FR-07 | The robot shall measure forward obstacle distance using a `VL53L0X` ToF sensor over `I2C`. | Must | Bench test |
| FR-08 | The robot shall be powered by an onboard `2S 18650` battery pack with charging and protection circuitry. | Must | Bench test |
| FR-09 | The power system shall provide separated logic and motor supply rails with a common ground. | Must | Bench and wiring inspection |
| FR-10 | The robot shall detect arena boundary or unsafe floor state and command a stop or retreat. | Must | Arena test |
| FR-11 | The operator UI shall display basic robot state, including at minimum connection status, speed setting, and camera availability; later revisions shall add battery, ToF, and boundary state. | Should | UI test |
| FR-12 | The robot shall estimate coarse arena pose using the downward camera and arena visual features such as colored inserts, grid geometry, or fixed markers. | Should | Arena test |
| FR-13 | The operator shall be able to click a point in the front camera image and command the robot to drive toward the corresponding ground-plane target. | Should | Arena test |
| FR-14 | The robot shall support both manual mode and autonomous sumo mode. | Should | Integration test |
| FR-15 | The system should log latency, distance, and battery data during development tests. | Could | Data logging test |
| FR-16 | The system could expose a machine-readable telemetry endpoint for notebooks and automated tests. | Could | API test |
| FR-17 | The system will not depend on an Android phone as the main compute platform in this reference design. | Won't | Design review |
| FR-18 | The V1-V2 baseline will not require UWB localization hardware. | Won't | Design review |

## 6. Performance Targets

The following are target values for the reference prototype. They are engineering targets, not yet measured acceptance results.

| Parameter | Target | Notes |
| --- | --- | --- |
| Maximum body footprint | `<= 150 x 150 mm` | Current CAD body is `150 x 108 mm` |
| Minimum ground clearance | `>= 10 mm` | From CAD target |
| Nominal top speed | `0.4-0.6 m/s` | To cross the arena quickly while remaining controllable |
| 180 degree turn-in-place time | `<= 1.2 s` | At nominal `6.0V` motor rail |
| Push force at wheels | `>= 4 N` target | Final value to be validated with sled test |
| Manual command latency | median `<= 100 ms`, p95 `<= 150 ms` | Button press to observed motor response |
| Camera stream rate | `>= 10 fps` at `QVGA` | Existing camera firmware already targets streaming |
| Boot to usable control page | `<= 20 s` | Power-on to reachable web UI |
| Active runtime | `>= 15 min` or `>= 5` matches of `3 min` with reserve | Based on `2S 2500 mAh` pack |
| Boundary stop distance | `< 100 mm` worst case | Must remain inside 100 mm warning zone |
| Click-to-drive position error | `<= 150 mm` at `0.5 m`, `<= 250 mm` at `1.0 m` | Target for V3 |
| Front distance sensing range | `50 mm` to `2 m` practical target | Final practical limit depends on target material |

## 7. Mechanical Specification

### 7.1 Chassis Concept

The robot uses a compact monocoque-style 3D-printed chassis modeled in OpenSCAD.

Baseline geometry from the current CAD:

- body length: `150 mm`;
- body width: `108 mm`;
- hull height: `34 mm`;
- ground clearance target: `10 mm`;
- PCB inner area: `60.5 x 60.5 mm`;
- motor cavity designed around `N20 6V` motors;
- front wedge/scoop for pushing;
- dedicated apertures for front camera, downward camera, and ToF sensor.

### 7.2 Drive Train

The reference drive train is:

- 2 x `N20 6V` DC gearmotors (`N20` class footprint assumed by CAD);
- dual-motor differential steering;
- 4 wheels total in the V1-V2 wheel baseline;
- 2 x `42 x 19 mm` wheels used as the driven motorized pair;
- 2 x `42 x 19 mm` wheels used as a passive free-rolling support pair without motors.

The wheel layout therefore uses:

- one left and one right driven wheel for propulsion and turning;
- one left and one right passive support wheel for stability and load distribution.

The current CAD still includes a rear support axle concept, so the chassis shall be revised if the passive rear-wheel layout is adopted as the final wheel baseline.

The wheel-based baseline is acceptable for early prototyping because it accelerates testing of:

- motor selection;
- current draw;
- driver behavior;
- web control latency;
- battery performance.

The drive architecture intentionally preserves the option to replace the wheel module with SMARS-style tracks later without changing the top-level electrical or software architecture.

### 7.3 Camera and Sensor Placement

- Front camera: mounted in the front upper shell and used for operator vision and click-to-drive projection.
- Downward camera: mounted near the front underside and used for floor observation, color/boundary detection, and coarse positioning.
- ToF sensor: mounted in the front shell in a separate window offset from the front camera.

### 7.4 Battery Packaging Revision

The current CAD still contains a `powerbank bay`. For the final `2S 18650` architecture this shall be replaced by:

- a `2 x 18650` holder or custom printed battery tray;
- a protected and serviceable battery mount;
- easy access for cell replacement and wiring inspection;
- placement that keeps the center of mass low and near the driven axle.

## 8. Electrical Specification

### 8.1 Power Tree

The reference power tree is:

```text
USB-C PD Trigger (12 V input source during charging/setup)
    -> 1-2S Li-Ion charger module
    -> 2S 18650 pack
    -> 2S BMS / protection module
    -> master power switch
        -> Buck regulator #1 -> 5.0 V logic rail
        -> Buck regulator #2 -> 6.0 V motor rail
```

Logic rail consumers:

- M5 ATOM main controller;
- 2 x M5 AtomS3R-CAM;
- `VL53L0X`;
- future low-power sensors and status LEDs.

Motor rail consumers:

- `TB6612FNG` VM input on `6.0V` motor rail;
- left and right `N20 6V` drive motors.

### 8.2 Power Design Requirements

- All modules shall share a common electrical ground.
- Logic and motor rails shall be separately regulated to reduce brownouts and motor noise coupling.
- A bulk capacitor of approximately `1000 uF / 16 V` shall be placed near the motor driver supply.
- The logic rail shall be fixed at `5.0V`.
- The motor rail shall be fixed at `6.0V` for the selected `N20 6V` motors.
- Raw `8.4 V` from a fully charged `2S` pack shall not be applied directly to the `N20 6V` motors.

### 8.3 Main Controller Pin Mapping

The current firmware defines the following control pins:

| Signal | Pin |
| --- | --- |
| `PWMA` | `22` |
| `AIN1` | `19` |
| `AIN2` | `23` |
| `PWMB` | `25` |
| `BIN1` | `21` |
| `BIN2` | `33` |
| `ATOM_LED_PIN` | `27` |

These pins are already used by the main controller firmware in [sketch_mar21a/sketch_mar21a.ino](../sketch_mar21a/sketch_mar21a.ino).

### 8.4 Camera Networking

The current camera firmware uses the following network assignments:

| Device | Mode | Address |
| --- | --- | --- |
| Main controller | Wi-Fi AP | `192.168.4.1` |
| Front camera node | Wi-Fi STA | `192.168.4.20` |
| Downward camera node | Wi-Fi STA | `192.168.4.21` |

The reference SSID in current firmware is `Robot-Control`.

### 8.5 ToF Interface

The front distance sensor shall be a `VL53L0X` connected via `I2C`. Required outputs:

- filtered distance value in millimeters;
- validity or timeout status;
- update rate sufficient for obstacle avoidance and telemetry.

## 9. Software Architecture

### 9.1 Main Controller Firmware

The main controller firmware is responsible for:

- creating the robot Wi-Fi access point;
- serving the operator web UI;
- receiving manual drive commands;
- setting motor direction and PWM;
- exposing speed control;
- integrating future telemetry and sensor endpoints;
- later managing mode switching between manual and autonomous behavior.

Current implemented HTTP endpoints in the repository:

- `GET /` -> control UI;
- `GET /move?dir=forward|backward|left|right|stop`;
- `GET /speed?value=0..255`.

### 9.2 Camera Node Firmware

Each M5 AtomS3R-CAM node is responsible for:

- connecting to the main controller access point;
- serving an MJPEG stream over HTTP;
- serving single snapshot capture on request.

Current camera endpoints in the repository:

- `GET /` -> simple camera page;
- `GET /stream` -> MJPEG stream;
- `GET /capture` -> single JPEG frame.

### 9.3 Operator Web UI

The web UI shall support:

- mobile-friendly and laptop-friendly layout;
- low-friction connection to the robot AP;
- live front camera video;
- live downward camera video or diagnostic view;
- manual directional control;
- stop command;
- speed adjustment slider;
- future mode selection and state display.

### 9.4 Planned Control Modes

The software shall evolve toward the following modes:

- `Manual`: direct operator commands;
- `Click-to-drive`: operator selects a ground target in the image and the robot executes rotate-and-drive;
- `Autonomous`: robot uses ToF and arena sensing to attack and avoid leaving the arena.

## 10. Positioning and Click-to-Drive Specification

### 10.1 Positioning System

The reference design uses the downward-facing camera as the primary positioning sensor.

The downward camera shall estimate coarse robot position and floor state using one or more of:

- arena hole-grid geometry at `100 mm` spacing;
- colored arena inserts;
- fixed printed markers placed on the arena surface or perimeter.

The design intentionally keeps the exact visual marker scheme flexible so that the team can calibrate what works best in the real arena without changing the robot hardware.

### 10.2 Click-to-Drive Logic

The click-to-drive pipeline shall be:

1. The operator clicks a point in the front camera image.
2. The system maps the pixel to a camera ray using calibrated intrinsics.
3. The ray is intersected with the arena ground plane using measured camera height and mounting angle.
4. The ground-plane target is transformed into robot-local coordinates.
5. The robot first rotates toward the target, then drives forward, then stops or corrects based on ToF and floor sensing.

### 10.3 Required Calibration Data

The following values shall be measured and stored:

- front camera focal parameters or calibrated projection model;
- front camera height above ground;
- front camera pitch relative to the ground plane;
- robot center reference point used in accuracy measurements.

### 10.4 Safety Limits for Click-to-Drive

The click-to-drive controller shall reject:

- clicks outside the valid ground region of the camera image;
- targets beyond the configured maximum ground distance;
- targets that intersect known arena boundaries or walls.

## 11. Communication and Data Interface Specification

### 11.1 Network

| Parameter | Value |
| --- | --- |
| Wi-Fi mode | Main controller as AP, camera nodes as STA |
| Reference SSID | `Robot-Control` |
| Security | WPA2 PSK |
| Reference password | stored in firmware and changeable in release builds |

### 11.2 Control API

Current control API baseline:

| Endpoint | Method | Purpose |
| --- | --- | --- |
| `/` | `GET` | Serve control page |
| `/move?dir=...` | `GET` | Direction command |
| `/speed?value=...` | `GET` | Speed setting |

Target telemetry API for later revisions:

| Endpoint | Method | Purpose |
| --- | --- | --- |
| `/state` | `GET` | Return robot telemetry as JSON |
| `/mode` | `POST` or `GET` | Change manual/autonomous mode |
| `/target` | `POST` | Submit click-to-drive target |

Target `/state` JSON fields:

```json
{
  "mode": "manual",
  "speed_pwm": 180,
  "tof_mm": 235,
  "boundary": false,
  "battery_v": 7.6,
  "cam_front": true,
  "cam_down": true
}
```

## 12. Safety, Fault Handling, and Serviceability

The robot shall be designed so that:

- a stop command is always available from the UI;
- the motors default to stop on controller reset;
- the power system includes battery protection;
- camera loss does not cause uncontrolled motion;
- future revisions add command timeout or heartbeat stop behavior;
- the battery is accessible for service between matches;
- wiring is restrained against pull-out during impacts.

Known risk items that must be validated:

- the ordered `2S 3A` BMS may be too weak for motor surge current;
- the exact motor gearbox ratio is not yet frozen and must still be validated by testing;
- the current CAD battery packaging must be revised for `18650`;
- boundary detection is still a planned software feature, not a completed implementation.

## 13. Verification and Test Plan

Each functional requirement shall be verified using one or more of the following methods:

- `Bench test`: stationary electrical or software test on the workbench;
- `Drive test`: flat-floor motion test;
- `Arena test`: test inside the real or mock arena;
- `Inspection`: visual inspection of hardware or CAD;
- `Data analysis`: logged measurements analyzed in a notebook.

### 13.1 Verification Matrix

| Requirement | Planned verification |
| --- | --- |
| FR-01 | Power on the robot and confirm AP visibility and successful client connection |
| FR-02 | Open the browser UI from a phone and a laptop without installing an app |
| FR-03 | Command all directions and verify immediate motor response and stop behavior |
| FR-04 | Confirm front video stream is visible and stable for at least 2 minutes |
| FR-05 | Confirm downward camera stream is visible and aligned to the floor view |
| FR-06 | Run forward, reverse, and turn tests on a flat surface |
| FR-07 | Measure ToF readings at fixed distances such as 5, 10, 20, 30, and 50 cm |
| FR-08 | Run robot from `2S 18650` pack and confirm charging path and protected discharge path |
| FR-09 | Verify separate regulated rails and absence of controller resets during motor start/stop |
| FR-10 | Approach the arena boundary ten times and measure successful stop/retreat rate |
| FR-11 | Confirm state indicators render in the UI and update during operation |
| FR-12 | Measure coarse pose estimate against the arena grid at multiple locations |
| FR-13 | Run click-to-drive to at least five target points and measure position error |
| FR-14 | Demonstrate manual mode and autonomous mode switching |
| FR-15 | Record latency and power measurements to CSV or notebook |
| FR-16 | Query telemetry endpoint and validate field format |
| FR-17 | Design review: confirm no Android phone is required in the reference build |
| FR-18 | Design review: confirm no UWB hardware is required in the reference build |

### 13.2 Priority Acceptance Tests for V2

The most important near-term tests are:

1. Motor drive test: forward, reverse, turn, stop, current draw, and push force.
2. Wi-Fi latency test: `100+` repeated command-response measurements.
3. Camera stability test: continuous streaming over the robot AP.
4. Power test: startup, brownout resistance, and runtime using `2S 18650`.
5. ToF accuracy test: repeated measurements at known distances.

## 14. Preliminary BOM and Procurement Status

The table below reflects the currently chosen architecture for one working reference build. Where a component is not shown in the order screenshots but is clearly implied by the repository, it is marked as assumed available.

| Item | Qty | Status | Approx. cost | Notes |
| --- | --- | --- | --- | --- |
| M5 ATOM main controller | 1 | assumed available | TBD | Working firmware already exists in repo |
| M5 AtomS3R-CAM | 2 | assumed available | TBD | Working camera firmware already exists in repo |
| `VL53L0X` ToF module | 1 | ordered | `9.50 EUR` | Front distance sensing |
| SparkFun `TB6612FNG` dual motor driver | 1 | ordered | `13.36 EUR` | Main dual motor driver |
| USB-C PD trigger module `12 V` | 1 | ordered | `4.28 EUR` | Charging/setup source path |
| `1-2S` Li-Ion charger module | 1 | ordered | `4.28 EUR` | Charging path |
| `2x18650` battery holder | 1 | ordered | `2.38 EUR` | Needs CAD integration |
| `2S 3A` BMS/protection board | 1 | ordered | `2.85 EUR` | Must be surge-tested |
| DC/DC step-down module `5 A` | 2 | ordered | `22.80 EUR` total | One set to `5.0V` for logic, one set to `6.0V` for motors |
| Samsung `18650` `2500 mAh` `20 A` cells | 2 | ordered | `20.90 EUR` total | Main battery pack |
| `42 x 19 mm` wheels | 2 working pairs | ordered | see order history | One pair is motor driven and one pair is passive support |
| `1000 uF / 16 V` capacitor | 1 | ordered | `0.929 EUR` | Motor rail decoupling |
| Slide switch | 1 | ordered | `4.83 EUR` | Master power switch |
| N20 6V DC gearmotors | 2 | TBD or existing stock | TBD | Motor voltage fixed at `6V`; ratio must still be locked by analysis/testing |
| 3D-printed chassis | 1 | CAD complete, print pending | material-dependent | From OpenSCAD model |

Approximate visible reference-build BOM spend from the available order screenshots is about `101.54 EUR`, excluding any lab-stock electronics and any still-unconfirmed motor purchase.

## 15. Open Design Issues

The following issues remain open and shall be closed in later revisions:

1. Replace the current CAD powerbank bay with a finalized `2S 18650` mount.
2. Confirm gearbox ratio from calculation plus bench tests.
3. Validate whether the ordered `2S 3A` BMS can tolerate motor surge current.
4. Implement and validate `VL53L0X` integration in firmware.
5. Implement boundary detection and coarse positioning using the downward camera.
6. Implement telemetry endpoint and click-to-drive.

## 16. Revision Guidance for Verstapost 2

The minimum expected changes for the next revision of this specification are:

- replace assumptions with measured values;
- add the exact battery holder geometry and updated chassis drawing;
- add measured motor speed, current, and push-force data;
- add measured network latency statistics;
- add measured ToF accuracy statistics;
- confirm BMS suitability under startup and stall current conditions.

## 17. Summary

This V1 specification defines a realistic and buildable sumo robot architecture that is already partially implemented in the repository. The design is centered on an M5 ATOM controller, dual M5 AtomS3R-CAM Wi-Fi camera nodes, `2S 18650` battery power, browser-based teleoperation, and a path toward click-to-drive and arena-aware behavior.

The strongest current assets are the existing firmware, existing camera streaming, existing CAD, and existing PCB files. The most important next steps are power integration, motor characterization, ToF integration, and arena sensing.
