# ATOM Sumo

`ATOM Sumo` is a SMARS-inspired sumo robot prototype for the University of Tartu prototyping course.

The project combines:
- an `M5 ATOM` main controller;
- `2 x M5 AtomS3R-CAM` camera nodes;
- `2S 18650` battery power;
- `TB6612FNG` motor driver;
- `4 x N20 6V` drive motors, wired as left/right parallel motor pairs;
- `VL53L0X` front ToF distance sensing;
- `TCS34725` floor color telemetry on Cam 2;
- a browser-based control interface over the robot's own Wi-Fi access point.

The long-term goal is one reliable working prototype plus enough documentation to support small-batch replication before the arena event.

## Current architecture

- Main controller firmware: [`firmware/main/main.ino`](firmware/main/main.ino)
- Camera firmware: [`firmware/camera/sketch_jan17a/sketch_jan17a.ino`](firmware/camera/sketch_jan17a/sketch_jan17a.ino)
- Documentation index: [`docs/README.md`](docs/README.md)
- Power concept: [`docs/specification/power_scheme.md`](docs/specification/power_scheme.md)
- Electrical block diagram: [`hardware/diagrams/power_tree.drawio`](hardware/diagrams/power_tree.drawio)
- System block diagram: [`hardware/diagrams/system_block.drawio`](hardware/diagrams/system_block.drawio)
- PCB package: [`hardware/pcb/README.md`](hardware/pcb/README.md)
- Formal V1 specification: [`docs/specification/V1_specification.md`](docs/specification/V1_specification.md)
- Requirements tracker Excel: [`docs/milestones/VERSTAPOST_REQUIREMENTS_TRACKER.xlsx`](docs/milestones/VERSTAPOST_REQUIREMENTS_TRACKER.xlsx)
- Technical comparison: [`docs/specification/TECH_COMPARISON.md`](docs/specification/TECH_COMPARISON.md)
- Estonian comparative analysis: [`docs/analysis/Vordlev_analyys.docx`](docs/analysis/Vordlev_analyys.docx), [`docs/analysis/Vordlev_analyys_summary.md`](docs/analysis/Vordlev_analyys_summary.md)
- Full test-results notebook: [`tests/notebooks/test_results_analysis.ipynb`](tests/notebooks/test_results_analysis.ipynb)
- Notebook launcher: [`tools/RUN_TEST_RESULTS_NOTEBOOK.ps1`](tools/RUN_TEST_RESULTS_NOTEBOOK.ps1)
- Original Verstapost brief images: [`docs/assets/briefs`](docs/assets/briefs)

Current hardware baseline:
- differential `4WD` drive with `2` motors wired in parallel on each side;
- `6.0V` motor rail for `4 x N20 6V` motors;
- separate `5.0V` logic rail for controller, cameras, and sensors;
- robot Wi-Fi AP `Robot-Control` with browser control UI.

Reference build components:

| Component | Quantity | Role / current status |
| --- | ---: | --- |
| `M5 ATOM` main controller | 1 | Main ESP32 controller for Wi-Fi AP, web UI, motor control, ToF, and robot state |
| `M5 AtomS3R-CAM` | 2 | Cam 1 front/operator stream and Cam 2 upward/downward diagnostic/color node |
| `VL53L0X` ToF module | 1 | Front distance sensing over I2C |
| `TCS34725` / `CJMCU-34725` RGB color sensor | 1 used / 2 ordered | Floor color telemetry on Cam 2 Grove I2C |
| `TB6612FNG` dual motor driver | 1 | Two-channel drive; left motor pair on channel A and right motor pair on channel B |
| `N20 6V` DC gearmotor | 4 | Four-wheel drive, two motors wired in parallel per side |
| Driven wheel pair `42 x 19 mm` | 2 pairs | Four driven wheels total |
| `2 x 18650` Li-Ion cells | 2 | Main `2S` battery pack |
| `2x18650` battery holder | 1 | Battery mount for the reference build |
| `2S 3A` BMS/protection board | 1 | Battery protection, still requires high-load validation |
| `1-2S` Li-Ion charger module | 1 | Charging path for the 2S pack |
| USB-C PD trigger module | 1 | Bench/charging input source used in the power setup |
| DC-DC step-down module `5A` | 2 | One set to `5.0V` logic rail, one set to `6.0V` motor rail |
| `1000uF / 16V` capacitor | 1 | Motor rail bulk decoupling near the driver |
| Slide switch | 1 | Master power switch |
| Custom PCB / interconnect board | 1 design | EasyEDA/Gerber evidence present; latest Gerber package is included |
| 3D-printed chassis/enclosure | 1 set | CAD and photo evidence present; final heat/impact/service validation still open |

