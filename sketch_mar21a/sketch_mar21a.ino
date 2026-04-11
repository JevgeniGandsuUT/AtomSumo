#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#define PWMA 22
#define AIN1 19
#define AIN2 23

#define PWMB 25
#define BIN1 21
#define BIN2 33
#define ATOM_LED_PIN 27

const char *AP_SSID = "Robot-Control";
const char *AP_PASSWORD = "robot123";

WebServer server(80);
DNSServer dnsServer;
const uint16_t DNS_PORT = 53;

enum MotionDirection {
  DIR_STOP,
  DIR_FORWARD,
  DIR_BACKWARD,
  DIR_LEFT,
  DIR_RIGHT
};

MotionDirection currentDirection = DIR_STOP;
uint8_t motorSpeed = 180;
bool standbyBlinkOn = false;
unsigned long lastStandbyBlinkMs = 0;

// Forward on many small robots can overload one side at max PWM.
// Keep A slightly prioritized and give both motors a short start boost.
const uint8_t MOTOR_A_FORWARD_TRIM = 255;
const uint8_t MOTOR_B_FORWARD_TRIM = 255;
const uint8_t MOTOR_A_BACKWARD_TRIM = 255;
const uint8_t MOTOR_B_BACKWARD_TRIM = 255;
const uint8_t START_BOOST_PWM = 255;
const uint16_t START_BOOST_MS = 120;
const uint16_t STANDBY_BLINK_INTERVAL_MS = 450;
const uint8_t LED_BRIGHTNESS = 110;
const bool MOTOR_A_FORWARD_PWM_INVERTED = false;
const bool MOTOR_A_BACKWARD_PWM_INVERTED = false;
const bool MOTOR_A_LEFT_TURN_PWM_INVERTED = false;
const bool MOTOR_A_RIGHT_TURN_PWM_INVERTED = false;

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Robot Control</title>
  <style>
    :root {
      --bg-a: #0f172a;
      --bg-b: #1d4e89;
      --panel: #ffffff;
      --ink: #0b1f33;
      --btn: #0ea5e9;
      --btn-active: #0369a1;
      --btn-stop: #ef4444;
      --btn-stop-active: #b91c1c;
    }

    * {
      box-sizing: border-box;
      -webkit-tap-highlight-color: transparent;
      font-family: "Segoe UI", "Trebuchet MS", sans-serif;
    }

    body {
      margin: 0;
      min-height: 100vh;
      display: grid;
      place-items: center;
      background: radial-gradient(circle at 20% 20%, #3b82f6 0%, var(--bg-b) 35%, var(--bg-a) 100%);
      color: var(--ink);
      padding: 16px;
    }

    .card {
      width: min(480px, 100%);
      background: var(--panel);
      border-radius: 20px;
      box-shadow: 0 16px 40px rgba(2, 6, 23, 0.35);
      padding: 18px;
    }

    h1 {
      margin: 0 0 6px;
      font-size: 1.4rem;
      text-align: center;
    }

    .hint {
      margin: 0 0 14px;
      text-align: center;
      color: #334155;
      font-size: 0.95rem;
    }

    .cams {
      display: grid;
      grid-template-columns: 1fr;
      gap: 10px;
      margin-bottom: 14px;
    }

    .cam-card {
      background: #0b1f33;
      border-radius: 14px;
      padding: 8px;
      color: #cbd5e1;
    }

    .cam-head {
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 0.8rem;
      font-weight: 700;
      margin: 0 3px 6px;
      letter-spacing: 0.03em;
    }

    .cam-state {
      font-size: 0.75rem;
      opacity: 0.9;
      text-transform: uppercase;
    }

    .cam-card img {
      width: 100%;
      aspect-ratio: 4 / 3;
      object-fit: cover;
      border-radius: 10px;
      background: #020617;
      display: block;
    }

    .pad {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-areas:
        ". up ."
        "left stop right"
        ". down .";
      gap: 12px;
      margin-bottom: 16px;
    }

    button,
    .ctrl-btn {
      border: none;
      border-radius: 16px;
      min-height: 82px;
      font-size: 1.05rem;
      font-weight: 700;
      color: #fff;
      background: var(--btn);
      box-shadow: 0 8px 18px rgba(14, 165, 233, 0.35);
      transition: transform 0.08s ease, background 0.08s ease;
      touch-action: manipulation;
      user-select: none;
      display: flex;
      align-items: center;
      justify-content: center;
      text-decoration: none;
      text-align: center;
    }

    .pad button {
      touch-action: none;
    }

    button:active,
    button.active,
    .ctrl-btn:active {
      transform: scale(0.97);
      background: var(--btn-active);
    }

    #up { grid-area: up; }
    #left { grid-area: left; }
    #right { grid-area: right; }
    #down { grid-area: down; }
    #stop {
      grid-area: stop;
      background: var(--btn-stop);
      box-shadow: 0 8px 18px rgba(239, 68, 68, 0.35);
    }

    #stop:active,
    #stop.active {
      background: var(--btn-stop-active);
    }

    .speed-box {
      background: #f1f5f9;
      border-radius: 14px;
      padding: 12px;
    }

    .speed-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 8px;
      font-weight: 700;
    }

    input[type="range"] {
      width: 100%;
      accent-color: #0284c7;
    }

    .footer {
      margin-top: 12px;
      text-align: center;
      color: #64748b;
      font-size: 0.85rem;
    }

    @media (min-width: 760px) {
      .card {
        width: min(860px, 100%);
      }

      .cams {
        grid-template-columns: 1fr 1fr;
      }
    }
  </style>
