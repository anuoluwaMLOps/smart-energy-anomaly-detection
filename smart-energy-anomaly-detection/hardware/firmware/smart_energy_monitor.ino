#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi Configuration
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
const char* mqtt_server = "mqtt-broker.com";
const char* mqtt_user = "mqtt_user";
const char* mqtt_password = "mqtt_password";

// MQTT Topics
const char* topic_voltage = "home/energy/voltage";
const char* topic_current = "home/energy/current";
const char* topic_power = "home/energy/power";
const char* topic_energy = "home/energy/consumption";

// Pin Configuration
const int VOLTAGE_PIN = 35;  // ADC1_7
const int CURRENT_PIN = 34;  // ADC1_6
const int LED_POWER = 4;
const int LED_WIFI = 2;
const int LED_ALERT = 5;

// Global Variables
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMeasurement = 0;
float totalEnergy = 0.0;  // in kWh

// Function Prototypes
void setup_wifi();
void callback(char* topic, byte* message, unsigned int length);
void reconnect();
void measureEnergy();
float readVoltage();
float readCurrent();
float calculatePower(float voltage, float current);

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // LED Setup
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);
  
  digitalWrite(LED_POWER, HIGH);  // Power LED ON
  
  // WiFi Connection
  setup_wifi();
  
  // MQTT Setup
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("WiFi connected! IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_WIFI, HIGH);
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  // Handle MQTT messages (for future use - control features)
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected!");
      digitalWrite(LED_WIFI, HIGH);
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      digitalWrite(LED_WIFI, LOW);
      delay(5000);
    }
  }
}

float readVoltage() {
  int rawVoltage = analogRead(VOLTAGE_PIN);
  float voltage = (rawVoltage / 4095.0) * 3.3;
  voltage = voltage * (10 + 20) / 20.0;
  return voltage;
}

float readCurrent() {
  int rawCurrent = analogRead(CURRENT_PIN);
  float voltage = (rawCurrent / 4095.0) * 3.3;
  float adcVoltage = voltage * 30 / 20.0;
  float current = (adcVoltage - 2.5) / 0.185;
  return abs(current);
}

float calculatePower(float voltage, float current) {
  float power_factor = 0.95;
  return voltage * current * power_factor;
}

void measureEnergy() {
  float voltage = readVoltage();
  float current = readCurrent();
  float power = calculatePower(voltage, current);
  
  float energyDelta = (power / 1000.0) * (10.0 / 3600.0);
  totalEnergy += energyDelta;
  
  StaticJsonDocument<200> doc;
  doc["voltage"] = voltage;
  doc["current"] = current;
  doc["power"] = power;
  doc["energy"] = totalEnergy;
  doc["timestamp"] = millis();
  
  char buffer[200];
  serializeJson(doc, buffer);
  
  client.publish(topic_voltage, String(voltage).c_str());
  client.publish(topic_current, String(current).c_str());
  client.publish(topic_power, String(power).c_str());
  client.publish(topic_energy, String(totalEnergy).c_str());
  
  Serial.print("V:");
  Serial.print(voltage);
  Serial.print(" I:");
  Serial.print(current);
  Serial.print(" P:");
  Serial.print(power);
  Serial.print(" E:");
  Serial.println(totalEnergy);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_WIFI, LOW);
    setup_wifi();
  } else {
    digitalWrite(LED_WIFI, HIGH);
  }
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if (millis() - lastMeasurement >= 10000) {
    measureEnergy();
    lastMeasurement = millis();
  }
}
