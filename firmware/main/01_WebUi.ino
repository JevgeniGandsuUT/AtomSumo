const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Robot Control</title>
  <style>
    :root {
      --bg-a: #020617;
      --bg-b: #07111f;
      --panel: rgba(7, 17, 31, 0.94);
      --panel-line: rgba(34, 211, 238, 0.28);
      --ink: #e5f7ff;
      --muted: #8fb3c8;
      --btn: #0891b2;
      --btn-active: #22d3ee;
      --btn-stop: #fb345c;
      --btn-stop-active: #be123c;
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
      place-items: start center;
      background:
        linear-gradient(rgba(34, 211, 238, 0.07) 1px, transparent 1px),
        linear-gradient(90deg, rgba(34, 211, 238, 0.06) 1px, transparent 1px),
        radial-gradient(circle at 18% 0%, rgba(34, 211, 238, 0.32), transparent 34%),
        radial-gradient(circle at 86% 8%, rgba(244, 63, 94, 0.22), transparent 30%),
        linear-gradient(145deg, var(--bg-b), var(--bg-a));
      background-size: 28px 28px, 28px 28px, auto, auto, auto;
      color: var(--ink);
      padding: 8px;
    }

    .card {
      width: min(480px, 100%);
      background: var(--panel);
      border: 1px solid var(--panel-line);
      border-radius: 16px;
      box-shadow: 0 20px 46px rgba(0, 0, 0, 0.48), inset 0 1px 0 rgba(255, 255, 255, 0.08);
      padding: 10px;
      backdrop-filter: blur(12px);
    }

    h1 {
      margin: 0 0 8px;
      font-size: 1.05rem;
      text-align: center;
      text-transform: uppercase;
      letter-spacing: 0.14em;
      color: #a7f3ff;
    }

    .hint {
      margin: 4px 0 8px;
      text-align: center;
      color: var(--muted);
      font-size: 0.74rem;
      text-transform: uppercase;
      letter-spacing: 0.08em;
    }

    .cams {
      display: grid;
      grid-template-columns: 1fr;
      gap: 8px;
      margin-bottom: 8px;
    }

    .camera-tools {
      display: grid;
      grid-template-columns: repeat(6, 1fr);
      gap: 6px;
      margin-bottom: 8px;
    }

    .camera-tools button {
      min-height: 34px;
      border-radius: 8px;
      font-size: 0.72rem;
      box-shadow: 0 0 14px rgba(34, 211, 238, 0.2);
    }

    .hidden,
    .cam-card.hidden,
    .auto-box.hidden {
      display: none;
    }

    .cam-card {
      background: rgba(2, 6, 23, 0.78);
      border: 1px solid rgba(34, 211, 238, 0.22);
      border-radius: 10px;
      padding: 6px;
      color: #cbd5e1;
      box-shadow: inset 0 0 18px rgba(34, 211, 238, 0.08);
    }

    .cam-head {
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 0.68rem;
      font-weight: 700;
      margin: 0 3px 5px;
      letter-spacing: 0.12em;
      text-transform: uppercase;
    }

    .cam-state {
      font-size: 0.75rem;
      opacity: 0.9;
      text-transform: uppercase;
    }

    .cam-view {
      position: relative;
      width: 100%;
      aspect-ratio: 16 / 9;
      border-radius: 8px;
      overflow: hidden;
      background: #020617;
    }

    .cam-card img {
      width: 100%;
      height: 100%;
      object-fit: cover;
      display: block;
    }

    .tap-marker {
      position: absolute;
      width: 24px;
      height: 24px;
      left: 50%;
      top: 50%;
      transform: translate(-50%, -50%);
      border: 2px solid #facc15;
      border-radius: 50%;
      box-shadow: 0 0 0 1px rgba(2, 6, 23, 0.75), 0 0 16px rgba(250, 204, 21, 0.75);
      pointer-events: none;
    }

    .tap-marker:before,
    .tap-marker:after {
      content: "";
      position: absolute;
      background: #facc15;
      left: 50%;
      top: 50%;
      transform: translate(-50%, -50%);
    }

    .tap-marker:before {
      width: 34px;
      height: 2px;
    }

    .tap-marker:after {
      width: 2px;
      height: 34px;
    }

    .cam-card.tap-armed img {
      cursor: crosshair;
      outline: 2px solid rgba(45, 212, 191, 0.72);
      box-shadow: 0 0 20px rgba(45, 212, 191, 0.28);
    }

    .cam-card.mirrored img {
      transform: scaleX(-1);
    }

    .target-state {
      min-height: 18px;
      margin: -2px 2px 8px;
      color: #99f6e4;
      font-size: 0.72rem;
      font-weight: 700;
      text-align: center;
      text-transform: uppercase;
      letter-spacing: 0.06em;
    }

    .pad {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-areas:
        "upLeft up upRight"
        "left stop right"
        "downLeft down downRight";
      gap: 7px;
      margin-bottom: 8px;
    }

    button,
    .ctrl-btn {
      border: none;
      border-radius: 10px;
      min-height: 58px;
      font-size: 0.8rem;
      font-weight: 700;
      color: #e0fbff;
      background: linear-gradient(180deg, rgba(8, 145, 178, 0.98), rgba(14, 116, 144, 0.94));
      border: 1px solid rgba(125, 249, 255, 0.32);
      box-shadow: 0 0 16px rgba(34, 211, 238, 0.22), inset 0 1px 0 rgba(255, 255, 255, 0.12);
      transition: transform 0.08s ease, background 0.08s ease, box-shadow 0.08s ease;
      touch-action: manipulation;
      user-select: none;
      display: flex;
      align-items: center;
      justify-content: center;
      text-decoration: none;
      text-align: center;
      text-transform: uppercase;
      letter-spacing: 0.02em;
    }

    .pad button {
      touch-action: none;
    }

    button:active,
    button.active,
    .ctrl-btn:active {
      transform: scale(0.97);
      color: #041016;
      background: linear-gradient(180deg, var(--btn-active), #67e8f9);
      box-shadow: 0 0 24px rgba(34, 211, 238, 0.65);
    }

    #up { grid-area: up; }
    #upLeft { grid-area: upLeft; }
    #upRight { grid-area: upRight; }
    #left { grid-area: left; }
    #right { grid-area: right; }
    #down { grid-area: down; }
    #downLeft { grid-area: downLeft; }
    #downRight { grid-area: downRight; }
    #stop {
      grid-area: stop;
      background: linear-gradient(180deg, var(--btn-stop), var(--btn-stop-active));
      box-shadow: 0 0 18px rgba(244, 63, 94, 0.42);
    }

    #stop:active,
    #stop.active {
      color: #fff;
      background: var(--btn-stop-active);
    }

    .speed-box {
      background: rgba(2, 6, 23, 0.62);
      border: 1px solid rgba(125, 249, 255, 0.18);
      border-radius: 10px;
      padding: 8px 10px;
    }

    .speed-box + .speed-box {
      margin-top: 6px;
    }

    .auto-box {
      background: rgba(3, 30, 36, 0.86);
      border: 1px solid rgba(45, 212, 191, 0.28);
      border-radius: 10px;
      padding: 10px;
      margin: 8px 0;
    }

    .auto-row {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 8px;
      margin-bottom: 8px;
    }

    .auto-row button {
      min-height: 42px;
      border-radius: 8px;
      font-size: 0.76rem;
      box-shadow: 0 0 12px rgba(45, 212, 191, 0.22);
    }

    .auto-state {
      color: #99f6e4;
      font-size: 0.75rem;
      font-weight: 700;
      min-height: 18px;
    }

    .log-state {
      color: #bae6fd;
      font-size: 0.68rem;
      min-height: 16px;
      margin-top: -2px;
      margin-bottom: 6px;
    }

    .map-canvas {
      width: 100%;
      height: 150px;
      display: block;
      background: #020617;
      border: 1px solid rgba(45, 212, 191, 0.22);
      border-radius: 8px;
      margin-top: 8px;
    }

    .tof-box {
      display: grid;
      grid-template-columns: 1fr auto;
      gap: 4px 10px;
      align-items: center;
      background: rgba(8, 47, 73, 0.72);
      border: 1px solid rgba(56, 189, 248, 0.24);
      border-radius: 10px;
      padding: 8px 10px;
      margin-bottom: 8px;
    }

    .tof-label {
      color: #7dd3fc;
      font-size: 0.66rem;
      font-weight: 800;
      text-transform: uppercase;
      letter-spacing: 0.12em;
    }

    .tof-value {
      color: #e0faff;
      font-size: 1.15rem;
      font-weight: 800;
      line-height: 1;
      text-align: right;
    }

    .tof-status {
      grid-column: 1 / -1;
      min-height: 14px;
      color: var(--muted);
      font-size: 0.68rem;
    }

    .color-box {
      display: grid;
      grid-template-columns: auto 1fr auto;
      gap: 4px 10px;
      align-items: center;
      background: rgba(15, 23, 42, 0.72);
      border: 1px solid rgba(125, 249, 255, 0.18);
      border-radius: 10px;
      padding: 8px 10px;
      margin: 0 0 8px;
    }

    .color-swatch {
      width: 28px;
      height: 28px;
      border-radius: 7px;
      background: #111827;
      border: 1px solid rgba(226, 232, 240, 0.38);
      box-shadow: 0 0 12px rgba(34, 211, 238, 0.18);
    }

    .color-name {
      color: #e0faff;
      font-size: 0.9rem;
      font-weight: 800;
      text-transform: uppercase;
    }

    .color-rgb {
      color: #b7ecff;
      font-size: 0.7rem;
      font-weight: 700;
      text-align: right;
    }

    .color-status {
      grid-column: 1 / -1;
      min-height: 14px;
      color: var(--muted);
      font-size: 0.68rem;
    }

    .safety-box {
      background: rgba(15, 23, 42, 0.72);
      border: 1px solid rgba(125, 249, 255, 0.18);
      border-radius: 10px;
      padding: 8px 10px;
      margin-bottom: 8px;
    }

    .safety-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      gap: 10px;
      margin-bottom: 6px;
      color: #b7ecff;
      font-size: 0.76rem;
      font-weight: 800;
      text-transform: uppercase;
      letter-spacing: 0.04em;
    }

    .switch {
      position: relative;
      width: 48px;
      height: 26px;
      flex: 0 0 auto;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .switch span {
      position: absolute;
      cursor: pointer;
      inset: 0;
      background: rgba(51, 65, 85, 0.95);
      border: 1px solid rgba(148, 163, 184, 0.35);
      border-radius: 999px;
      transition: background 0.12s ease, border-color 0.12s ease;
    }

    .switch span:before {
      content: "";
      position: absolute;
      width: 20px;
      height: 20px;
      left: 2px;
      top: 2px;
      background: #cbd5e1;
      border-radius: 50%;
      transition: transform 0.12s ease, background 0.12s ease;
    }

    .switch input:checked + span {
      background: rgba(34, 211, 238, 0.72);
      border-color: rgba(125, 249, 255, 0.8);
    }

    .switch input:checked + span:before {
      transform: translateX(22px);
      background: #ecfeff;
    }

    .speed-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 4px;
      font-weight: 700;
      color: #b7ecff;
      font-size: 0.76rem;
      text-transform: uppercase;
      letter-spacing: 0.04em;
    }

    input[type="range"] {
      width: 100%;
      accent-color: #22d3ee;
    }

    .footer {
      margin-top: 8px;
      text-align: center;
      color: #638499;
      font-size: 0.62rem;
    }

    @media (min-width: 760px) {
      .card {
        width: min(860px, 100%);
        padding: 14px;
      }

      .cams {
        grid-template-columns: 1fr 1fr;
      }

      .pad {
        gap: 10px;
      }

      button,
      .ctrl-btn {
        min-height: 72px;
      }

      .map-canvas {
        height: 220px;
      }
    }
  </style>
