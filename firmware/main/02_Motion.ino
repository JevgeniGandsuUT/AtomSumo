void setupMotorPwm() {
  ledcAttach(PWMA, MOTOR_PWM_FREQ_HZ, MOTOR_PWM_RESOLUTION_BITS);
  ledcAttach(PWMB, MOTOR_PWM_FREQ_HZ, MOTOR_PWM_RESOLUTION_BITS);
}

void setMotorA(int direction, uint8_t pwm) {
  if (direction > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, pwm);
  } else if (direction < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, pwm);
  } else {
    analogWrite(PWMA, 0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }
}

void setMotorB(int direction, uint8_t pwm) {
  if (direction > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, pwm);
  } else if (direction < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, pwm);
  } else {
    analogWrite(PWMB, 0);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }
}

uint8_t applyTrim(uint8_t baseSpeed, uint8_t trim) {
  return static_cast<uint8_t>((static_cast<uint16_t>(baseSpeed) * trim) / 255);
}

uint8_t maybeInvertPwm(uint8_t pwm, bool inverted) {
  return inverted ? static_cast<uint8_t>(255 - pwm) : pwm;
}

uint8_t applyArcInnerSpeed(uint8_t baseSpeed) {
  uint8_t pwm = applyTrim(baseSpeed, arcTurnInnerTrim);
  if (arcTurnInnerTrim > 0 && pwm > 0 && pwm < ARC_TURN_MIN_INNER_PWM) {
    return min(baseSpeed, ARC_TURN_MIN_INNER_PWM);
  }
  return pwm;
}

void setAtomLed(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t rr = static_cast<uint8_t>((static_cast<uint16_t>(r) * LED_BRIGHTNESS) / 255);
  uint8_t gg = static_cast<uint8_t>((static_cast<uint16_t>(g) * LED_BRIGHTNESS) / 255);
  uint8_t bb = static_cast<uint8_t>((static_cast<uint16_t>(b) * LED_BRIGHTNESS) / 255);
  rgbLedWrite(ATOM_LED_PIN, rr, gg, bb);
}

void applyDirectionLed(MotionDirection dir) {
  switch (dir) {
    case DIR_FORWARD:
      standbyBlinkOn = false;
      setAtomLed(0, 255, 0);      // Green
      break;
    case DIR_BACKWARD:
      standbyBlinkOn = false;
      setAtomLed(255, 0, 0);      // Red
      break;
    case DIR_LEFT:
      standbyBlinkOn = false;
      setAtomLed(160, 0, 255);    // Violet
      break;
    case DIR_RIGHT:
      standbyBlinkOn = false;
      setAtomLed(255, 120, 0);    // Orange
      break;
    case DIR_FORWARD_LEFT:
    case DIR_FORWARD_RIGHT:
      standbyBlinkOn = false;
      setAtomLed(0, 180, 255);    // Cyan
      break;
    case DIR_BACKWARD_LEFT:
    case DIR_BACKWARD_RIGHT:
      standbyBlinkOn = false;
      setAtomLed(255, 0, 120);    // Pink
      break;
    case DIR_STOP:
    default:
      standbyBlinkOn = true;
      lastStandbyBlinkMs = millis();
      setAtomLed(0, 0, 255);      // Blue blink starts "ON"
      break;
  }
}

void updateStandbyLedBlink() {
  if (!standbyBlinkOn || currentDirection != DIR_STOP) {
    return;
  }

  unsigned long now = millis();
  if (now - lastStandbyBlinkMs < STANDBY_BLINK_INTERVAL_MS) {
    return;
  }

  lastStandbyBlinkMs = now;
  static bool phaseOn = true;
  phaseOn = !phaseOn;
  if (phaseOn) {
    setAtomLed(0, 0, 255);
  } else {
    setAtomLed(0, 0, 0);
  }
}

void updateTofReading() {
  if (!tofReady) {
    return;
  }

  unsigned long now = millis();
  if (now - lastTofReadMs < TOF_READ_INTERVAL_MS) {
    return;
  }
  lastTofReadMs = now;

  VL53L0X_RangingMeasurementData_t measure;
  tofSensor.rangingTest(&measure, false);
  lastTofStatus = measure.RangeStatus;
  lastTofValid = (measure.RangeStatus != 4);
  if (lastTofValid) {
    lastTofMm = measure.RangeMilliMeter;
  }
}

