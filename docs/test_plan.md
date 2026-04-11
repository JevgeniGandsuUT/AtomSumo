# Test Plan

This document summarizes the practical verification plan for the current `ATOM Sumo` baseline before the next defense.

## Scope

The current test plan focuses on:

- Wi-Fi access point and browser control;
- manual drive commands;
- camera streaming;
- HTTP latency measurements;
- power-system inspection;
- future integration points for ToF, boundary sensing, and click-to-drive.

It does not yet claim a full mechanical validation, because the chassis is still not fully assembled.

## Current verified tests

### 1. Access point and UI test

Objective:
- confirm that the robot starts its own Wi-Fi network and serves the control page.

Method:
- power on the controller;
- verify that SSID `Robot-Control` appears;
- connect a client device;
- open `http://192.168.4.1/`.

Pass condition:
- the AP is visible;
- the web page opens successfully;
- the UI loads without external infrastructure.

Evidence:
- main firmware: [../sketch_mar21a/sketch_mar21a.ino](../sketch_mar21a/sketch_mar21a.ino)

### 2. Camera stream test

Objective:
- confirm that at least one real camera stream is operational through the robot network.

Method:
- power the `AtomS3R-CAM` node;
- connect it to the robot AP;
- open the stream from the web page or direct stream endpoint;
- visually verify live video.

Pass condition:
- at least one camera stream loads and updates continuously.

Evidence:
- camera firmware: [../cam/sketch_jan17a.ino](../cam/sketch_jan17a.ino)
- demo video: [../video/MicrosoftTeams-video.mp4](../video/MicrosoftTeams-video.mp4)

### 3. Manual drive command test

Objective:
- verify that the UI can send motion commands to the robot.

Method:
- send `forward`, `backward`, `left`, `right`, and `stop` from the web UI;
- confirm motor response on the bench or on a raised test stand;
- verify speed control using the UI speed slider.

Pass condition:
- all direction commands are accepted;
- both motor channels respond as expected;
- stop command always works.

Evidence:
- control firmware: [../sketch_mar21a/sketch_mar21a.ino](../sketch_mar21a/sketch_mar21a.ino)

### 4. HTTP latency test

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
- script: [../analysis/http_latency_test.py](../analysis/http_latency_test.py)
- analysis notebook: [../analysis/http_latency_analysis.ipynb](../analysis/http_latency_analysis.ipynb)
- results: [../analysis/results](../analysis/results)
- interpretation: [latency_test_result.md](./latency_test_result.md)

## Still pending tests

### 5. Motor characterization test

Status:
- pending until the rolling chassis is assembled.

Planned measurements:
- straight-line speed;
- startup behavior;
- current consumption;
- thrust / loaded movement.

### 6. ToF sensor test

Status:
- pending firmware integration of `VL53L0X`.

Planned measurements:
- distance repeatability;
- practical range on sumo-relevant targets;
- update-rate suitability for obstacle detection.

### 7. Boundary detection test

Status:
- pending implementation.

Planned measurements:
- detection reliability on arena colors;
- stop or retreat behavior before crossing the border;
- repeatability over multiple runs.

### 8. Click-to-drive test

Status:
- pending implementation.

Planned measurements:
- target-point accuracy on the arena grid;
- repeatability from a fixed starting pose;
- error versus distance and image position.

## Recommended defense wording

The most accurate current statement is:

> The communication path, browser control, and camera-stream baseline are already verified. The remaining missing validation is mainly mechanical integration and sensor integration, not proof of the core control architecture.
