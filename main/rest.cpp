#include "rest.h"
#include "secrets.h"
#include "sensor_dht.h"
#include "server_config.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <string.h>

ESP8266WebServer restServer(80);

char server_user[] = SECRET_SERVER_USERNAME;
char server_pass[] = SECRET_SERVER_PASSWORD;

String version = VERSION;

// HTTP Server Methods
void handleServer() {
  restServer.handleClient();  
}

void checkAuth() {
  if (!restServer.authenticate(server_user, server_pass)) {
        return restServer.requestAuthentication();
  }
}

void sendJSONResponse(DynamicJsonDocument doc) {
  String response = "";
  serializeJson(doc, response);

  restServer.send(200, "application/json", response);
}

void handleReadAll()
{
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadTemperature()
{
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float temperature = dht.readTemperature();

  doc["temperature"] = temperature;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadHumidity()
{
  checkAuth();

  DynamicJsonDocument doc(DOC_SIZE);

  float humidity = dht.readHumidity();

  doc["humidity"] = humidity;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleGetInfo()
{
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
