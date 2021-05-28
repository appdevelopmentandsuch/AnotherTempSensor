#include "mqtt.h"
#include "sensor_dht.h"
#include "constants.h"
#include "utils.h"
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <string.h>

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// MQTT Methods
void handleMQTTSetup() {
  DynamicJsonDocument settings = loadConfig();
  const char* mqttUser = settings[JSON_KEY_MQTT_USER];
  const char* mqttPass = settings[JSON_KEY_MQTT_PASS];

  if(!(mqttUser == "" || mqttPass == "")) {
    mqttClient.setUsernamePassword(mqttUser, mqttPass);
  }

  const char* mqttBroker = settings[JSON_KEY_MQTT_BROKER];
  int mqttPort = settings[JSON_KEY_MQTT_PORT];
  int tries = 0;

  while (!mqttClient.connect(mqttBroker, mqttPort) && tries < MQTT_MAX_TRIES) {
    delay(SETUP_DELAY);
    tries += 1;
  }

  if(!mqttClient.connected() || tries > MQTT_MAX_TRIES) {
    ESP.reset();
  }
}

String buildTopic(String root) {
  return root + "/" + WiFi.macAddress();
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

  sendMqttMessage(buildTopic(TOPIC_TEMPERATURE), String(t));
  sendMqttMessage(buildTopic(TOPIC_HUMIDITY), String(h));
  delay(LOOP_DELAY);
}