</head>
<body>
  <main class="card">
    <h1>Robot Control</h1>
    <section class="cams">
      <article class="cam-card">
        <div class="cam-head">
          <span>CAM 1</span>
          <span class="cam-state">live</span>
        </div>
        <img src="http://192.168.4.20/stream" alt="Camera 1 stream" />
      </article>
      <article class="cam-card">
        <div class="cam-head">
          <span>CAM 2</span>
          <span class="cam-state">live</span>
        </div>
        <img src="http://192.168.4.21/stream" alt="Camera 2 stream" />
      </article>
    </section>
    <p class="hint">Hold a direction button to move. Release to stop.</p>

    <section class="pad">
      <button type="button" id="up">FORWARD</button>
      <button type="button" id="left">LEFT</button>
      <button type="button" id="stop">STOP</button>
      <button type="button" id="right">RIGHT</button>
      <button type="button" id="down">BACK</button>
    </section>

    <section class="speed-box">
      <div class="speed-row">
        <span>Speed</span>
        <span><strong id="speedValue">180</strong> / 255</span>
      </div>
      <input id="speed" name="value" type="range" min="0" max="255" value="180" />
    </section>

    <p class="footer">AP: Robot-Control (192.168.4.1) | Cam1: 192.168.4.20 | Cam2: 192.168.4.21 | Build: R2026-03-28-3</p>
  </main>
  <script>
    (function () {
      var speedInput = document.getElementById("speed");
      var speedValue = document.getElementById("speedValue");
      var stopButton = document.getElementById("stop");
      var hasPointer = !!window.PointerEvent;
      var holdButtons = [
        { id: "up", dir: "forward" },
        { id: "down", dir: "backward" },
        { id: "left", dir: "left" },
        { id: "right", dir: "right" }
      ];
      var activeDirection = "stop";
      var speedTimer = null;
      var activeHoldButtonId = null;
      var activePointerId = null;
      var ignoreMouseUntil = 0;

      function send(path) {
        if (window.fetch) {
          fetch(path, { cache: "no-store" }).catch(function () {});
        } else {
          var beacon = new Image();
          beacon.src = path + "&_ts=" + Date.now();
        }
      }

      function setActiveButton(id) {
        var buttons = document.querySelectorAll(".pad button");
        for (var i = 0; i < buttons.length; i++) {
          buttons[i].classList.remove("active");
        }
        if (!id) {
          return;
        }
        var selected = document.getElementById(id);
        if (selected) {
          selected.classList.add("active");
        }
      }

      function move(dir, buttonId) {
        if (activeDirection === dir && activeHoldButtonId === buttonId) {
          return;
        }
        activeDirection = dir;
        activeHoldButtonId = buttonId;
        setActiveButton(buttonId);
        send("/move?dir=" + dir);
      }

      function stop() {
        if (activeDirection === "stop") {
          return;
        }
        activeDirection = "stop";
        activeHoldButtonId = null;
        activePointerId = null;
        setActiveButton("stop");
        send("/move?dir=stop");
      }

      function bindHold(buttonId, dir) {
        var button = document.getElementById(buttonId);
        if (!button) {
          return;
        }

        function startMove(e) {
          if (e.cancelable) {
            e.preventDefault();
          }
          move(dir, buttonId);
        }

        function endMove(e) {
          if (e && e.cancelable) {
            e.preventDefault();
          }
          if (activeHoldButtonId === buttonId) {
            stop();
          }
        }

        if (hasPointer) {
          button.addEventListener("pointerdown", function (e) {
            if (e.button !== undefined && e.button !== 0) {
              return;
            }
            if (activePointerId !== null && activePointerId !== e.pointerId) {
              return;
            }
            activePointerId = e.pointerId;
            if (typeof button.setPointerCapture === "function") {
              try {
                button.setPointerCapture(e.pointerId);
              } catch (err) {}
            }
            startMove(e);
          });

          button.addEventListener("pointerup", function (e) {
            if (activePointerId === e.pointerId) {
              endMove(e);
            }
          });

          button.addEventListener("pointercancel", function (e) {
            if (activePointerId === e.pointerId) {
              endMove(e);
            }
          });

          button.addEventListener("lostpointercapture", function () {
            if (activeHoldButtonId === buttonId) {
              stop();
            }
          });
          return;
        }

        button.addEventListener("touchstart", function (e) {
          ignoreMouseUntil = Date.now() + 700;
          startMove(e);
        }, { passive: false });
        button.addEventListener("touchend", endMove, { passive: false });
        button.addEventListener("touchcancel", endMove, { passive: false });

        button.addEventListener("mousedown", function (e) {
          if (Date.now() < ignoreMouseUntil) {
            return;
          }
          if (e.button !== undefined && e.button !== 0) {
            return;
          }
          startMove(e);
        });
        button.addEventListener("mouseup", endMove);
        button.addEventListener("mouseleave", function (e) {
          if (e.buttons === 0) {
            endMove(e);
          }
        });
      }

      for (var j = 0; j < holdButtons.length; j++) {
        bindHold(holdButtons[j].id, holdButtons[j].dir);
      }

      if (stopButton) {
        if (hasPointer) {
          stopButton.addEventListener("pointerdown", function (e) {
            if (e.cancelable) {
              e.preventDefault();
            }
            stop();
          });
        } else {
          stopButton.addEventListener("click", function (e) {
            e.preventDefault();
            stop();
          });
        }
      }

      if (speedInput) {
        speedInput.addEventListener("input", function () {
          if (speedValue) {
            speedValue.textContent = speedInput.value;
          }
          if (speedTimer) {
            clearTimeout(speedTimer);
          }
          speedTimer = setTimeout(function () {
            send("/speed?value=" + encodeURIComponent(speedInput.value));
          }, 120);
        });
      }

      if (hasPointer) {
        document.addEventListener("pointerup", function (e) {
          if (activePointerId === e.pointerId) {
            stop();
          }
        });
        document.addEventListener("pointercancel", function (e) {
          if (activePointerId === e.pointerId) {
            stop();
          }
        });
      } else {
        document.addEventListener("touchend", function () {
          if (activeHoldButtonId) {
            stop();
          }
        }, { passive: false });
      }

      document.addEventListener("visibilitychange", function () {
        if (document.hidden) {
          stop();
        }
      });

      window.addEventListener("blur", function () {
        stop();
      });

      window.addEventListener("beforeunload", function () {
        stop();
      });

      setActiveButton("stop");
    })();
  </script>
