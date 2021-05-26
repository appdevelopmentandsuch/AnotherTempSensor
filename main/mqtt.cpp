#include "mqtt.h"
#include "sensor_dht.h"
#include "constants.h"
#include "utils.h"
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <string.h>

const String humidityTopic  = "humidity";
const String tempTopic  = "temperature";


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// MQTT Methods
void handleMQTTSetup() {
  const char* mqttUser = settings[JSON_KEY_MQTT_USER];
  const char* mqttPass = settings[JSON_KEY_MQTT_PASS];
  if(!(mqttUser == "" || mqttPass == "")) {
    mqttClient.setUsernamePassword(mqttUser, mqttPass);
  }

  const char* mqttBroker = settings[JSON_KEY_MQTT_BROKER];
  int mqttPort = settings[JSON_KEY_MQTT_PORT];
  if (!mqttClient.connect(mqttBroker, mqttPort)) {
    delay(SETUP_DELAY);
    while (1);
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

  sendMqttMessage(buildTopic(tempTopic), String(t));
  sendMqttMessage(buildTopic(humidityTopic), String(h));
  delay(LOOP_DELAY);
}