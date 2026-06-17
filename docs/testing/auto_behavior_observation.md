# Auto Behavior Observation

Source data:
- [auto_behavior_operator_observation.csv](../../tests/results/auto_behavior_operator_observation.csv)

## Observation

During the V3 auto-behavior check, the operator observed the robot turning/avoiding when an obstacle was approximately `200 mm` in front of the ToF sensor.

The robot battery depleted before a full five-trial CSV logger run could be completed. Therefore this is recorded as operator-observed evidence, not as a full measured reliability dataset.

## Current Status

The implementation and logger exist in the UI:

- [main web UI](../../firmware/main/01_WebUi.ino)
- [auto behavior logger](auto_behavior_logger.md)

The remaining work to fully close the reliability claim is to recharge the robot and collect a logger CSV with at least five marked pass trials.
