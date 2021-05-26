#include "mqtt.h"
#include "secrets.h"
#include "sensor_dht.h"
#include "server_config.h"
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include "utils.h"

const String humidityTopic  = "humidity";
const String tempTopic  = "temperature";


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// MQTT Methods
void handleMQTTSetup() {
  const char* mqttUser = settings["mqttUser"];
  const char* mqttPass = settings["mqttPass"];
  if(!(mqttUser == "" || mqttPass == "")) {
    mqttClient.setUsernamePassword(mqttUser, mqttPass);
  }

  const char* mqttBroker = settings["mqttBroker"];
  int mqttPort = settings["mqttPort"];
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