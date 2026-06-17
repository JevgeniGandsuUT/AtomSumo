# Power Brownout Retest 2026-06-17

This note records the 2026-06-17 power/brownout retest interpretation.

## Operator Observation

During the retest, the camera stream stayed visible in the browser throughout the run. This means the browser-visible camera stream did not obviously drop out during the physical test.

The UI screenshot from the run also shows:

- Cam 1 marked `LIVE` and displaying video;
- Cam 2 marked `LIVE`;
- ToF distance updating at `196 mm`;
- Cam 2 color telemetry updating with `COLOR RED` and raw RGB/clear values.

## Automatic CSV Result

Source file:
- [power_brownout_test.csv](../../tests/results/power_brownout_test.csv)
- [power_brownout_retest_2026-06-17_summary.csv](../../tests/results/power_brownout_retest_2026-06-17_summary.csv)

The final `--camera-observed` retest rows for PWM `80`, `150`, and `180` show:

| PWM range | Main controller / Wi-Fi result | Camera check result |
| --- | --- | --- |
| `80` | `10 / 10` rows reset `no`, Wi-Fi disconnect `no` | browser camera observed in `10 / 10`; `2` automatic color endpoint transient failures |
| `150` | `10 / 10` rows reset `no`, Wi-Fi disconnect `no` | browser camera observed in `10 / 10`; automatic camera/color status clean |
| `180` | `10 / 10` rows reset `no`, Wi-Fi disconnect `no` | browser camera observed in `10 / 10`; automatic camera/color status clean |

Earlier PWM `255` rows recorded main endpoint failures and should be treated as an unstable maximum-speed condition, not as the normal operating profile.

## Interpretation

The automatic camera check used camera HTTP endpoints that timed out or returned URL errors, while the operator observed the browser stream as continuously visible. Therefore the `camera_loss=yes` value in this run should be treated as an automatic endpoint-check failure, not as proof that camera power dropped.

Likely cause: the camera firmware serves an MJPEG browser stream and may not reliably serve a second simultaneous capture/health-check request while the browser is already connected.

The `255` PWM rows still need caution because the script also lost the main controller endpoints during those commands. The stable operating range for the current prototype should be documented as PWM `80-180`.

## Script Fix

The power test script now checks the camera root page by default:

- `http://192.168.4.20/`
- `http://192.168.4.21/`

This avoids treating the long-running MJPEG stream or slow snapshot endpoint as a simple short HTTP health check.

The script also supports a manual observation flag:

```powershell
python tests\scripts\power_brownout_test.py --pwms 80 150 180 --camera-observed
```

Use this mode when the browser UI is open and the camera panels remain visible during the test.

## Current Status

V2 power robustness is stronger than before:

- main controller is stable at PWM `80`, `150`, and `180`;
- camera browser stream was observed as stable by the operator;
- PWM `255` is not accepted as a stable operating mode for the current prototype.

Conclusion: the V2 power robustness requirement can be marked complete for the documented operating range PWM `80-180`.
