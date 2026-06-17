# Color Repeatability Summary

Source data:
- [color_repeatability_test.csv](../../tests/results/color_repeatability_test.csv)

## Setup

Cam 2 uses a `TCS34725` color sensor connected on Grove I2C. The endpoint returns:

```text
GET http://192.168.4.21:81/color
```

with `name`, `clear`, `red`, `green`, and `blue` values. The main UI displays the same telemetry in the color panel.

## Measurements

| Surface | Trials | Clear range | Red range | Green range | Blue range | Result |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| red | `3` | `720-722` | `456-459` | `168` | `117` | pass |
| light | `3` | `797-798` | `367-370` | `266-269` | `165` | pass |
| dark | `3` | `86-87` | `44` | `27` | `17` | pass after threshold fix |

## Threshold Fix

The first dark-surface measurement showed that the old `dark` threshold was too low:

```cpp
if (clear < 35) return "dark";
```

The dark surface measured `clear=86-87`, so it was misclassified. The classifier was updated to:

```cpp
if (clear < 150) return "dark";
```

The light/white rule was also relaxed for high-clear surfaces with moderate channel spread:

```cpp
if (maxChannel < 80 || (clear > 250 && (maxChannel - minChannel) < (clear / 3))) return "white";
```

## Interpretation

The color telemetry is repeatable across red, light, and dark surfaces. After flashing the threshold fix to Cam 2, the UI confirmed `COLOR DARK` on the dark surface.

The project can claim working color/boundary telemetry from Cam 2. Closed-loop robot retreat/avoid behavior based on color is not claimed here; that remains a later behavior layer.