</body>
</html>
)rawliteral";

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

void moveWithBoost(int dirA, uint8_t pwmA, int dirB, uint8_t pwmB, bool useBoost) {
  if (useBoost && (pwmA > 0 || pwmB > 0)) {
    setMotorA(dirA, max(pwmA, START_BOOST_PWM));
    setMotorB(dirB, max(pwmB, START_BOOST_PWM));
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
    case DIR_STOP:
    default:
      setMotorA(0, 0);
      setMotorB(0, 0);
      break;
  }

  applyDirectionLed(dir);
}

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "0");
  server.send_P(200, "text/html", INDEX_HTML);
}

void redirectToPortal(uint16_t code = 302) {
  String portalUrl = String("http://") + WiFi.softAPIP().toString() + "/";
  server.sendHeader("Location", portalUrl, true);
  server.send(code, "text/plain", "");
}

void redirectToRoot() {
  server.sendHeader("Location", "/", true);
  server.send(303, "text/plain", "");
}

void handleCaptiveProbe() {
  redirectToPortal(302);
}

void handleNotFound() {
  redirectToPortal(302);
}

void handleMove() {
  if (!server.hasArg("dir")) {
    server.send(400, "text/plain", "Missing dir");
    return;
  }

  String dir = server.arg("dir");

  if (dir == "forward") {
    applyDirection(DIR_FORWARD);
  } else if (dir == "backward") {
    applyDirection(DIR_BACKWARD);
  } else if (dir == "left") {
    applyDirection(DIR_LEFT);
  } else if (dir == "right") {
    applyDirection(DIR_RIGHT);
  } else if (dir == "stop") {
    applyDirection(DIR_STOP);
  } else {
    server.send(400, "text/plain", "Invalid dir");
    return;
  }

  if (server.hasArg("ui")) {
    redirectToRoot();
    return;
  }

  server.send(200, "text/plain", "OK");
}

