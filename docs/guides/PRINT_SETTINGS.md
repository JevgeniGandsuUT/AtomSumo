# Print Settings

This document records the current 3D-printing guidance for the robot chassis/enclosure.

Status:
- CAD evidence is present;
- final material-specific print validation is still open.

## CAD Sources

Baseline chassis:
- [hardware/cad/ver1](../../hardware/cad/ver1)

Functional enclosure/body:
- [hardware/cad/ver3](../../hardware/cad/ver3)

Real Fusion 360 package:
- [hardware/cad/real/README.md](../../hardware/cad/real/README.md)

## Recommended Starting Settings

These are starting values for prototype prints, not yet final production settings.

| Parameter | Starting value |
| --- | --- |
| Material | PLA or PETG |
| Layer height | `0.20 mm` |
| Wall/perimeter count | `3` |
| Top/bottom layers | `4` |
| Infill | `20-35%` |
| Infill pattern | grid, gyroid, or cubic |
| Nozzle | `0.4 mm` |
| Supports | enable only where camera/sensor openings or overhangs require it |
| Bed adhesion | brim if corners lift |

## Material Notes

PLA:
- easier to print;
- good for first fit checks;
- weaker near heat and impacts.

PETG:
- better toughness;
- better for functional enclosure tests;
- may need slower print speed and more stringing cleanup.

## Required Validation Before Production Claim

1. Print the final body parts.
2. Check that motors and wheels do not rub.
3. Check that camera views are not blocked.
4. Check that `VL53L0X` has a clear front window.
5. Check that the downward `TCS34725` sees the floor reliably.
6. Check that the battery can be removed and reinstalled.
7. Run a `5 min` motor heat test.
8. Run `3-5` controlled impact checks.
9. Run repeated `90 s` match-like endurance tests.

Suggested result files:

- `tests/results/enclosure_heat_test.csv`
- `tests/results/enclosure_impact_test.csv`
- `tests/results/battery_service_test.csv`
- `tests/results/match_endurance_test.csv`

## Photos Still Needed

For final documentation, add photos of:

- printed parts before assembly;
- support removal / cleanup;
- electronics installed;
- battery access;
- final assembled robot from front, back, left, right, and top.
