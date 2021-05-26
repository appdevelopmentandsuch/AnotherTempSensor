#include "http.h"
#include "rest.h"
#include "sensor_dht.h"
#include "server_config.h"
#include "utils.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <string.h>

ESP8266WebServer restServer(80);

String version = VERSION;

// HTTP Server Methods
void handleServer() {
  restServer.handleClient();  
}

void checkAuth() {
  const char* serverUser = settings[JSON_SETTING_REST_USER];
  const char* serverPass = settings[JSON_SETTING_REST_PASS];
  if (!restServer.authenticate(serverUser, serverPass)) {
        return restServer.requestAuthentication();
  }
}

void sendJSONResponse(DynamicJsonDocument doc) {
  String response = "";
  serializeJson(doc, response);

  restServer.send(HTTP_OK, HTTP_TYPE_JSON, response);
}

void handleReadAll() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadTemperature() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float temperature = dht.readTemperature();

  doc["temperature"] = temperature;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadHumidity() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float humidity = dht.readHumidity();

  doc["humidity"] = humidity;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleGetInfo() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  doc["identifier"] = WiFi.macAddress();
  doc["version"] = version;

  sendJSONResponse(doc);
}

void handleServerSetup() {
  restServer.on("/api/read/all/", handleReadAll);
  restServer.on("/api/read/temperature/", handleReadTemperature);
  restServer.on("/api/read/humidity/", handleReadHumidity);
  restServer.on("/api/info/", handleGetInfo);
  restServer.begin();
}
