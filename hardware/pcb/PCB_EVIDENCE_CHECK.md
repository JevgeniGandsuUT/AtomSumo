# PCB Evidence Check

This checklist records the current PCB evidence package for the `ATOM Sumo` interconnect board.

## Available Evidence

| Evidence item | File | Status |
| --- | --- | --- |
| Latest schematic screenshot | [Schematic_ATOM-Sumo_2026-06-13.png](Schematic_ATOM-Sumo_2026-06-13.png) | available |
| Latest schematic source export | [SCH_ATOM-Sumo_2026-06-13.json](SCH_ATOM-Sumo_2026-06-13.json) | available |
| Latest PCB layout screenshot | [PCB_PCB_ATOM_SUMO_2026-06-13.png](PCB_PCB_ATOM_SUMO_2026-06-13.png) | available |
| Latest PCB layout source export | [PCB_PCB_ATOM_SUMO_2026-06-13.json](PCB_PCB_ATOM_SUMO_2026-06-13.json) | available |
| Additional PCB screenshot | [PCB_extra_screenshot_2026-06-13_235021.png](PCB_extra_screenshot_2026-06-13_235021.png) | available |
| PCB 3D view | [3d.png](3d.png) | available |
| Latest Gerber archive | [Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-06-17.zip](Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-06-17.zip) | current manufacturing export; contains top/bottom copper, silkscreen, paste mask, solder mask, board outline, and PTH/NPTH drill files |
| Older Gerber archive | [Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08.zip](Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08.zip) | previous export, kept for traceability |
| Extra source archive | [Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08-2.zip](Gerber_ATOM-Sumo_PCB_ATOM_SUMO_2026-04-08-2.zip) | available; contains nested EasyEDA schematic/PCB JSON export |

## Electrical Intent Covered By The Board

The PCB package supports the project wiring concept:

- M5 ATOM main controller breakout;
- TB6612FNG motor-driver wiring;
- left motor pair and right motor pair wiring;
- logic/sensor connectors;
- shared ground and separated motor/logic rails according to the documented power scheme.

Related files:

- [BOM.csv](../../docs/specification/BOM.csv)
- [power_scheme.md](../../docs/specification/power_scheme.md)
- [Schematic_ATOM-Sumo_2026-06-13.png](Schematic_ATOM-Sumo_2026-06-13.png)

## Remaining Production Evidence

The current repository has schematic/layout screenshots, source JSON, 3D PCB evidence, and the latest `2026-06-17` Gerber archive with manufacturing layers and drill files. This is sufficient for the Verstapost `PCB evidence` requirement.

For stronger production-readiness evidence, add one or both of these optional files:

- `hardware/pcb/DRC_pass_YYYY-MM-DD.png`
- `hardware/pcb/Gerber_export_confirmation_YYYY-MM-DD.png`

Recommended capture steps:

1. Open the latest PCB project in EasyEDA.
2. Run Design Rule Check.
3. Capture the result window showing no blocking errors, or record the exact warnings.
4. Export Gerber/manufacturing files.
5. Capture the export confirmation or generated archive screen.

## Current Status

The PCB package is complete enough as design/source/current-Gerber evidence for Verstapost 1-3. A dedicated DRC pass screenshot is still useful for final production confidence, but V2 `PCB evidence` can be marked complete.
