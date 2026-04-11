
    (function () {
      var speedInput = document.getElementById("speed");
      var speedValue = document.getElementById("speedValue");
      var speedForm = document.querySelector(".speed-form");
      var stopButton = document.getElementById("stop");
      var holdButtons = [
        { id: "up", dir: "forward" },
        { id: "down", dir: "backward" },
        { id: "left", dir: "left" },
        { id: "right", dir: "right" }
      ];
      var activeDirection = "stop";
      var speedTimer = null;

      function send(path) {
        if (window.fetch) {
          fetch(path, { cache: "no-store" }).catch(function () {});
        } else {
          var beacon = new Image();
          beacon.src = path + "&_ts=" + Date.now();
        }
      }

      function setActiveButton(id) {
        var buttons = document.querySelectorAll(".pad .ctrl-btn");
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
        if (activeDirection === dir) {
          return;
        }
        activeDirection = dir;
        setActiveButton(buttonId);
        send("/move?dir=" + dir);
      }

      function stop() {
        if (activeDirection === "stop") {
          return;
        }
        activeDirection = "stop";
        setActiveButton("stop");
        send("/move?dir=stop");
      }

      function bindHold(buttonId, dir) {
        var button = document.getElementById(buttonId);
        if (!button) {
          return;
        }

        button.addEventListener("pointerdown", function (e) {
          e.preventDefault();
          move(dir, buttonId);
        });

        var releaseEvents = ["pointerup", "pointercancel", "pointerleave"];
        for (var i = 0; i < releaseEvents.length; i++) {
          (function (eventName) {
            button.addEventListener(eventName, function (e) {
              e.preventDefault();
              stop();
            });
          })(releaseEvents[i]);
        }

        button.addEventListener("click", function (e) {
          e.preventDefault();
        });
      }

      for (var j = 0; j < holdButtons.length; j++) {
        bindHold(holdButtons[j].id, holdButtons[j].dir);
      }

      if (stopButton) {
        stopButton.addEventListener("click", function (e) {
          e.preventDefault();
          stop();
        });
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

      if (speedForm) {
        speedForm.addEventListener("submit", function (e) {
          e.preventDefault();
          if (speedInput) {
            send("/speed?value=" + encodeURIComponent(speedInput.value));
          }
        });
      }

      document.addEventListener("visibilitychange", function () {
        if (document.hidden) {
          stop();
        }
      });

      window.addEventListener("beforeunload", function () {
        stop();
      });
    })();
  
