/*
  ESP32 Smart Aquarium Monitoring and Feeding System

  Recreated portfolio version based on an older student project idea.
  Features:
  - Scheduled fish feeding using servo motor
  - Manual feeding using local WiFi webpage
  - Temperature and pH sensor reading using analogue inputs
  - Basic alert LED for unsafe values

  Note:
  pH sensor values must be calibrated with real buffer solutions.
  The formula used here is only a starting point for demonstration.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// -------- WiFi Details --------
// Update before uploading to ESP32
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// -------- Pin Configuration --------
const int SERVO_PIN = 18;
const int TEMP_SENSOR_PIN = 34;
const int PH_SENSOR_PIN = 35;
const int ALERT_LED_PIN = 2;

// -------- Feeder Settings --------
Servo feederServo;

const int SERVO_CLOSED_ANGLE = 0;
const int SERVO_OPEN_ANGLE = 75;

const unsigned long FEED_INTERVAL_MS = 12UL * 60UL * 60UL * 1000UL; // 12 hours
unsigned long lastFeedTime = 0;

// -------- Sensor Thresholds --------
const float MIN_SAFE_TEMP = 22.0;
const float MAX_SAFE_TEMP = 30.0;

const float MIN_SAFE_PH = 6.5;
const float MAX_SAFE_PH = 8.0;

// -------- pH Calibration Values --------
// These values should be changed after real calibration.
const float PH_SLOPE = -5.70;
const float PH_OFFSET = 21.34;

// -------- Web Server --------
WebServer server(80);

// -------- Function Declarations --------
float readTemperatureC();
float readPH();
void feedFish();
void checkAlerts(float temperatureC, float phValue);
void handleRoot();
void handleFeedNow();

void setup() {
  Serial.begin(115200);

  pinMode(ALERT_LED_PIN, OUTPUT);
  digitalWrite(ALERT_LED_PIN, LOW);

  feederServo.attach(SERVO_PIN);
  feederServo.write(SERVO_CLOSED_ANGLE);

  Serial.println();
  Serial.println("ESP32 Smart Aquarium System Starting...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/feed", handleFeedNow);
  server.begin();

  Serial.println("Web server started.");
  Serial.println("Open the ESP32 IP address in a browser.");
}

void loop() {
  server.handleClient();

  float temperatureC = readTemperatureC();
  float phValue = readPH();

  checkAlerts(temperatureC, phValue);

  unsigned long currentTime = millis();

  if (currentTime - lastFeedTime >= FEED_INTERVAL_MS) {
    Serial.println("Scheduled feeding started.");
    feedFish();
    lastFeedTime = currentTime;
  }

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" C | pH: ");
  Serial.println(phValue);

  delay(3000);
}

float readTemperatureC() {
  /*
    Example assumes an analogue temperature sensor like LM35.
    LM35 gives 10 mV per degree Celsius.

    ESP32 ADC range: 0 - 4095
    Voltage range: around 0 - 3.3 V
  */

  int rawValue = analogRead(TEMP_SENSOR_PIN);
  float voltage = rawValue * (3.3 / 4095.0);
  float temperatureC = voltage * 100.0;

  return temperatureC;
}

float readPH() {
  /*
    pH sensors need calibration.
    This is a basic conversion for demo/portfolio purposes.

    For a real aquarium:
    - Calibrate using pH 4, pH 7 and pH 10 buffer solutions
    - Update PH_SLOPE and PH_OFFSET
  */

  int rawValue = analogRead(PH_SENSOR_PIN);
  float voltage = rawValue * (3.3 / 4095.0);
  float phValue = PH_SLOPE * voltage + PH_OFFSET;

  return phValue;
}

void feedFish() {
  Serial.println("Opening feeder...");
  feederServo.write(SERVO_OPEN_ANGLE);
  delay(1000);

  Serial.println("Closing feeder...");
  feederServo.write(SERVO_CLOSED_ANGLE);
  delay(500);

  Serial.println("Feeding completed.");
}

void checkAlerts(float temperatureC, float phValue) {
  bool temperatureUnsafe = temperatureC < MIN_SAFE_TEMP || temperatureC > MAX_SAFE_TEMP;
  bool phUnsafe = phValue < MIN_SAFE_PH || phValue > MAX_SAFE_PH;

  if (temperatureUnsafe || phUnsafe) {
    digitalWrite(ALERT_LED_PIN, HIGH);
  } else {
    digitalWrite(ALERT_LED_PIN, LOW);
  }
}

void handleRoot() {
  float temperatureC = readTemperatureC();
  float phValue = readPH();

  String html = "";
  html += "<!DOCTYPE html><html>";
  html += "<head><title>Smart Aquarium</title></head>";
  html += "<body style='font-family: Arial;'>";
  html += "<h2>ESP32 Smart Aquarium Monitoring System</h2>";
  html += "<p><b>Temperature:</b> " + String(temperatureC, 2) + " &deg;C</p>";
  html += "<p><b>pH Value:</b> " + String(phValue, 2) + "</p>";
  html += "<p><b>Safe Temperature Range:</b> " + String(MIN_SAFE_TEMP) + " - " + String(MAX_SAFE_TEMP) + " &deg;C</p>";
  html += "<p><b>Safe pH Range:</b> " + String(MIN_SAFE_PH) + " - " + String(MAX_SAFE_PH) + "</p>";
  html += "<p><a href='/feed'><button style='padding:10px;'>Feed Fish Now</button></a></p>";
  html += "<p>Refresh the page to update readings.</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleFeedNow() {
  Serial.println("Manual feeding requested from web page.");
  feedFish();
  lastFeedTime = millis();

  server.sendHeader("Location", "/");
  server.send(303);
}
