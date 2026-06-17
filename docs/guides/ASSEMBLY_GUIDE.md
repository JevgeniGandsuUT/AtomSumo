# Assembly Guide

This is the current assembly guide for the `ATOM Sumo` prototype.

Status:
- repository assembly documentation is present as a technical checklist;
- final photo-based assembly documentation is still required for a true production-ready release.

## Required Reference Files

- [BOM.csv](../specification/BOM.csv)
- [power_scheme.md](../specification/power_scheme.md)
- [hardware/pcb/README.md](../../hardware/pcb/README.md)
- [hardware/cad/real/README.md](../../hardware/cad/real/README.md)
- [hardware/cad/ver1](../../hardware/cad/ver1)
- [hardware/cad/ver3](../../hardware/cad/ver3)

## Main Hardware Blocks

- `M5 ATOM` main controller
- `2 x M5 AtomS3R-CAM`
- `TB6612FNG` motor driver
- `4 x N20 6V` DC gearmotors
- `2S 18650` battery pack
- `2S BMS`
- `5.0V` buck converter for logic
- `6.0V` buck converter for motors
- `VL53L0X` front distance sensor
- `TCS34725` color sensor on Cam 2 Grove I2C
- custom PCB / wiring harness
- printed chassis/enclosure parts

## Mechanical Assembly Order

1. Inspect the real Fusion CAD package:
   - [Robot.f3d](../../hardware/cad/real/Robot.f3d)
   - [Assembly new.f3z](<../../hardware/cad/real/Assembly new.f3z>)
   - [Shell.f3d](../../hardware/cad/real/Shell.f3d)
   - [Topcover.f3d](../../hardware/cad/real/Topcover.f3d)
   - [Battery pack.f3d](<../../hardware/cad/real/Battery pack.f3d>)
2. Print or inspect the chassis/enclosure parts.
3. Install the four N20 motors.
4. Install all four driven wheels.
5. Mount the `2S 18650` battery holder or pack.
6. Mount the main controller and hardware/pcb/body holder.
7. Mount the front `VL53L0X` sensor so it has a clear forward view.
8. Mount Cam 1 as the front/operator camera.
9. Mount Cam 2 downward.
10. Mount the `TCS34725` sensor on Cam 2 Grove I2C so it sees the floor.
11. Verify that no wires touch the wheels or motor shafts.

## Electrical Assembly Order

1. Build the power chain according to [power_scheme.md](../specification/power_scheme.md).
2. Confirm common `GND` between:
   - main controller;
   - motor driver;
   - buck converters;
   - sensors;
   - cameras.
3. Set the logic buck to `5.0V` before connecting controllers/cameras.
4. Set the motor buck to `6.0V` before connecting `TB6612FNG VM`.
5. Add the `1000uF / 16V` capacitor near the motor rail.
6. Connect the motor driver to the main controller pins used by [firmware/main/main.ino](../../firmware/main/main.ino).
7. Wire the left-side motor pair in parallel to TB6612 channel A.
8. Wire the right-side motor pair in parallel to TB6612 channel B.
9. Before floor testing, validate current draw because each TB6612 channel now drives two motors.
10. Connect `VL53L0X` to the configured I2C pins in [firmware/main/main.ino](../../firmware/main/main.ino).
11. Connect `TCS34725` to Cam 2 Grove:
   - SDA `G2`
   - SCL `G1`
   - `5V`
   - `GND`

## Bring-Up Checklist

1. Power only the logic rail and confirm `Robot-Control` Wi-Fi appears.
2. Open `http://192.168.4.1`.
3. Confirm `/tof` responds.
4. Power Cam 1 and confirm `http://192.168.4.20/stream`.
5. Power Cam 2 and confirm `http://192.168.4.21/stream`.
6. Confirm Cam 2 color endpoint: `http://192.168.4.21:81/color`.
7. Test motors on a raised stand before placing the robot on the floor.
8. Test `stop` before any high-speed movement.

## Photos Still Required

For a stronger Verstapost 5 package, add photos of:

- empty printed chassis;
- motors installed;
- battery pack installed;
- hardware/pcb/controller installed;
- front ToF placement;
- Cam 1 placement;
- Cam 2 plus TCS34725 placement;
- wiring before closing the shell;
- complete assembled robot.
