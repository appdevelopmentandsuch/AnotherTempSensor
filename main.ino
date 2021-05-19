#include "DHT.h"
#include "secrets.h"
#include "server_config.h"
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <string.h>

char mqttPass[] = SECRET_MQTT_PASS;
char mqttUser[] = SECRET_MQTT_USER;
char pass[] = SECRET_PASS;
char ssid[] = SECRET_SSID;
char server_user[] = SECRET_SERVER_USERNAME;
char server_pass[] = SECRET_SERVER_PASSWORD;

const char broker[] = MQTT_BROKER;
const String humidityTopic  = "humidity";
const String tempTopic  = "temperature";
int        port     = MQTT_PORT;

DHT dht(DHT_PIN, DHT_TYPE);
ESP8266WebServer server(80);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
String identifier = "";

void setup() {
  Serial.begin(115200);

  handleWifiSetup();

  // Uncomment either handleMQTTSetup or handleServerSetup, depending on how you want your data.
  handleServerSetup();
  // handleMQTTSetup();

  dht.begin();

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Uncomment server.handleClient if handleServerSetup was uncommented, or handleMQTT if handleMQTTSetup was uncommented
    server.handleClient();  
    // handleMQTT();
  }
}

// Wifi Connection Methods
void handleWifiSetup() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(SETUP_DELAY);
  }

  Serial.println("WiFi connected.");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  identifier = WiFi.macAddress();
}

// HTTP Server Methods
void handleServerSetup() {
  server.on("/read", handleRead);
  server.begin();
}

void handleRead()
{
  if (!server.authenticate(server_user, server_pass)) {
        return server.requestAuthentication();
  }
  DynamicJsonDocument doc(1024);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["identifier"] = identifier;

  String response = "";
  serializeJson(doc, response);

  server.send(200, "application/json", response);
}

// MQTT Methods
void handleMQTTSetup() {
  if(!(mqttUser == "" || mqttPass == "")) {
    mqttClient.setUsernamePassword(mqttUser, mqttPass);
  }

  if (!mqttClient.connect(broker, port)) {
    delay(SETUP_DELAY);
    while (1);
  }
}

String buildTopic(String root) {
  return root + "/" + identifier;
}

void sendMqttMessage(String topic, String message) {
    mqttClient.beginMessage(topic);
    mqttClient.print(message);
    mqttClient.endMessage();
}

void handleMQTT() {
  mqttClient.poll();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  sendMqttMessage(buildTopic(tempTopic), String(t));
  sendMqttMessage(buildTopic(humidityTopic), String(h));
  delay(LOOP_DELAY);
}
