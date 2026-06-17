#include <WebServer.h>
#include <DNSServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

//#define PWMA 22
//#define AIN1 19
//#define AIN2 23
//
#define PWMA 23
#define AIN1 19
#define AIN2 22



#define PWMB 25
#define BIN1 21
#define BIN2 33



#define ATOM_LED_PIN 27
#define TOF_PRIMARY_SDA 32
#define TOF_PRIMARY_SCL 26
#define TOF_ALT_SDA 26
#define TOF_ALT_SCL 32

const char *AP_SSID = "Robot-Control";
const char *AP_PASSWORD = "robot123";

WebServer server(80);
DNSServer dnsServer;
Adafruit_VL53L0X tofSensor;
const uint16_t DNS_PORT = 53;

enum MotionDirection {
  DIR_STOP,
  DIR_FORWARD,
  DIR_BACKWARD,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_FORWARD_LEFT,
  DIR_FORWARD_RIGHT,
  DIR_BACKWARD_LEFT,
  DIR_BACKWARD_RIGHT
};

MotionDirection currentDirection = DIR_STOP;
uint8_t motorSpeed = 180;
uint8_t arcTurnInnerTrim = 55;
bool standbyBlinkOn = false;
bool tofReady = false;
String i2cScanResult = "";
uint8_t activeTofSda = TOF_PRIMARY_SDA;
uint8_t activeTofScl = TOF_PRIMARY_SCL;
unsigned long lastStandbyBlinkMs = 0;
unsigned long lastTofReadMs = 0;
uint16_t lastTofMm = 0;
uint8_t lastTofStatus = 255;
bool lastTofValid = false;

// Forward on many small robots can overload one side at max PWM.
// Keep A slightly prioritized and give both motors a short start boost.
const uint8_t MOTOR_A_FORWARD_TRIM = 255;
const uint8_t MOTOR_B_FORWARD_TRIM = 255;
const uint8_t MOTOR_A_BACKWARD_TRIM = 255;
const uint8_t MOTOR_B_BACKWARD_TRIM = 255;
const uint32_t MOTOR_PWM_FREQ_HZ = 22000;
const uint8_t MOTOR_PWM_RESOLUTION_BITS = 8;
const uint8_t START_BOOST_EXTRA_PWM = 35;
const uint8_t ARC_TURN_MIN_INNER_PWM = 75;
const uint16_t START_BOOST_MS = 55;
const uint16_t STANDBY_BLINK_INTERVAL_MS = 450;
const uint8_t LED_BRIGHTNESS = 110;
const uint16_t TOF_READ_INTERVAL_MS = 200;
const uint16_t TOF_I2C_TIMEOUT_MS = 50;
const bool MOTOR_A_FORWARD_PWM_INVERTED = false;
const bool MOTOR_A_BACKWARD_PWM_INVERTED = false;
const bool MOTOR_A_LEFT_TURN_PWM_INVERTED = false;
const bool MOTOR_A_RIGHT_TURN_PWM_INVERTED = false;

String scanI2cBus();
bool i2cScanHasAddress(const String &scanResult, const char *address);
void setupMotorPwm();
void applyDirection(MotionDirection dir);
void updateStandbyLedBlink();
void updateTofReading();
void handleRoot();
void handleMove();
void handleSpeed();
void handleArcTurn();
void handleTof();
void handleCaptiveProbe();
void handleNotFound();

void setup() {
  Serial.begin(115200);

  Wire.begin(TOF_PRIMARY_SDA, TOF_PRIMARY_SCL);
  Wire.setTimeOut(TOF_I2C_TIMEOUT_MS);
  i2cScanResult = scanI2cBus();
  activeTofSda = TOF_PRIMARY_SDA;
  activeTofScl = TOF_PRIMARY_SCL;
  Serial.printf("I2C scan on SDA=%d SCL=%d: %s\n", activeTofSda, activeTofScl, i2cScanResult.c_str());

  if (!i2cScanHasAddress(i2cScanResult, "0x29")) {
    Wire.end();
    Wire.begin(TOF_ALT_SDA, TOF_ALT_SCL);
    Wire.setTimeOut(TOF_I2C_TIMEOUT_MS);
    String altScanResult = scanI2cBus();
    Serial.printf("I2C scan on SDA=%d SCL=%d: %s\n", TOF_ALT_SDA, TOF_ALT_SCL, altScanResult.c_str());
    if (i2cScanHasAddress(altScanResult, "0x29")) {
      activeTofSda = TOF_ALT_SDA;
      activeTofScl = TOF_ALT_SCL;
      i2cScanResult = altScanResult;
    } else {
      Wire.end();
      Wire.begin(TOF_PRIMARY_SDA, TOF_PRIMARY_SCL);
      Wire.setTimeOut(TOF_I2C_TIMEOUT_MS);
    }
  }

  tofReady = tofSensor.begin(0x29, false, &Wire);
  Serial.printf("VL53L0X on SDA=%d SCL=%d: %s\n", activeTofSda, activeTofScl, tofReady ? "OK" : "NOT FOUND");

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(ATOM_LED_PIN, OUTPUT);
  setupMotorPwm();

  applyDirection(DIR_STOP);

  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(AP_SSID, AP_PASSWORD, 1, 0, 8);

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  server.on("/speed", HTTP_GET, handleSpeed);
  server.on("/arc", HTTP_GET, handleArcTurn);
  server.on("/tof", HTTP_GET, handleTof);
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
  updateTofReading();
}
