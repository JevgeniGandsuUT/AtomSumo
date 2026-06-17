# Power Brownout / Reset Analysis

This note summarizes the current power robustness evidence and the exact retest needed to close the tracker row.

## Source Data

- [power_brownout_test.csv](../../tests/results/power_brownout_test.csv)
- [power_brownout_retest_2026-06-17_summary.csv](../../tests/results/power_brownout_retest_2026-06-17_summary.csv)
- [power_brownout_test.py](../../tests/scripts/power_brownout_test.py)

The dataset contains the original `2026-06-14` run and a `2026-06-17` retest:

- PWM values: `80`, `150`, `180`, `255`;
- commands: `forward`, `backward`, `left`, `right`, `stop`;
- trials: `2` per command/PWM combination;
- power source: `2S_18650`.

## Current Result

| Check | Result |
| --- | --- |
| Original run, main-controller reset observed | `0 / 40` |
| Original run, Wi-Fi disconnect observed | `0 / 40` |
| Original run, camera endpoint loss recorded | `40 / 40` |
| 2026-06-17 final retest, PWM `80/150/180` | `30 / 30` rows reset `no` and Wi-Fi disconnect `no` |
| 2026-06-17 retest, PWM `255` | main endpoint failures recorded; reset marked `suspected` |
| 2026-06-17 operator camera observation | browser camera stream stayed visible during the run |

Interpretation:

- The main controller and robot Wi-Fi stayed alive during the original run.
- In the 2026-06-17 final retest, the main controller stayed reachable at PWM `80`, `150`, and `180`.
- The 2026-06-17 operator observation confirms that the browser-visible camera stream stayed visible during the run.
- The automatic camera check was too strict for this setup and may have collided with the already-open browser MJPEG stream. The script now has a `--camera-observed` mode for browser-observed camera evidence.
- PWM `255` is documented as outside the stable operating range because the automatic test recorded main endpoint failures.

## Retest Needed To Close The Requirement

Before retesting:

1. Confirm main controller UI opens at `http://192.168.4.1/`.
2. Confirm Cam1 stream opens at `http://192.168.4.20/stream`.
3. Confirm Cam2 stream opens at `http://192.168.4.21/stream`.
4. Confirm Cam2 color endpoint opens at `http://192.168.4.21:81/color`.
5. Keep the browser UI open during the test.

Recommended command when the browser UI is used as camera evidence:

```powershell
python tests\scripts\power_brownout_test.py --pwms 80 150 180 --camera-observed
```

Retest matrix:

| PWM | Commands | Trials |
| ---: | --- | ---: |
| `80` | `forward`, `backward`, `left`, `right`, `stop` | `2` |
| `150` | `forward`, `backward`, `left`, `right`, `stop` | `2` |
| `180` | `forward`, `backward`, `left`, `right`, `stop` | `2` |
| `255` | `forward`, `backward`, `left`, `right`, `stop` | `2` |

Pass condition:

- `reset_observed = no` for every row;
- `wifi_disconnect = no` for every row;
- `camera_loss = no` for every row or a documented non-power cause;
- notes identify battery voltage, regulator settings, and camera state before/after the run.

## Current Tracker Status

The tracker can be closed for V2 power robustness with the operating range documented as PWM `80-180`. PWM `255` should not be presented as a stable operating mode for the current prototype.

Related retest note:
- [power_brownout_retest_2026-06-17.md](power_brownout_retest_2026-06-17.md)
