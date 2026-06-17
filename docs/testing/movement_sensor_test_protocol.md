# Movement and Sensor Test Protocol

This protocol defines the next practical tests after HTTP latency measurements.
The goal is to collect repeatable evidence for Verstapost 2 and Verstapost 3.

## Test Order

1. Straight-line speed test
2. Straightness / drift test
3. Turn-in-place test
4. ToF stop-distance test
5. ToF distance accuracy test
6. Power brownout / reset test

## Common Setup

- Connect the operator laptop or phone to Wi-Fi SSID `Robot-Control`.
- Open the control UI at `http://192.168.4.1`.
- Use a flat floor or table with enough free space.
- Record video from above or from the side when possible.
- Repeat each movement test at least `3` times per speed setting.
- Use PWM speed values `80`, `150`, `180`, and `255` unless the robot becomes unsafe.

## 1. Straight-Line Speed Test

Objective:
- measure how fast the robot travels at different PWM values.

Method:
- mark a straight `1.0 m` distance on the floor;
- place the robot with its front aligned to the start line;
- set the PWM speed;
- command `forward`;
- measure the time until the front of the robot reaches the finish line;
- press `stop`.

Calculation:

```text
speed_m_s = distance_m / time_s
```

Pass / evidence condition:
- measured values are recorded for at least one usable speed setting;
- the robot can drive the measured path without losing Wi-Fi control.

Suggested data file:
- `tests/results/movement_speed_test.csv`

## 2. Straightness / Drift Test

Objective:
- measure whether the robot drives straight or pulls left/right.

Method:
- use the same `1.0 m` straight line;
- drive forward from the start line to the finish line;
- measure lateral offset from the center line at the finish.

Pass / evidence condition:
- drift is measured and documented;
- if drift is high, the result is used to tune motor trim.

Suggested data file:
- `tests/results/movement_drift_test.csv`

## 3. Turn-In-Place Test

Objective:
- measure turning response for left and right commands.

Method:
- place the robot on a flat surface;
- mark its initial heading;
- command `left` or `right`;
- measure time for `90 deg`, `180 deg`, and optionally `360 deg`;
- repeat for both directions.

Pass / evidence condition:
- robot can turn in both directions;
- approximate `180 deg` turn time is recorded.

Suggested data file:
- `tests/results/movement_turn_test.csv`

## 4. ToF Stop-Distance Test

Objective:
- measure whether the automatic ToF stop behavior stops the robot before an obstacle.

Method:
- set a ToF threshold such as `400 mm`, `300 mm`, or `250 mm`;
- drive toward a flat obstacle;
- record final distance between robot front and obstacle;
- repeat at several PWM values.

Pass / evidence condition:
- the robot stops before contact at moderate PWM values;
- collisions or pressed-against-obstacle cases are recorded honestly.

Suggested data file:
- `tests/results/tof_stop_test.csv`

## 5. ToF Distance Accuracy Test

Objective:
- verify `VL53L0X` distance readings against known distances.

Method:
- place a flat obstacle in front of the sensor at known distances;
- recommended distances: `50`, `100`, `200`, `300`, and `500 mm`;
- open `http://192.168.4.1/tof`;
- record measured distance and status.

Calculation:

```text
error_mm = measured_mm - actual_mm
```

Pass / evidence condition:
- sensor returns valid readings at short and medium distances;
- any unstable or invalid distance ranges are documented.

Suggested data file:
- `tests/results/tof_accuracy_test.csv`

## 6. Power Brownout / Reset Test

Objective:
- verify that motor activity does not reset the controller or disconnect Wi-Fi.

Method:
- keep the UI open;
- run short movement commands at increasing PWM values;
- test `forward`, `backward`, `left`, `right`, and `stop`;
- watch for controller reboot, Wi-Fi disconnect, or camera stream loss.

Pass / evidence condition:
- no reset or Wi-Fi loss during normal movement;
- any failure is recorded with the command and speed value that caused it.

Suggested data file:
- `tests/results/power_brownout_test.csv`

## Recommended Verstapost Wording

The robot was tested using repeatable movement and sensor procedures. HTTP latency was measured automatically, while movement speed, drift, turning, stopping, ToF accuracy, and brownout behavior were recorded manually from physical tests. Remaining issues are treated as calibration and integration work for later milestones.