</head>
<body>
  <main class="card">
    <h1>Robot Control</h1>
    <section class="camera-tools">
      <button type="button" id="toggleCam1">CAM 1</button>
      <button type="button" id="toggleCam2">CAM 2</button>
      <button type="button" id="toggleCamMirror">MIRROR</button>
      <button type="button" id="toggleTapDrive">TAP</button>
      <button type="button" id="toggleTapCalibrate">CALIB</button>
      <button type="button" id="toggleAutoPanel">AUTO</button>
    </section>
    <section class="cams">
      <article class="cam-card" id="cam1Card">
        <div class="cam-head">
          <span>CAM 1</span>
          <span class="cam-state">live</span>
        </div>
        <div class="cam-view" id="cam1View">
          <img id="cam1Stream" crossorigin="anonymous" src="http://192.168.4.20/stream" alt="Camera 1 stream" />
          <span class="tap-marker hidden" id="tapMarker"></span>
        </div>
      </article>
      <article class="cam-card" id="cam2Card">
        <div class="cam-head">
          <span>CAM 2</span>
          <span class="cam-state">live</span>
        </div>
        <img src="http://192.168.4.21/stream" alt="Camera 2 stream" />
      </article>
    </section>
    <div class="target-state" id="targetState">Tap drive idle</div>
    <section class="tof-box">
      <span class="tof-label">Distance</span>
      <span class="tof-value"><span id="tofValue">--</span> mm</span>
      <span class="tof-status" id="tofStatus">VL53L0X starting...</span>
    </section>

    <section class="color-box">
      <span class="color-swatch" id="cam2ColorSwatch"></span>
      <span class="color-name" id="cam2ColorName">Color --</span>
      <span class="color-rgb" id="cam2ColorRgb">R -- G -- B --</span>
      <span class="color-status" id="cam2ColorStatus">TCS34725 on CAM 2 starting...</span>
    </section>

    <section class="safety-box">
      <div class="safety-row">
        <span>TOF stop</span>
        <label class="switch" aria-label="Toggle TOF stop mode">
          <input id="tofStopMode" type="checkbox" checked />
          <span></span>
        </label>
      </div>
      <div class="speed-row">
        <span>Stop below</span>
        <span><strong id="tofStopValue">150</strong> mm</span>
      </div>
      <input id="tofStopDistance" name="value" type="range" min="60" max="400" step="10" value="150" />
    </section>

    <section class="speed-box">
      <div class="speed-row">
        <span>Tap horizontal</span>
        <span><strong id="tapHorizontalValue">50</strong> deg</span>
      </div>
      <input id="tapHorizontal" name="value" type="range" min="30" max="100" step="1" value="50" />
    </section>

    <section class="speed-box">
      <div class="speed-row">
        <span>Tap vertical</span>
        <span><strong id="tapVerticalValue">5000</strong> ms</span>
      </div>
      <input id="tapVertical" name="value" type="range" min="1000" max="9000" step="100" value="5000" />
    </section>

    <p class="hint">Manual drive</p>

    <section class="pad">
      <button type="button" id="upLeft">FWD LEFT</button>
      <button type="button" id="up">FORWARD</button>
      <button type="button" id="upRight">FWD RIGHT</button>
      <button type="button" id="left">LEFT</button>
      <button type="button" id="stop">STOP</button>
      <button type="button" id="right">RIGHT</button>
      <button type="button" id="downLeft">BACK LEFT</button>
      <button type="button" id="down">BACK</button>
      <button type="button" id="downRight">BACK RIGHT</button>
    </section>

    <section class="speed-box">
      <div class="speed-row">
        <span>Speed</span>
        <span><strong id="speedValue">180</strong> / 255</span>
      </div>
      <input id="speed" name="value" type="range" min="0" max="255" value="180" />
    </section>

    <section class="speed-box">
      <div class="speed-row">
        <span>Arc turn</span>
        <span><strong id="arcValue">55</strong> / 255</span>
      </div>
      <input id="arcTurn" name="value" type="range" min="0" max="255" value="55" />
    </section>

    <section class="auto-box hidden" id="autoPanel">
      <div class="auto-row">
        <button type="button" id="autoMode">Auto mode</button>
        <button type="button" id="clearMap">Clear map</button>
      </div>
      <div class="auto-row">
        <button type="button" id="autoLogToggle">Start log</button>
        <button type="button" id="autoLogDownload">CSV</button>
      </div>
      <div class="auto-row">
        <button type="button" id="autoLogClear">Clear log</button>
        <button type="button" id="autoMarkPass">Mark pass</button>
      </div>
      <div class="auto-state" id="autoState">Auto idle</div>
      <div class="log-state" id="autoLogState">Auto log idle</div>
      <div class="speed-row">
        <span>Turn below</span>
        <span><strong id="autoTurnValue">150</strong> mm</span>
      </div>
      <input id="autoTurnDistance" name="value" type="range" min="80" max="500" step="10" value="150" />
      <canvas class="map-canvas" id="mapCanvas" width="420" height="220"></canvas>
    </section>

    <p class="footer">AP: Robot-Control (192.168.4.1) | Cam1: 192.168.4.20 | Cam2: 192.168.4.21 | Build: R2026-06-13-cam2-color</p>
  </main>
  <script>
    (function () {
      var speedInput = document.getElementById("speed");
      var speedValue = document.getElementById("speedValue");
      var arcInput = document.getElementById("arcTurn");
      var arcValue = document.getElementById("arcValue");
      var tofValue = document.getElementById("tofValue");
      var tofStatus = document.getElementById("tofStatus");
      var cam2ColorSwatch = document.getElementById("cam2ColorSwatch");
      var cam2ColorName = document.getElementById("cam2ColorName");
      var cam2ColorRgb = document.getElementById("cam2ColorRgb");
      var cam2ColorStatus = document.getElementById("cam2ColorStatus");
      var tofStopModeInput = document.getElementById("tofStopMode");
      var tofStopInput = document.getElementById("tofStopDistance");
      var tofStopValue = document.getElementById("tofStopValue");
      var tapHorizontalInput = document.getElementById("tapHorizontal");
      var tapHorizontalValue = document.getElementById("tapHorizontalValue");
      var tapVerticalInput = document.getElementById("tapVertical");
      var tapVerticalValue = document.getElementById("tapVerticalValue");
      var cam1Card = document.getElementById("cam1Card");
      var cam2Card = document.getElementById("cam2Card");
      var cam1View = document.getElementById("cam1View");
      var cam1Stream = document.getElementById("cam1Stream");
      var tapMarker = document.getElementById("tapMarker");
      var toggleCam1Button = document.getElementById("toggleCam1");
      var toggleCam2Button = document.getElementById("toggleCam2");
      var toggleCamMirrorButton = document.getElementById("toggleCamMirror");
      var toggleTapDriveButton = document.getElementById("toggleTapDrive");
      var toggleTapCalibrateButton = document.getElementById("toggleTapCalibrate");
      var toggleAutoPanelButton = document.getElementById("toggleAutoPanel");
      var autoPanel = document.getElementById("autoPanel");
      var targetState = document.getElementById("targetState");
      var autoModeButton = document.getElementById("autoMode");
      var clearMapButton = document.getElementById("clearMap");
      var autoTurnInput = document.getElementById("autoTurnDistance");
      var autoTurnValue = document.getElementById("autoTurnValue");
      var autoState = document.getElementById("autoState");
      var autoLogToggleButton = document.getElementById("autoLogToggle");
      var autoLogDownloadButton = document.getElementById("autoLogDownload");
      var autoLogClearButton = document.getElementById("autoLogClear");
      var autoMarkPassButton = document.getElementById("autoMarkPass");
      var autoLogState = document.getElementById("autoLogState");
      var mapCanvas = document.getElementById("mapCanvas");
      var stopButton = document.getElementById("stop");
      var hasPointer = !!window.PointerEvent;
      var holdButtons = [
        { id: "up", dir: "forward" },
        { id: "down", dir: "backward" },
        { id: "left", dir: "left" },
        { id: "right", dir: "right" },
        { id: "upLeft", dir: "forward-left" },
        { id: "upRight", dir: "forward-right" },
        { id: "downLeft", dir: "backward-left" },
        { id: "downRight", dir: "backward-right" }
      ];
      var activeDirection = "stop";
      var speedTimer = null;
      var arcTimer = null;
      var cam2ColorPending = false;
      var activeHoldButtonId = null;
      var activePointerId = null;
      var ignoreMouseUntil = 0;
      var lastTofData = null;
      var autoEnabled = false;
      var tapDriveEnabled = false;
      var tapCalibrateEnabled = false;
      var cam1Mirrored = true;
      var tapDriveTimers = [];
      var tapDriveSpeed = 70;
      var tapTurnSpeed = 150;
      var tapHorizontalFovDeg = 50;
      var tapCenterDriveMs = 5000;
      var tapTurnDegPerSec = 193;
      var tapCanvas = document.createElement("canvas");
      var tapCtx = tapCanvas.getContext("2d", { willReadFrequently: true });
      var tapFrameW = 160;
      var tapFrameH = 90;
      var autoTimer = null;
      var autoTurningUntilClear = false;
      var autoTurnDistanceMm = 150;
      var tofStopDistanceMm = 150;
      var autoTurnDir = "right";
      var autoLogEnabled = false;
      var autoLogRows = [];
      var autoLogTrial = 1;
      var mapCtx = mapCanvas ? mapCanvas.getContext("2d") : null;
      var mapPose = { x: 210, y: 180, heading: -Math.PI / 2 };
      var mapLastMs = Date.now();

      function send(path) {
        if (window.fetch) {
          fetch(path, { cache: "no-store" }).catch(function () {});
        } else {
          var beacon = new Image();
          beacon.src = path + "&_ts=" + Date.now();
        }
      }

      function setTargetState(text) {
        if (targetState) {
          targetState.textContent = text;
        }
      }

      function updateAutoLogState() {
        if (!autoLogState) {
          return;
        }
        autoLogState.textContent = (autoLogEnabled ? "Logging" : "Log idle") + " | rows " + autoLogRows.length + " | trial " + autoLogTrial;
      }

      function startAutoLog() {
        autoLogEnabled = true;
        if (autoLogToggleButton) {
          autoLogToggleButton.classList.add("active");
          autoLogToggleButton.textContent = "Stop log";
        }
        updateAutoLogState();
      }

      function stopAutoLog() {
        autoLogEnabled = false;
        if (autoLogToggleButton) {
          autoLogToggleButton.classList.remove("active");
          autoLogToggleButton.textContent = "Start log";
        }
        updateAutoLogState();
      }

      function toggleAutoLog() {
        if (autoLogEnabled) {
          stopAutoLog();
        } else {
          startAutoLog();
        }
      }

      function clearAutoLog() {
        autoLogRows = [];
        autoLogTrial = 1;
        updateAutoLogState();
      }

      function addAutoLogRow(action, result, note) {
        if (!autoLogEnabled) {
          return;
        }
        autoLogRows.push({
          ts_ms: Date.now(),
          trial: autoLogTrial,
          tof_ready: lastTofData && lastTofData.ready ? "true" : "false",
          tof_valid: lastTofData && lastTofData.valid ? "true" : "false",
          distance_mm: lastTofData && lastTofData.valid ? lastTofData.mm : "",
          threshold_mm: autoTurnDistanceMm,
          action: action,
          result: result,
          note: note || ""
        });
        updateAutoLogState();
      }

      function markAutoPass() {
        if (autoLogRows.length === 0) {
          return;
        }
        addAutoLogRow("mark", "pass", "operator marked trial pass");
        autoLogTrial += 1;
        updateAutoLogState();
      }

      function csvEscape(value) {
        var text = String(value === undefined || value === null ? "" : value);
        if (text.indexOf(",") >= 0 || text.indexOf("\"") >= 0 || text.indexOf("\n") >= 0) {
          return "\"" + text.replace(/"/g, "\"\"") + "\"";
        }
        return text;
      }

      function downloadAutoLogCsv() {
        var header = ["ts_ms", "trial", "tof_ready", "tof_valid", "distance_mm", "threshold_mm", "action", "result", "note"];
        var lines = [header.join(",")];
        for (var i = 0; i < autoLogRows.length; i++) {
          var row = autoLogRows[i];
          lines.push(header.map(function (key) { return csvEscape(row[key]); }).join(","));
        }
        var blob = new Blob([lines.join("\n") + "\n"], { type: "text/csv" });
        var url = URL.createObjectURL(blob);
        var a = document.createElement("a");
        a.href = url;
        a.download = "auto_behavior_log.csv";
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
      }

      function clearTapDriveTimers() {
        for (var i = 0; i < tapDriveTimers.length; i++) {
          clearTimeout(tapDriveTimers[i]);
        }
        tapDriveTimers = [];
      }

      function scheduleTapStep(delayMs, fn) {
        tapDriveTimers.push(setTimeout(fn, delayMs));
      }

      function showTapMarker(xPx, yPx) {
        if (!tapMarker) {
          return;
        }
        tapMarker.style.left = xPx + "px";
        tapMarker.style.top = yPx + "px";
        tapMarker.classList.remove("hidden");
      }

      function visualToSourceX(xPx, width) {
        return cam1Mirrored ? width - xPx : xPx;
      }

      function sourceToVisualX(xPx, width) {
        return cam1Mirrored ? width - xPx : xPx;
      }

      function drawTapFrame() {
        if (!tapCtx || !cam1Stream || !cam1Stream.complete) {
          return false;
        }
        tapCanvas.width = tapFrameW;
        tapCanvas.height = tapFrameH;
        try {
          tapCtx.drawImage(cam1Stream, 0, 0, tapFrameW, tapFrameH);
          return true;
        } catch (err) {
          return false;
        }
      }

      /*
       * Red-target visual servoing was tested as an experiment and is disabled.
       * It is not used as project evidence because the behavior is not reliable
       * enough on the current camera/lighting setup.
       */
      function tapServoStep() {
        setTargetState("Red target servo disabled");
        tapCommand("stop", "stop");
      }

      function setUiSpeed(value) {
        var safeValue = Math.max(0, Math.min(255, value));
        if (speedInput) {
          speedInput.value = safeValue;
        }
        if (speedValue) {
          speedValue.textContent = safeValue;
        }
        send("/speed?value=" + safeValue);
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

      function updateTof() {
        if (!window.fetch || !tofValue || !tofStatus) {
          return Promise.resolve(null);
        }

        return fetch("/tof", { cache: "no-store" })
          .then(function (response) {
            if (!response.ok) {
              throw new Error("HTTP " + response.status);
            }
            return response.json();
          })
          .then(function (data) {
            lastTofData = data;
            if (!data.ready) {
              tofValue.textContent = "--";
              tofStatus.textContent = "VL53L0X offline, I2C: " + (data.scan || "unknown") + " on SDA " + data.sda + " / SCL " + data.scl;
              return;
            }
            if (data.valid) {
              tofValue.textContent = data.mm;
              tofStatus.textContent = "range status " + data.status + " on SDA " + data.sda + " / SCL " + data.scl;
            } else {
              tofValue.textContent = "--";
              tofStatus.textContent = "out of range";
            }
            enforceManualTofStop();
          })
          .catch(function () {
            tofValue.textContent = "--";
            tofStatus.textContent = "no sensor data";
            lastTofData = null;
          });
      }

      function clampByte(value) {
        return Math.max(0, Math.min(255, Math.round(value)));
      }

      function updateCam2Color() {
        if (!window.fetch || !cam2ColorSwatch || !cam2ColorName || !cam2ColorRgb || !cam2ColorStatus) {
          return Promise.resolve(null);
        }
        if (cam2ColorPending) {
          return Promise.resolve(null);
        }

        cam2ColorPending = true;

        var controller = window.AbortController ? new AbortController() : null;
        var timeoutId = controller ? setTimeout(function () {
          controller.abort();
        }, 700) : null;
        var fetchOptions = { cache: "no-store" };
        if (controller) {
          fetchOptions.signal = controller.signal;
        }

        return fetch("http://192.168.4.21:81/color", fetchOptions)
          .then(function (response) {
            if (!response.ok) {
              throw new Error("HTTP " + response.status);
            }
            return response.json();
          })
          .then(function (data) {
            if (!data.ready) {
              cam2ColorName.textContent = "Color --";
              cam2ColorRgb.textContent = "R -- G -- B --";
              cam2ColorStatus.textContent = "TCS34725 offline on CAM 2";
              cam2ColorSwatch.style.background = "#111827";
              return;
            }

            var clear = Math.max(1, data.clear || 1);
            var scale = 255 / Math.max(data.red || 0, data.green || 0, data.blue || 0, clear);
            var r = clampByte((data.red || 0) * scale);
            var g = clampByte((data.green || 0) * scale);
            var b = clampByte((data.blue || 0) * scale);
            var cssColor = "rgb(" + r + "," + g + "," + b + ")";

            cam2ColorSwatch.style.background = cssColor;
            cam2ColorName.textContent = "Color " + (data.name || "mixed");
            cam2ColorRgb.textContent = "R " + data.red + " G " + data.green + " B " + data.blue;
            cam2ColorStatus.textContent = "clear " + data.clear + " on SDA " + data.sda + " / SCL " + data.scl;
          })
          .catch(function () {
            cam2ColorName.textContent = "Color --";
            cam2ColorRgb.textContent = "R -- G -- B --";
            cam2ColorStatus.textContent = "no CAM 2 color data";
            cam2ColorSwatch.style.background = "#111827";
          })
          .then(function () {
            if (timeoutId) {
              clearTimeout(timeoutId);
            }
            cam2ColorPending = false;
          });
      }

      function isTofProtectedDirection(dir) {
        return dir === "forward" || dir === "forward-left" || dir === "forward-right";
      }

      function enforceManualTofStop() {
        if (autoEnabled || !tofStopModeInput || !tofStopModeInput.checked) {
          return;
        }
        if (!lastTofData || !lastTofData.ready || !lastTofData.valid) {
          return;
        }
        if (!isTofProtectedDirection(activeDirection)) {
          return;
        }
        if (lastTofData.mm < tofStopDistanceMm) {
          stop();
          if (tofStatus) {
            tofStatus.textContent = "TOF stop at " + lastTofData.mm + " mm";
          }
        }
      }

      function drawMapBase() {
        if (!mapCtx || !mapCanvas) {
          return;
        }
        mapCtx.fillStyle = "#020617";
        mapCtx.fillRect(0, 0, mapCanvas.width, mapCanvas.height);
        mapCtx.strokeStyle = "rgba(148, 163, 184, 0.22)";
        mapCtx.lineWidth = 1;
        for (var x = 20; x < mapCanvas.width; x += 20) {
          mapCtx.beginPath();
          mapCtx.moveTo(x, 0);
          mapCtx.lineTo(x, mapCanvas.height);
          mapCtx.stroke();
        }
        for (var y = 20; y < mapCanvas.height; y += 20) {
          mapCtx.beginPath();
          mapCtx.moveTo(0, y);
          mapCtx.lineTo(mapCanvas.width, y);
          mapCtx.stroke();
        }
      }

      function drawRobotOnMap() {
        if (!mapCtx) {
          return;
        }
        mapCtx.save();
        mapCtx.translate(mapPose.x, mapPose.y);
        mapCtx.rotate(mapPose.heading);
        mapCtx.fillStyle = "#22c55e";
        mapCtx.beginPath();
        mapCtx.moveTo(0, -7);
        mapCtx.lineTo(5, 7);
        mapCtx.lineTo(-5, 7);
        mapCtx.closePath();
        mapCtx.fill();
        mapCtx.restore();
      }

      function resetMap() {
        mapPose = { x: 210, y: 180, heading: -Math.PI / 2 };
        mapLastMs = Date.now();
        drawMapBase();
        drawRobotOnMap();
      }

      function plotObstacle(mm) {
        if (!mapCtx || !mm) {
          return;
        }
        var scale = 0.08;
        var ox = mapPose.x + Math.cos(mapPose.heading) * mm * scale;
        var oy = mapPose.y + Math.sin(mapPose.heading) * mm * scale;
        mapCtx.fillStyle = "#f97316";
        mapCtx.fillRect(ox - 2, oy - 2, 4, 4);
      }

      function advanceMap(dir) {
        if (!mapCtx) {
          return;
        }
        var now = Date.now();
        var dt = Math.min(0.5, (now - mapLastMs) / 1000);
        mapLastMs = now;
        var oldX = mapPose.x;
        var oldY = mapPose.y;
        var step = 36 * dt;
        var turn = 1.7 * dt;

        if (dir === "left") {
          mapPose.heading -= turn * 1.6;
        } else if (dir === "right") {
          mapPose.heading += turn * 1.6;
        } else if (dir === "forward-left") {
          mapPose.heading -= turn;
          mapPose.x += Math.cos(mapPose.heading) * step;
          mapPose.y += Math.sin(mapPose.heading) * step;
        } else if (dir === "forward-right") {
          mapPose.heading += turn;
          mapPose.x += Math.cos(mapPose.heading) * step;
          mapPose.y += Math.sin(mapPose.heading) * step;
        } else if (dir === "forward") {
          mapPose.x += Math.cos(mapPose.heading) * step;
          mapPose.y += Math.sin(mapPose.heading) * step;
        } else if (dir === "backward") {
          mapPose.x -= Math.cos(mapPose.heading) * step;
          mapPose.y -= Math.sin(mapPose.heading) * step;
        } else if (dir === "backward-left") {
          mapPose.heading += turn;
          mapPose.x -= Math.cos(mapPose.heading) * step;
          mapPose.y -= Math.sin(mapPose.heading) * step;
        } else if (dir === "backward-right") {
          mapPose.heading -= turn;
          mapPose.x -= Math.cos(mapPose.heading) * step;
          mapPose.y -= Math.sin(mapPose.heading) * step;
        }

        mapPose.x = Math.max(8, Math.min(mapCanvas.width - 8, mapPose.x));
        mapPose.y = Math.max(8, Math.min(mapCanvas.height - 8, mapPose.y));
        mapCtx.strokeStyle = "#38bdf8";
        mapCtx.lineWidth = 2;
        mapCtx.beginPath();
        mapCtx.moveTo(oldX, oldY);
        mapCtx.lineTo(mapPose.x, mapPose.y);
        mapCtx.stroke();
        if (lastTofData && lastTofData.valid) {
          plotObstacle(lastTofData.mm);
        }
        drawRobotOnMap();
      }

      function autoCommand(dir, buttonId) {
        if (activeDirection !== dir) {
          activeDirection = dir;
          activeHoldButtonId = null;
          activePointerId = null;
          setActiveButton(buttonId);
          send("/move?dir=" + dir);
        }
        advanceMap(dir);
      }

      function setAutoMode(enabled) {
        if (enabled && tapDriveEnabled) {
          setTapDriveMode(false);
        }
        if (enabled && tapCalibrateEnabled) {
          setTapCalibrateMode(false);
        }
        autoEnabled = enabled;
        if (autoModeButton) {
          autoModeButton.classList.toggle("active", enabled);
          autoModeButton.textContent = enabled ? "Stop auto" : "Auto mode";
        }
        if (autoState) {
          autoState.textContent = enabled ? "Auto exploring" : "Auto idle";
        }
        if (!enabled) {
          stop();
        }
      }

      function setTapDriveMode(enabled) {
        if (enabled && tapCalibrateEnabled) {
          setTapCalibrateMode(false);
        }
        tapDriveEnabled = enabled;
        clearTapDriveTimers();
        if (toggleTapDriveButton) {
          toggleTapDriveButton.classList.toggle("active", enabled);
        }
        if (cam1Card) {
          cam1Card.classList.toggle("tap-armed", enabled);
        }
        if (!enabled) {
          stop();
          setTargetState("Tap drive idle");
        } else {
          if (autoEnabled) {
            setAutoMode(false);
          }
          setTargetState("Tap CAM 1 target");
        }
      }

      function setTapCalibrateMode(enabled) {
        if (enabled && tapDriveEnabled) {
          setTapDriveMode(false);
        }
        if (enabled && autoEnabled) {
          setAutoMode(false);
        }
        tapCalibrateEnabled = enabled;
        clearTapDriveTimers();
        if (toggleTapCalibrateButton) {
          toggleTapCalibrateButton.classList.toggle("active", enabled);
        }
        if (cam1Card) {
          cam1Card.classList.toggle("tap-armed", enabled || tapDriveEnabled);
        }
        stop();
        setTargetState(enabled ? "Calibration click mode: robot will not move" : "Tap drive idle");
      }

      function tapCommand(dir, buttonId) {
        activeDirection = dir;
        activeHoldButtonId = null;
        activePointerId = null;
        setActiveButton(buttonId);
        send("/move?dir=" + dir);
        enforceManualTofStop();
      }

      function executeTapDrive(e) {
        if (!tapDriveEnabled || !cam1View) {
          return;
        }
        if (e.cancelable) {
          e.preventDefault();
        }
        if (autoEnabled) {
          setAutoMode(false);
        }

        var rect = cam1View.getBoundingClientRect();
        if (!rect.width || !rect.height) {
          return;
        }
        var xPx = e.clientX - rect.left;
        var yPx = e.clientY - rect.top;
        var xNorm = (xPx / rect.width) * 2 - 1;
        var yNorm = yPx / rect.height;
        xNorm = Math.max(-1, Math.min(1, xNorm));
        yNorm = Math.max(0, Math.min(1, yNorm));
        showTapMarker(xPx, yPx);
        clearTapDriveTimers();
        stop();
        var angleDeg = xNorm * (tapHorizontalFovDeg / 2);
        var turnMs = Math.round(Math.abs(angleDeg) / tapTurnDegPerSec * 1000);
        var verticalOffset = 0.5 - yNorm;
        var driveMs = Math.round(tapCenterDriveMs + verticalOffset * tapCenterDriveMs);
        driveMs = Math.max(600, Math.min(12000, driveMs));
        var turnDir = angleDeg < 0 ? "left" : "right";

        setTargetState("Angle " + angleDeg.toFixed(1) + " deg, turn " + turnMs + " ms, drive " + driveMs + " ms");

        if (Math.abs(angleDeg) > 4) {
          scheduleTapStep(700, function () {
            setUiSpeed(tapTurnSpeed);
            setTargetState("Turning " + turnDir + " " + angleDeg.toFixed(1) + " deg");
            tapCommand(turnDir, turnDir);
          });
          scheduleTapStep(700 + turnMs, function () {
            tapCommand("stop", "stop");
          });
          scheduleTapStep(1250 + turnMs, function () {
            setUiSpeed(tapDriveSpeed);
            setTargetState("Driving to tap target");
            tapCommand("forward", "up");
          });
          scheduleTapStep(1250 + turnMs + driveMs, function () {
            stop();
            setTargetState("Tap drive done");
          });
        } else {
          scheduleTapStep(700, function () {
            setUiSpeed(tapDriveSpeed);
            setTargetState("Driving straight to tap target");
            tapCommand("forward", "up");
          });
          scheduleTapStep(700 + driveMs, function () {
            stop();
            setTargetState("Tap drive done");
          });
        }
      }

      function executeTapCalibration(e) {
        if (!tapCalibrateEnabled || !cam1View) {
          return;
        }
        if (e.cancelable) {
          e.preventDefault();
        }
        var rect = cam1View.getBoundingClientRect();
        if (!rect.width || !rect.height) {
          return;
        }
        var xPx = e.clientX - rect.left;
        var yPx = e.clientY - rect.top;
        var xNorm = (xPx / rect.width) * 2 - 1;
        var yNorm = yPx / rect.height;
        xNorm = Math.max(-1, Math.min(1, xNorm));
        yNorm = Math.max(0, Math.min(1, yNorm));
        showTapMarker(xPx, yPx);
        clearTapDriveTimers();
        stop();
        setTargetState("PX " + Math.round(xPx) + "," + Math.round(yPx) + " | norm " + xNorm.toFixed(3) + "," + yNorm.toFixed(3));
      }

      function handleCam1Click(e) {
        if (tapCalibrateEnabled) {
          executeTapCalibration(e);
          return;
        }
        executeTapDrive(e);
      }

      function autoTick() {
        if (!autoEnabled) {
          return;
        }
        updateTof().then(function () {
          if (!autoEnabled) {
            return;
          }
          if (!lastTofData || !lastTofData.ready) {
            autoCommand("stop", "stop");
            addAutoLogRow("stop", "waiting", "VL53L0X not ready");
            if (autoState) {
              autoState.textContent = "Auto waiting for VL53L0X";
            }
            return;
          }

          if (autoTurningUntilClear) {
            if (lastTofData.valid && lastTofData.mm > 400) {
              autoTurningUntilClear = false;
              autoCommand("forward", "up");
              addAutoLogRow("forward", "clear", "distance above 400 mm after turn");
              if (autoState) {
                autoState.textContent = "Clear " + lastTofData.mm + " mm, forward";
              }
              return;
            }
            autoCommand(autoTurnDir, autoTurnDir);
            addAutoLogRow(autoTurnDir, "turning", lastTofData.valid ? "turning until clear" : "turning with invalid distance");
            if (autoState) {
              autoState.textContent = lastTofData.valid ? "Turning " + autoTurnDir + " until >400 mm, now " + lastTofData.mm + " mm" : "Turning " + autoTurnDir + ", no return";
            }
            return;
          }

          if (lastTofData.valid && lastTofData.mm < autoTurnDistanceMm) {
            autoTurnDir = autoTurnDir === "right" ? "left" : "right";
            autoTurningUntilClear = true;
            autoCommand(autoTurnDir, autoTurnDir);
            addAutoLogRow(autoTurnDir, "obstacle", "distance below turn threshold");
            if (autoState) {
              autoState.textContent = "Obstacle " + lastTofData.mm + " mm, turning below " + autoTurnDistanceMm + " mm";
            }
            return;
          }
          if (lastTofData.valid && lastTofData.mm < autoTurnDistanceMm + 110) {
            var arcDir = autoTurnDir === "right" ? "forward-right" : "forward-left";
            var arcButton = autoTurnDir === "right" ? "upRight" : "upLeft";
            autoCommand(arcDir, arcButton);
            addAutoLogRow(arcDir, "near", "distance near threshold");
            if (autoState) {
              autoState.textContent = "Close wall " + lastTofData.mm + " mm, arcing";
            }
            return;
          }
          autoCommand("forward", "up");
          addAutoLogRow("forward", "clear", lastTofData.valid ? "distance clear" : "distance invalid");
          if (autoState) {
            autoState.textContent = lastTofData.valid ? "Forward, clear " + lastTofData.mm + " mm" : "Forward, no return";
          }
        });
      }

      function setCameraVisible(card, button, label, visible) {
        if (!card || !button) {
          return;
        }
        card.classList.toggle("hidden", !visible);
        button.textContent = label;
        button.classList.toggle("active", visible);
      }

      function setCam1Mirror(enabled) {
        cam1Mirrored = enabled;
        if (cam1Card) {
          cam1Card.classList.toggle("mirrored", enabled);
        }
        if (toggleCamMirrorButton) {
          toggleCamMirrorButton.classList.toggle("active", enabled);
        }
      }

      function setAutoPanelVisible(visible) {
        if (!autoPanel || !toggleAutoPanelButton) {
          return;
        }
        autoPanel.classList.toggle("hidden", !visible);
        toggleAutoPanelButton.classList.toggle("active", visible);
        if (!visible) {
          setAutoMode(false);
        } else {
          resetMap();
        }
      }

      function move(dir, buttonId) {
        if (autoEnabled) {
          setAutoMode(false);
        }
        if (tapDriveEnabled) {
          setTapDriveMode(false);
        }
        if (tapCalibrateEnabled) {
          setTapCalibrateMode(false);
        }
        if (activeDirection === dir && activeHoldButtonId === buttonId) {
          return;
        }
        activeDirection = dir;
        activeHoldButtonId = buttonId;
        setActiveButton(buttonId);
        send("/move?dir=" + dir);
        enforceManualTofStop();
      }

      function stop() {
        clearTapDriveTimers();
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
            setAutoMode(false);
            setTapDriveMode(false);
            setTapCalibrateMode(false);
          });
        } else {
          stopButton.addEventListener("click", function (e) {
            e.preventDefault();
            setAutoMode(false);
            setTapDriveMode(false);
            setTapCalibrateMode(false);
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

      if (arcInput) {
        arcInput.addEventListener("input", function () {
          if (arcValue) {
            arcValue.textContent = arcInput.value;
          }
          if (arcTimer) {
            clearTimeout(arcTimer);
          }
          arcTimer = setTimeout(function () {
            send("/arc?value=" + encodeURIComponent(arcInput.value));
          }, 120);
        });
      }

      if (tofStopInput) {
        tofStopInput.addEventListener("input", function () {
          tofStopDistanceMm = parseInt(tofStopInput.value, 10) || 150;
          if (tofStopValue) {
            tofStopValue.textContent = tofStopDistanceMm;
          }
          enforceManualTofStop();
        });
      }

      if (tofStopModeInput) {
        tofStopModeInput.addEventListener("change", enforceManualTofStop);
      }

      if (tapHorizontalInput) {
        tapHorizontalInput.addEventListener("input", function () {
          tapHorizontalFovDeg = parseInt(tapHorizontalInput.value, 10) || 62;
          if (tapHorizontalValue) {
            tapHorizontalValue.textContent = tapHorizontalFovDeg;
          }
        });
      }

      if (tapVerticalInput) {
        tapVerticalInput.addEventListener("input", function () {
          tapCenterDriveMs = parseInt(tapVerticalInput.value, 10) || 5000;
          if (tapVerticalValue) {
            tapVerticalValue.textContent = tapCenterDriveMs;
          }
        });
      }

      if (toggleCam1Button) {
        toggleCam1Button.addEventListener("click", function () {
          setCameraVisible(cam1Card, toggleCam1Button, "CAM 1", cam1Card.classList.contains("hidden"));
        });
      }

      if (toggleCam2Button) {
        toggleCam2Button.addEventListener("click", function () {
          setCameraVisible(cam2Card, toggleCam2Button, "CAM 2", cam2Card.classList.contains("hidden"));
        });
      }

      if (toggleCamMirrorButton) {
        toggleCamMirrorButton.addEventListener("click", function () {
          setCam1Mirror(!cam1Mirrored);
        });
      }

      if (toggleTapDriveButton) {
        toggleTapDriveButton.addEventListener("click", function () {
          setTapDriveMode(!tapDriveEnabled);
        });
      }

      if (toggleTapCalibrateButton) {
        toggleTapCalibrateButton.addEventListener("click", function () {
          setTapCalibrateMode(!tapCalibrateEnabled);
        });
      }

      if (cam1View) {
        cam1View.addEventListener("click", handleCam1Click);
      }

      if (toggleAutoPanelButton) {
        toggleAutoPanelButton.addEventListener("click", function () {
          setAutoPanelVisible(autoPanel.classList.contains("hidden"));
        });
      }

      if (autoModeButton) {
        autoModeButton.addEventListener("click", function () {
          setAutoMode(!autoEnabled);
        });
      }

      if (clearMapButton) {
        clearMapButton.addEventListener("click", function () {
          resetMap();
        });
      }

      if (autoLogToggleButton) {
        autoLogToggleButton.addEventListener("click", toggleAutoLog);
      }

      if (autoLogDownloadButton) {
        autoLogDownloadButton.addEventListener("click", downloadAutoLogCsv);
      }

      if (autoLogClearButton) {
        autoLogClearButton.addEventListener("click", clearAutoLog);
      }

      if (autoMarkPassButton) {
        autoMarkPassButton.addEventListener("click", markAutoPass);
      }

      if (autoTurnInput) {
        autoTurnInput.addEventListener("input", function () {
          autoTurnDistanceMm = parseInt(autoTurnInput.value, 10) || 150;
          if (autoTurnValue) {
            autoTurnValue.textContent = autoTurnDistanceMm;
          }
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
          setAutoMode(false);
        }
      });

      window.addEventListener("blur", function () {
        setAutoMode(false);
      });

      window.addEventListener("beforeunload", function () {
        setAutoMode(false);
      });

      setActiveButton("stop");
      setCameraVisible(cam1Card, toggleCam1Button, "CAM 1", true);
      setCameraVisible(cam2Card, toggleCam2Button, "CAM 2", false);
      setCam1Mirror(true);
      setTapDriveMode(false);
      setTapCalibrateMode(false);
      setAutoPanelVisible(false);
      updateAutoLogState();
      resetMap();
      updateTof();
      updateCam2Color();
      setInterval(updateTof, 250);
      setInterval(updateCam2Color, 150);
      autoTimer = setInterval(autoTick, 220);
    })();
  </script>
</body>
</html>
)rawliteral";

