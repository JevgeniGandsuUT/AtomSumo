# ATOM Sumo

`ATOM Sumo` is a SMARS-inspired sumo robot prototype for the University of Tartu prototyping course.

The project combines:
- an `M5 ATOM` main controller;
- `2 x M5 AtomS3R-CAM` camera nodes;
- `2S 18650` battery power;
- `TB6612FNG` motor driver;
- `N20 6V` drive motors;
- a browser-based control interface over the robot's own Wi-Fi access point.

The long-term goal is one reliable working prototype plus enough documentation to support small-batch replication before the arena event.

## Current architecture

- Main controller firmware: [`sketch_mar21a/sketch_mar21a.ino`](sketch_mar21a/sketch_mar21a.ino)
- Camera firmware: [`cam/sketch_jan17a.ino`](cam/sketch_jan17a.ino)
- Power concept: [`docs/power_scheme.md`](docs/power_scheme.md)
- Electrical block diagram: [`drawio/power_tree.drawio`](drawio/power_tree.drawio)
- System block diagram: [`drawio/system_block.drawio`](drawio/system_block.drawio)
- Formal V1 specification: [`docs/V1_specification.md`](docs/V1_specification.md)
- Milestone plan: [`docs/VERSTAPOSTID_PLAN.md`](docs/VERSTAPOSTID_PLAN.md)

Current hardware baseline:
- differential drive with `2` powered wheels and `2` passive wheels;
- `6.0V` motor rail for `N20 6V` motors;
- separate `5.0V` logic rail for controller, cameras, and sensors;
- robot Wi-Fi AP `Robot-Control` with browser control UI.

## Current status

Already available in the repository:
- working main control sketch with AP mode and web control UI;
- working `AtomS3R-CAM` streaming sketch;
- V1 specification and BOM;
- power tree and PCB artifacts;
- first latency measurements and analysis notebook;
- CAD for the chassis baseline.

Important current limitation:
- full mechanical `motor test` is still pending because the chassis is not yet fully assembled.

## Key results

HTTP control latency measurements are stored in [`analysis/results`](analysis/results).

Most useful current runs:
- before motor A fix: [`http_latency_2026-04-11_18-28-33.csv`](analysis/results/http_latency_2026-04-11_18-28-33.csv)
- after motor A fix at `speed=0`: [`http_latency_2026-04-11_19-34-31.csv`](analysis/results/http_latency_2026-04-11_19-34-31.csv)
- after fix at `speed=180`: [`http_latency_2026-04-11_19-38-18_medium_speed.csv`](analysis/results/http_latency_2026-04-11_19-38-18_medium_speed.csv)

Summary:
- `speed=0` after fix: median `31.538 ms`, p95 `53.042 ms`, max `68.883 ms`
- `speed=180`: median `87.373 ms`, p95 `158.018 ms`, max `186.038 ms`

Detailed write-up:
- [`docs/latency_test_result.md`](docs/latency_test_result.md)
- [`analysis/http_latency_analysis.ipynb`](analysis/http_latency_analysis.ipynb)

## Repository structure

### Firmware

- [`sketch_mar21a/`](sketch_mar21a/)  
  Main robot controller firmware: Wi-Fi AP, web UI, motor control, speed control, command handling.

- [`cam/`](cam/)  
  Firmware for the `M5 AtomS3R-CAM` nodes that provide MJPEG video streams.

### Documentation

- [`docs/V1_specification.md`](docs/V1_specification.md)  
  Formal system specification for the first milestone.

- [`docs/VERSTAPOSTID_PLAN.md`](docs/VERSTAPOSTID_PLAN.md)  
  Working milestone checklist and project progress notes.

- [`docs/BOM.csv`](docs/BOM.csv)  
  Reference bill of materials for the selected `2S 18650` robot build.

- [`docs/power_scheme.md`](docs/power_scheme.md)  
  Fixed power architecture for the current robot revision.

- [`docs/test_plan.md`](docs/test_plan.md)  
  Practical verification plan for bench tests, camera tests, and current pending integration tests.

- [`docs/latency_test_result.md`](docs/latency_test_result.md)  
  Human-readable latency test interpretation.

- [`docs/Order1.png`](docs/Order1.png), [`docs/Order2.png`](docs/Order2.png)  
  Order evidence for purchased components.

### Analysis

- [`analysis/http_latency_test.py`](analysis/http_latency_test.py)  
  Script used to measure HTTP request-response latency to the robot.

- [`analysis/http_latency_analysis.ipynb`](analysis/http_latency_analysis.ipynb)  
  Jupyter notebook with tables, graphs, and interpretation for the available latency runs.

- [`analysis/results/`](analysis/results/)  
  Raw CSV files and summary text files from executed tests.

### Electronics and diagrams

- [`drawio/power_tree.drawio`](drawio/power_tree.drawio)  
  Editable draw.io electrical block diagram.

- [`drawio/system_block.drawio`](drawio/system_block.drawio)  
  Editable top-level system architecture diagram.

- [`PCB/`](PCB/)  
  PCB screenshots, schematic exports, and Gerber archives.

### Mechanical

- [`cad/chassis_n20.scad`](cad/chassis_n20.scad)  
  Parametric OpenSCAD chassis model.

- [`cad/chassis_n20.stl`](cad/chassis_n20.stl)  
  Exported printable chassis geometry.

### Media

- [`video/MicrosoftTeams-video.mp4`](video/MicrosoftTeams-video.mp4)  
  Demo evidence showing camera streaming.

## Recommended reading order

If you are opening this repository for the first time, start here:

1. [`docs/V1_specification.md`](docs/V1_specification.md)
2. [`docs/VERSTAPOSTID_PLAN.md`](docs/VERSTAPOSTID_PLAN.md)
3. [`docs/BOM.csv`](docs/BOM.csv)
4. [`docs/power_scheme.md`](docs/power_scheme.md)
5. [`docs/test_plan.md`](docs/test_plan.md)
6. [`sketch_mar21a/sketch_mar21a.ino`](sketch_mar21a/sketch_mar21a.ino)
7. [`cam/sketch_jan17a.ino`](cam/sketch_jan17a.ino)
8. [`analysis/http_latency_analysis.ipynb`](analysis/http_latency_analysis.ipynb)

## Next project steps

Planned next technical steps:
- assemble the rolling chassis;
- integrate `VL53L0X` into the controller firmware;
- complete physical motor characterization;
- validate the `2S 3A` BMS under real motor startup load;
- continue toward V2 and V3 integration milestones.
