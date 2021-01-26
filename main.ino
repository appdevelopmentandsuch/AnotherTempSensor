#include "DHT.h"
#include "secrets.h"
#include "server_config.h"
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <string.h>

char mqttPass[] = SECRET_MQTT_PASS;
char mqttUser[] = SECRET_MQTT_USER;
char pass[] = SECRET_PASS;
char ssid[] = SECRET_SSID;

String deviceLocation = DEVICE_LOCATION;

const char broker[] = MQTT_BROKER;
const String humidityTopic  = "humidity";
const String tempTopic  = "temp";
int        port     = MQTT_PORT;

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


void setup() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_SETUP_DELAY);
  }
  
  dht.begin();

  if(!(mqttUser == "" || mqttPass == "")) {
    mqttClient.setUsernamePassword(mqttUser, mqttPass);
  }

  if (!mqttClient.connect(broker, port)) {
    while (1);
  }
}

String buildTopic(String root) {
  return root + "/" + deviceLocation;
}

void sendMqttMessage(String topic, String message) {
    mqttClient.beginMessage(topic);
    mqttClient.print(message);
    mqttClient.endMessage();
}

void loop() {
 
  if (WiFi.status() == WL_CONNECTED) {
    mqttClient.poll();
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    sendMqttMessage(buildTopic(tempTopic), String(t));
    sendMqttMessage(buildTopic(humidityTopic), String(h));
  }

 delay(LOOP_DELAY);
}
