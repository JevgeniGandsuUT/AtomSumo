# Flashing Guide

This guide documents the current firmware flashing workflow.

## Arduino CLI

Known local Arduino CLI path on the development machine:

```powershell
$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe
```

Check version:

```powershell
& "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" version
```

Verified version during repository review:

```text
arduino-cli 1.2.0
```

## Main Controller Firmware

Folder:
- [main](../../firmware/main)

Board/FQBN:
- `m5stack:esp32:m5stack_atom`

Compile:

```powershell
& "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" compile --fqbn m5stack:esp32:m5stack_atom firmware\main
```

Upload example:

```powershell
& "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" upload -p COM5 --fqbn m5stack:esp32:m5stack_atom firmware\main
```

Notes:
- main controller was previously flashed on `COM5`;
- COM port may change after reconnecting USB.

## Camera Firmware

Folder:
- [firmware/camera/sketch_jan17a](../../firmware/camera/sketch_jan17a)

Board/FQBN:
- `m5stack:esp32:m5stack_atoms3r`

The same camera firmware is used for both cameras. Select the slot at compile time.

### Cam 1

Compile:

```powershell
& "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" compile --fqbn m5stack:esp32:m5stack_atoms3r --build-property build.extra_flags="-DCAMERA_SLOT=1" firmware\camera\sketch_jan17a
```

Expected IP after boot:

```text
http://192.168.4.20/stream
```

### Cam 2

Compile:

```powershell
& "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" compile --fqbn m5stack:esp32:m5stack_atoms3r --build-property build.extra_flags="-DCAMERA_SLOT=2" firmware\camera\sketch_jan17a
```

Upload example:

```powershell
& "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" upload -p COM6 --fqbn m5stack:esp32:m5stack_atoms3r --build-property build.extra_flags="-DCAMERA_SLOT=2" firmware\camera\sketch_jan17a
```

Expected endpoints after boot:

```text
http://192.168.4.21/stream
http://192.168.4.21:81/color
```

Notes:
- Cam 2 was previously flashed on `COM6`;
- Cam 2 contains the `TCS34725` color telemetry path.

## Post-Flash Check

1. Power the main controller.
2. Connect to Wi-Fi SSID `Robot-Control`.
3. Open `http://192.168.4.1`.
4. Confirm the UI loads.
5. Confirm `/tof` returns JSON.
6. Confirm Cam 1 stream.
7. Confirm Cam 2 stream.
8. Confirm Cam 2 color JSON on port `81`.

## Measurement Launchers

After flashing and connecting the laptop to `Robot-Control`, measurements can be started from PowerShell launchers in [tools](../../tools):

- [RUN_MEASUREMENTS.ps1](../../tools/RUN_MEASUREMENTS.ps1) - menu for brownout and ToF accuracy tests.
- [RUN_POWER_BROWNOUT_TEST.ps1](../../tools/RUN_POWER_BROWNOUT_TEST.ps1) - direct brownout test launcher.