void handleSpeed() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  int value = constrain(server.arg("value").toInt(), 0, 255);
  motorSpeed = static_cast<uint8_t>(value);

  if (currentDirection != DIR_STOP) {
    applyDirection(currentDirection);
  }

  if (server.hasArg("ui")) {
    redirectToRoot();
    return;
  }

  server.send(200, "text/plain", String(motorSpeed));
}

void setup() {
  Serial.begin(115200);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(ATOM_LED_PIN, OUTPUT);

  applyDirection(DIR_STOP);

  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(AP_SSID, AP_PASSWORD, 1, 0, 8);

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  server.on("/speed", HTTP_GET, handleSpeed);
  server.on("/generate_204", HTTP_GET, handleCaptiveProbe);       // Android
  server.on("/gen_204", HTTP_GET, handleCaptiveProbe);            // Android alt
  server.on("/hotspot-detect.html", HTTP_GET, handleCaptiveProbe); // iOS/macOS
  server.on("/library/test/success.html", HTTP_GET, handleCaptiveProbe); // iOS
  server.on("/connecttest.txt", HTTP_GET, handleCaptiveProbe);    // Windows
  server.on("/ncsi.txt", HTTP_GET, handleCaptiveProbe);           // Windows
  server.on("/fwlink", HTTP_GET, handleCaptiveProbe);             // Windows
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println();
  Serial.println("=== Robot AP mode ===");
  Serial.printf("AP started: %s\n", apStarted ? "YES" : "NO");
  Serial.printf("SSID: %s\n", AP_SSID);
  Serial.printf("Password: %s\n", AP_PASSWORD);
  Serial.printf("Open: http://%s\n", WiFi.softAPIP().toString().c_str());
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  updateStandbyLedBlink();
}