## Current status

Already available in the repository:
- working main control sketch with AP mode and web control UI;
- working `AtomS3R-CAM` streaming sketch;
- V1 specification and BOM;
- power tree and PCB artifacts;
- latency, movement, ToF, and brownout measurements with executed analysis notebooks;
- CAD for the baseline chassis, functional enclosure, and real Fusion 360 robot assembly.

Important current limitation:
- final enclosure/endurance validation is still pending; the robot has movement CSVs, but heat, impact, battery-service, and repeated match-like tests still need measured tables.

## Submission tracker

The main submission document is:

- [`docs/milestones/VERSTAPOST_REQUIREMENTS_TRACKER.xlsx`](docs/milestones/VERSTAPOST_REQUIREMENTS_TRACKER.xlsx)

It lists all Verstapost 1-5 requirements in order, marks whether each item is complete, and links to the evidence files.

## Key results

HTTP control latency measurements are stored in [`tests/results`](tests/results).

Most useful current runs:
- before motor A fix: [`http_latency_2026-04-11_18-28-33.csv`](tests/results/http_latency_2026-04-11_18-28-33.csv)
- after motor A fix at `speed=0`: [`http_latency_2026-04-11_19-34-31.csv`](tests/results/http_latency_2026-04-11_19-34-31.csv)
- after fix at `speed=180`: [`http_latency_2026-04-11_19-38-18_medium_speed.csv`](tests/results/http_latency_2026-04-11_19-38-18_medium_speed.csv)

Summary:
- `speed=0` after fix: median `31.538 ms`, p95 `53.042 ms`, max `68.883 ms`
- `speed=180`: median `87.373 ms`, p95 `158.018 ms`, max `186.038 ms`

Detailed write-up:
- [`docs/testing/latency_test_result.md`](docs/testing/latency_test_result.md)
- [`tests/notebooks/http_latency_analysis.ipynb`](tests/notebooks/http_latency_analysis.ipynb)
- [`tests/notebooks/test_results_analysis.ipynb`](tests/notebooks/test_results_analysis.ipynb)

## Milestone evidence

Detailed evidence is linked from the Excel tracker and from the documentation index. The most important supporting areas are:

- [`docs/specification`](docs/specification) - specification, BOM, power scheme, and comparison analysis.
- [`docs/testing`](docs/testing) - test plan and test protocols.
- [`tests/results`](tests/results) - CSV measurement data.
- [`tests/notebooks`](tests/notebooks) - analysis notebooks.
- [`firmware`](firmware) - main controller and camera firmware.
- [`hardware`](hardware) - CAD, PCB, and diagrams.

## Repository structure

### Firmware

- [`firmware/main/`](firmware/main)  
  Main robot controller firmware: Wi-Fi AP, web UI, motor control, speed control, command handling.

- [`firmware/camera/`](firmware/camera)  
  Firmware for the `M5 AtomS3R-CAM` nodes that provide MJPEG video streams.

### Tools

- [`tools/`](tools)  
  Double-click PowerShell launchers for measurement runs and notebook regeneration.

### Documentation

- [`docs/README.md`](docs/README.md)  
  Documentation index and fastest way into the Excel tracker and evidence files.

- [`docs/specification/`](docs/specification)  
  System specification, BOM, power scheme, and technical comparison.

- [`docs/guides/`](docs/guides)  
  Assembly, flashing, and print-setting guides.

- [`docs/testing/`](docs/testing)  
  Test plan, movement/sensor protocol, and latency write-up.

