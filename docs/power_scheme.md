# Power Scheme: 2S 18650 + N20 6V motors

## Fixed power architecture

The robot power system is fixed as follows:

```text
USB-C PD Trigger 12V
    -> 1-2S Li-Ion charger module
    -> 2S 18650 battery pack
    -> 2S BMS / protection module
    -> master power switch
        -> Buck #1 set to 5.0V
            -> M5 ATOM main controller
            -> 2 x M5 AtomS3R-CAM
            -> VL53L0X
            -> low-power logic and telemetry devices
        -> Buck #2 set to 6.0V
            -> TB6612FNG VM
            -> 2 x N20 6V drive motors
```

## Design decisions

- Battery type: `2S 18650`
- Nominal battery voltage: `7.2-7.4V`
- Fully charged battery voltage: `8.4V`
- Logic rail: `5.0V`
- Motor rail: `6.0V`
- Drive motors: `2 x N20 6V`
- Wheel layout: `4 wheels total`, with `2` driven wheels and `2` passive free-rolling support wheels

## Wiring rules

- All modules shall share common `GND`.
- Logic and motor rails shall remain separately regulated.
- `TB6612FNG` motor supply shall be powered from the `6.0V` motor rail.
- A `1000uF / 16V` bulk capacitor shall be placed close to the motor driver supply.
- The `M5 ATOM`, both `M5 AtomS3R-CAM` boards, and `VL53L0X` shall be powered from the `5.0V` logic rail.

## Important constraints

- Raw `8.4V` from the fully charged `2S` pack shall not be applied directly to the `6V` N20 motors.
- The ordered `2S 3A` BMS must still be validated under motor startup and stall conditions.
- The CAD battery packaging must be revised from the current legacy powerbank bay to a `2x18650` holder or custom tray.

## Evidence in repository

- Draw.io block diagram: [../drawio/power_tree.drawio](../drawio/power_tree.drawio)
- Main controller firmware: [sketch_mar21a/sketch_mar21a.ino](../sketch_mar21a/sketch_mar21a.ino)
- Camera firmware: [cam/sketch_jan17a.ino](../cam/sketch_jan17a.ino)
- BOM: [BOM.csv](./BOM.csv)
- System specification: [V1_specification.md](./V1_specification.md)
