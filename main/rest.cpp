#include "rest.h"
#include "secrets.h"
#include "sensor_dht.h"
#include "server_config.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <string.h>

ESP8266WebServer server(80);

char server_user[] = SECRET_SERVER_USERNAME;
char server_pass[] = SECRET_SERVER_PASSWORD;

String version = VERSION;

// HTTP Server Methods
void handleServer() {
  server.handleClient();  
}

void checkAuth() {
  if (!server.authenticate(server_user, server_pass)) {
        return server.requestAuthentication();
  }
}

void sendJSONResponse(DynamicJsonDocument doc) {
  String response = "";
  serializeJson(doc, response);

  server.send(200, "application/json", response);
}

void handleReadAll()
{
  checkAuth();

  DynamicJsonDocument doc(1024);

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

  DynamicJsonDocument doc(1024);

  float temperature = dht.readTemperature();

  doc["temperature"] = temperature;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleReadHumidity()
{
  checkAuth();

  DynamicJsonDocument doc(1024);

  float humidity = dht.readHumidity();

  doc["humidity"] = humidity;
  doc["identifier"] = WiFi.macAddress();

  sendJSONResponse(doc);
}

void handleGetInfo()
{
  checkAuth();

  DynamicJsonDocument doc(1024);

  doc["identifier"] = WiFi.macAddress();
  doc["version"] = version;

  sendJSONResponse(doc);
}

void handleServerSetup() {
  server.on("/api/read/all/", handleReadAll);
  server.on("/api/read/temperature/", handleReadTemperature);
  server.on("/api/read/humidity/", handleReadHumidity);
  server.on("/api/info/", handleGetInfo);
  server.begin();
}
