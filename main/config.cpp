#include "config.h"
#include "server_config.h"
#include "utils.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer configServer(80);

uint8_t currentConfig = OPTION_CONFIG;

void resetConfig() {
    writeString(ADDRESS_SSID, "");
    writeString(ADDRESS_PASS, "");
    writeString(ADDRESS_MQTT_BROKER, "");
    writeString(ADDRESS_MQTT_USER, "");
    writeString(ADDRESS_MQTT_PASS, "");
    writeString(ADDRESS_REST_USER, "");
    writeString(ADDRESS_REST_PASS, "");
    writeInt(ADDRESS_SERVICE_CONFIG, OPTION_CONFIG);

    ESP.reset();
}

void setWifiConfig(const char* ssid, const char* pass) {
    writeString(ADDRESS_SSID, ssid);
    writeString(ADDRESS_PASS, pass);
}

void setMQTTConfig(const char* mqttBroker, const char* mqttUser, const char* mqttPass) {
    writeString(ADDRESS_MQTT_BROKER, mqttBroker);
    writeString(ADDRESS_MQTT_USER, mqttUser);
    writeString(ADDRESS_MQTT_PASS, mqttPass);
}

void setRESTConfig(const char* user, const char* pass) {
    writeString(ADDRESS_REST_USER, user);
    writeString(ADDRESS_REST_PASS, pass);
}

void setServiceConfig(int service) {
    writeInt(ADDRESS_SERVICE_CONFIG, service);
}

void handleConfig() {
    // if (configServer.hasArg("plain")== false){ //Check if body received
    //     configServer.send(200, "text/plain", "Body not received");
    //     return;
    // }

    DynamicJsonDocument doc(1024);

    deserializeJson(doc, configServer.arg("plain"));
    const char* ssid = doc["ssid"];
    const char* pass = doc["pass"];

    setWifiConfig(ssid, pass);
    
    const char* mqttBroker = doc["mqttBroker"];
    const char* mqttUser = doc["mqttUser"];
    const char* mqttPass = doc["mqttPass"];

    setMQTTConfig(mqttBroker, mqttUser, mqttPass);

    const char* restUser = doc["restUser"];
    const char* restPass = doc["restPass"];

    setRESTConfig(restUser, restPass);
    int config = doc["service"];

    if(config > OPTION_MQTT || config < OPTION_REST) {
        config = OPTION_CONFIG;
        setServiceConfig(config);
        configServer.send(400, "text/plain", "Invalid service, must select 0 for REST, or 1 for MQTT, restarting...");
    } else {
        setServiceConfig(config);
        configServer.send(200, "text/json", "{success:true}");
        delay(500);
        ESP.reset();
    }
}

void handleConfigServerSetup() {
    configServer.on("/api/config/", HTTP_POST, handleConfig);
    configServer.begin();
}

void handleConfigServer() {
    configServer.handleClient();  
}
