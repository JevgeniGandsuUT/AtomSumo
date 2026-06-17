# Vordlev Analyys Summary

Source document:
- [Vordlev_analyys.docx](Vordlev_analyys.docx)

This document summarizes the Estonian comparative analysis file and aligns it with the current repository architecture.

## Compared Robots

| Robot | Description |
| --- | --- |
| Robot 1 | Advanced Sumo Robot based on Arduino Nano ESP32, high-power multi-motor competition design |
| Robot 2 | JSumo Predator Mini Sumo Robot Kit, commercial mini-sumo platform |
| Tallinn ATOM | Course prototype using M5 ATOM, AtomS3R-CAM modules, `VL53L0X`, `2S 18650`, and `4WD` with N20/GA12-N20 class motors |

## Key Engineering Point

The strongest part of the analysis is the explanation of:

- `traction-limited` systems;
- `motor-limited` systems;
- why high-power competition robots often do not benefit from simply adding more motors;
- why a low-torque learning prototype can benefit from `4WD`.

## Tallinn ATOM Drive Interpretation

The real current build uses:

- `4 x N20 6V` drive motors;
- `4 x 42 x 19 mm` driven wheels;
- two left-side motors wired in parallel to one TB6612 channel;
- two right-side motors wired in parallel to the other TB6612 channel.

This matches the `4WD` reasoning in the Estonian comparative analysis while keeping the firmware model simple:

```text
left motor pair  -> TB6612 channel A
right motor pair -> TB6612 channel B
```

## Current Risk

The 4WD parallel-pair architecture improves available torque, but it increases current per TB6612 channel.

Required validation:

- motor startup current;
- driver heating;
- brownout/reset behavior;
- BMS behavior under sharp turns and pushing.

Relevant initial result file:
- [power_brownout_test.csv](../../tests/results/power_brownout_test.csv)

Current interpretation: the first brownout run did not show main-controller resets or Wi-Fi disconnects, but camera endpoint failures mean the camera-aware test must be repeated with Cam1/Cam2 confirmed online.

## Link to Main Repository Analysis

The repository-level English design rationale is stored here:
- [TECH_COMPARISON.md](../specification/TECH_COMPARISON.md)
