#include "DHT.h"
#include "secrets.h"
#include "server_config.h"
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

char mqttPass[] = SECRET_MQTT_PASS;
char mqttUser[] = SECRET_MQTT_USER;
char pass[] = SECRET_PASS;
char ssid[] = SECRET_SSID; 

const char broker[] = MQTT_BROKER;
const char humidityTopic[]  = "humidity/bath-room";
const char tempTopic[]  = "temp/bath-room";
int        port     = MQTT_PORT;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


void setup() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_SETUP_DELAY);
  }
  
  dht.begin();

  mqttClient.setUsernamePassword(mqttUser, mqttPass);

  if (!mqttClient.connect(broker, port)) {
    while (1);
  }
}

void loop() {
 
  if (WiFi.status() == WL_CONNECTED) {
    mqttClient.poll();
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    mqttClient.beginMessage(tempTopic);
    mqttClient.print(t);
    mqttClient.endMessage();

    mqttClient.beginMessage(humidityTopic);
    mqttClient.print(h);
    mqttClient.endMessage();
 
  } else {
    Serial.println("Error in WiFi connection");
  }
 delay(LOOP_DELAY);
}
