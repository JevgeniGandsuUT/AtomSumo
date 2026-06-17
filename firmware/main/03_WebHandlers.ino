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
  } else if (dir == "forward-left") {
    applyDirection(DIR_FORWARD_LEFT);
  } else if (dir == "forward-right") {
    applyDirection(DIR_FORWARD_RIGHT);
  } else if (dir == "backward-left") {
    applyDirection(DIR_BACKWARD_LEFT);
  } else if (dir == "backward-right") {
    applyDirection(DIR_BACKWARD_RIGHT);
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

void handleArcTurn() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  int value = constrain(server.arg("value").toInt(), 0, 255);
  arcTurnInnerTrim = static_cast<uint8_t>(value);

  if (currentDirection == DIR_FORWARD_LEFT ||
      currentDirection == DIR_FORWARD_RIGHT ||
      currentDirection == DIR_BACKWARD_LEFT ||
      currentDirection == DIR_BACKWARD_RIGHT) {
    applyDirection(currentDirection);
  }

  server.send(200, "text/plain", String(arcTurnInnerTrim));
}

void handleTof() {
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");

  if (!tofReady) {
    String json = String("{\"ready\":false,\"valid\":false,\"mm\":0,\"status\":-1,\"scan\":\"") + i2cScanResult +
                  "\",\"sda\":" + String(activeTofSda) +
                  ",\"scl\":" + String(activeTofScl) + "}";
    server.send(200, "application/json", json);
    return;
  }

  String json = String("{\"ready\":true,\"valid\":") + (lastTofValid ? "true" : "false") +
                ",\"mm\":" + String(lastTofValid ? lastTofMm : 0) +
                ",\"status\":" + String(lastTofStatus) +
                ",\"scan\":\"" + i2cScanResult +
                "\",\"sda\":" + String(activeTofSda) +
                ",\"scl\":" + String(activeTofScl) + "}";
  server.send(200, "application/json", json);
}

String scanI2cBus() {
  String result = "";
  uint8_t count = 0;

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();
    if (error == 0) {
      if (count > 0) {
        result += ",";
      }
      if (address < 16) {
        result += "0x0";
      } else {
        result += "0x";
      }
      result += String(address, HEX);
      count++;
    }
  }

  if (count == 0) {
    return "none";
  }
  return result;
}

bool i2cScanHasAddress(const String &scanResult, const char *address) {
  return scanResult.indexOf(address) >= 0;
}