uint8_t applyStartBoost(uint8_t pwm) {
  if (pwm == 0) {
    return 0;
  }
  return static_cast<uint8_t>(min(255, static_cast<int>(pwm) + START_BOOST_EXTRA_PWM));
}

void moveWithBoost(int dirA, uint8_t pwmA, int dirB, uint8_t pwmB, bool useBoost) {
  if (useBoost && (pwmA > 0 || pwmB > 0)) {
    setMotorA(dirA, applyStartBoost(pwmA));
    setMotorB(dirB, applyStartBoost(pwmB));
    delay(START_BOOST_MS);
  }

  setMotorA(dirA, pwmA);
  setMotorB(dirB, pwmB);
}

void applyDirection(MotionDirection dir) {
  MotionDirection previousDirection = currentDirection;
  currentDirection = dir;
  bool fromStop = (previousDirection == DIR_STOP && dir != DIR_STOP);

  if (previousDirection != DIR_STOP && dir != DIR_STOP && previousDirection != dir) {
    setMotorA(0, 0);
    setMotorB(0, 0);
    delay(40);
  }

  switch (dir) {
    case DIR_FORWARD:
      moveWithBoost(
        1,
        maybeInvertPwm(applyTrim(motorSpeed, MOTOR_A_FORWARD_TRIM), MOTOR_A_FORWARD_PWM_INVERTED),
        1,
        applyTrim(motorSpeed, MOTOR_B_FORWARD_TRIM),
        fromStop
      );
      break;
    case DIR_BACKWARD:
      moveWithBoost(
        -1,
        maybeInvertPwm(applyTrim(motorSpeed, MOTOR_A_BACKWARD_TRIM), MOTOR_A_BACKWARD_PWM_INVERTED),
        -1,
        applyTrim(motorSpeed, MOTOR_B_BACKWARD_TRIM),
        fromStop
      );
      break;
    case DIR_LEFT:
      moveWithBoost(
        -1,
        maybeInvertPwm(motorSpeed, MOTOR_A_LEFT_TURN_PWM_INVERTED),
        1,
        motorSpeed,
        fromStop
      );
      break;
    case DIR_RIGHT:
      moveWithBoost(
        1,
        maybeInvertPwm(motorSpeed, MOTOR_A_RIGHT_TURN_PWM_INVERTED),
        -1,
        motorSpeed,
        fromStop
      );
      break;
    case DIR_FORWARD_LEFT:
      moveWithBoost(
        1,
        maybeInvertPwm(applyArcInnerSpeed(motorSpeed), MOTOR_A_FORWARD_PWM_INVERTED),
        1,
        applyTrim(motorSpeed, MOTOR_B_FORWARD_TRIM),
        fromStop
      );
      break;
    case DIR_FORWARD_RIGHT:
      moveWithBoost(
        1,
        maybeInvertPwm(applyTrim(motorSpeed, MOTOR_A_FORWARD_TRIM), MOTOR_A_FORWARD_PWM_INVERTED),
        1,
        applyArcInnerSpeed(motorSpeed),
        fromStop
      );
      break;
    case DIR_BACKWARD_LEFT:
      moveWithBoost(
        -1,
        maybeInvertPwm(applyArcInnerSpeed(motorSpeed), MOTOR_A_BACKWARD_PWM_INVERTED),
        -1,
        applyTrim(motorSpeed, MOTOR_B_BACKWARD_TRIM),
        fromStop
      );
      break;
    case DIR_BACKWARD_RIGHT:
      moveWithBoost(
        -1,
        maybeInvertPwm(applyTrim(motorSpeed, MOTOR_A_BACKWARD_TRIM), MOTOR_A_BACKWARD_PWM_INVERTED),
        -1,
        applyArcInnerSpeed(motorSpeed),
        fromStop
      );
      break;
    case DIR_STOP:
    default:
      setMotorA(0, 0);
      setMotorB(0, 0);
      break;
  }

  applyDirectionLed(dir);
}

