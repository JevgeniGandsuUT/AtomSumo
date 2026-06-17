# VL53L0X ToF Calibration Note

This note explains the current `VL53L0X` distance measurements and how they should be interpreted for the Verstapost tracker.

## Source Data

Measured files:

- [tof_accuracy_test.csv](../../tests/results/tof_accuracy_test.csv)
- [tof_stop_test.csv](../../tests/results/tof_stop_test.csv)

The accuracy test contains `15` valid readings from `2026-06-14`, with `3` trials at each reference distance: `50`, `100`, `200`, `300`, and `500 mm`.

## Observed Accuracy

| Actual distance | Mean measured distance | Mean error | Interpretation |
| ---: | ---: | ---: | --- |
| `50 mm` | `113.7 mm` | `+63.7 mm` | strong positive offset |
| `100 mm` | `170.3 mm` | `+70.3 mm` | strong positive offset |
| `200 mm` | `271.3 mm` | `+71.3 mm` | strong positive offset |
| `300 mm` | `331.0 mm` | `+31.0 mm` | smaller positive offset |
| `500 mm` | `362.3 mm` | `-137.7 mm` | unstable / outside reliable current setup |

The useful current range for control decisions is therefore the short and medium range up to about `300 mm`. At `500 mm`, the readings are inconsistent enough that they should not be used for precise distance decisions without retesting.

## Calibration Model

Using only the `50-300 mm` range, a simple linear correction gives:

```text
measured_mm = 0.8747 * actual_mm + 79.45
actual_mm   = 1.1202 * measured_mm - 85.72
```

After this correction, the current dataset has:

- mean absolute corrected error: about `11.7 mm`;
- maximum absolute corrected error: about `23.5 mm`;
- valid only for the current sensor mounting and the `50-300 mm` measured range.

## Control Interpretation

For the current robot, the ToF sensor is already useful for obstacle stop / near-wall behavior, because the stop behavior test confirms that the robot stops at moderate speeds:

- `PWM=80`, threshold `400 mm`: final distance about `26.5-27.7 cm`;
- `PWM=150`, threshold `400 mm`: final distance about `11.6-14.9 cm`;
- `PWM=80`, threshold `250-300 mm`: final distance about `10.2-18.8 cm`.

At `PWM=255`, one test reached collision/pressed-against-obstacle state, so high-speed stopping must still be tuned.

## Status Decision

The ToF integration requirement can be treated as implemented and documented:

- `/tof` endpoint exists in firmware;
- UI displays live ToF data;
- stop-distance behavior is measured;
- static offset is measured and explained;
- a practical correction model exists for the reliable short/medium range.

Remaining work is not basic integration. It is refinement:

- retest after final sensor mounting;
- decide whether to apply correction in firmware or only in analysis;
- avoid using `500 mm` readings as precise control data until retested.