- [`docs/milestones/`](docs/milestones)  
  Main Excel tracker and milestone status.

- [`docs/assets/`](docs/assets)  
  Milestone brief images, order screenshots, and photo evidence.

### Tests and measurements

- [`tests/README.md`](tests/README.md)  
  Index for executable tests, measurement CSV files, and notebooks.

- [`tests/scripts/`](tests/scripts)  
  Python scripts used to collect latency, brownout, and ToF accuracy data.

- [`tests/notebooks/`](tests/notebooks)  
  Jupyter notebooks with tables, graphs, and interpretation.

- [`tests/results/`](tests/results)  
  Raw CSV files and summary text files from executed tests.

### Electronics and diagrams

- [`hardware/diagrams/power_tree.drawio`](hardware/diagrams/power_tree.drawio)  
  Editable draw.io electrical block diagram.

- [`hardware/diagrams/system_block.drawio`](hardware/diagrams/system_block.drawio)  
  Editable top-level system architecture diagram.

- [`hardware/pcb/`](hardware/pcb)  
  PCB screenshots, schematic/layout source exports, 3D view, and Gerber archives. Start with [`hardware/pcb/README.md`](hardware/pcb/README.md).

### Mechanical

- [`hardware/cad/ver1/chassis_n20.scad`](hardware/cad/ver1/chassis_n20.scad)  
  Parametric OpenSCAD chassis model.

- [`hardware/cad/ver1/chassis_n20.stl`](hardware/cad/ver1/chassis_n20.stl)  
  Exported printable chassis geometry.

- [`hardware/cad/ver3/`](hardware/cad/ver3)  
  Functional enclosure/body models and rendered views.

- [`hardware/cad/real/`](hardware/cad/real)  
  Real Fusion 360 robot assembly and parts, including body, shell, top cover, battery pack, wheels, N20 motor reference, PCB body, and M5 Atom placement.

### Media

- [`docs/assets/photos/README.md`](docs/assets/photos/README.md)
  Photo evidence for fabrication, electronics integration, printed enclosure parts, and assembled robot state.

- [`media/video/README.md`](media/video/README.md)
  Index of video evidence with measured durations.

- [`media/video/MicrosoftTeams-video.mp4`](media/video/MicrosoftTeams-video.mp4)  
  Demo evidence showing camera streaming.

- [`media/video/0-02-05-5cafa7a371385dc5dc37026175887d8e437ea1f9ac49a73fc301792a9de68af1_9c676645fed6c17c.mp4`](media/video/0-02-05-5cafa7a371385dc5dc37026175887d8e437ea1f9ac49a73fc301792a9de68af1_9c676645fed6c17c.mp4)
  Additional robot/camera demonstration video evidence.

## Recommended reading order

If you are opening this repository for the first time, start here:

1. [`docs/specification/V1_specification.md`](docs/specification/V1_specification.md)
2. [`docs/milestones/VERSTAPOST_REQUIREMENTS_TRACKER.xlsx`](docs/milestones/VERSTAPOST_REQUIREMENTS_TRACKER.xlsx)
3. [`docs/specification/TECH_COMPARISON.md`](docs/specification/TECH_COMPARISON.md)
4. [`docs/testing/test_plan.md`](docs/testing/test_plan.md)
5. [`docs/specification/BOM.csv`](docs/specification/BOM.csv)
6. [`docs/specification/power_scheme.md`](docs/specification/power_scheme.md)
7. [`docs/testing/test_plan.md`](docs/testing/test_plan.md)
8. [`firmware/main/main.ino`](firmware/main/main.ino)
9. [`firmware/camera/sketch_jan17a/sketch_jan17a.ino`](firmware/camera/sketch_jan17a/sketch_jan17a.ino)
10. [`tests/notebooks/http_latency_analysis.ipynb`](tests/notebooks/http_latency_analysis.ipynb)

## Next project steps

Planned next technical steps:
- calibrate or explain the measured `VL53L0X` offset;
- run the full auto-behavior logger test after charging the robot;
- complete enclosure heat/impact/service/endurance tests;
- add final multi-angle assembly photos and measured validation tables.
