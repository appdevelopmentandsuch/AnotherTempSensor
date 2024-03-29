#include "rest.h"
#include "sensor_dht.h"
#include "constants.h"
#include "utils.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <string.h>

ESP8266WebServer restServer(WEB_SERVER_PORT);

String version = VERSION;

// HTTP Server Methods
void handleServer() {
  restServer.handleClient();  
}

void checkAuth() {
  DynamicJsonDocument settings = loadConfig();
  const char* serverUser = settings[JSON_KEY_REST_USER];
  const char* serverPass = settings[JSON_KEY_REST_PASS];
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

  doc[JSON_KEY_TEMPERATURE] = temperature;
  doc[JSON_KEY_HUMIDITY] = humidity;
  doc[JSON_KEY_IDENTIFIER] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadTemperature() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float temperature = dht.readTemperature();

  doc[JSON_KEY_TEMPERATURE] = temperature;
  doc[JSON_KEY_IDENTIFIER] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadHumidity() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float humidity = dht.readHumidity();

  doc[JSON_KEY_HUMIDITY] = humidity;
  doc[JSON_KEY_IDENTIFIER] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleGetInfo() {
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  doc[JSON_KEY_IDENTIFIER] = WiFi.macAddress();
  doc[JSON_KEY_VERSION] = version;

  sendJSONResponse(doc);
}

void handleServerSetup() {
  restServer.on(ENDPOINT_READ_ALL, handleReadAll);
  restServer.on(ENDPOINT_READ_TEMPERATURE, handleReadTemperature);
  restServer.on(ENDPOINT_READ_HUMIDITY, handleReadHumidity);
  restServer.on(ENDPOINT_INFO, handleGetInfo);
  restServer.begin();
}
