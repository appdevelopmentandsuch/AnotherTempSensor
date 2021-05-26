#include "config.h"
#include "http.h"
#include "server_config.h"
#include "utils.h"
#include "wifi.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer configServer(80);

bool validExpectedService(DynamicJsonDocument doc, int service) {
    return doc.containsKey(JSON_SETTING_SERVICE_CONFIG) && 
        doc[JSON_SETTING_SERVICE_CONFIG] == service;
}

bool validService(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_SETTING_SERVICE_CONFIG) && 
        !(doc[JSON_SETTING_SERVICE_CONFIG] > OPTION_MQTT || doc[JSON_SETTING_SERVICE_CONFIG] < OPTION_REST);
}

bool validWiFiConfig(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_SETTING_WIFI_SSID) &&
        doc.containsKey(JSON_SETTING_WIFI_PASS);
}

bool validRESTConfig(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_SETTING_REST_USER) &&
        doc.containsKey(JSON_SETTING_REST_PASS) &&
        validExpectedService(doc, OPTION_REST);
}

bool validMQTTConfig(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_SETTING_MQTT_BROKER) &&
        doc.containsKey(JSON_SETTING_MQTT_PORT) &&
        doc.containsKey(JSON_SETTING_MQTT_USER) &&
        doc.containsKey(JSON_SETTING_MQTT_PASS) &&
        validExpectedService(doc, OPTION_MQTT);
}

bool setDefaultConfig() {
    return setDefaultServerConfig();
}

void resetConfig() {
    bool stored = setDefaultConfig();

    if(stored) {
        WiFi.disconnect();

        while(isConnected()) {
            delay(100);
        }

        ESP.reset();
    }
}

void handleConfig() {
    if (configServer.hasArg("plain") == false){
        configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON,  HTTP_MISSING_JSON_BODY);
        return;
    }

    DynamicJsonDocument doc(DOC_SIZE);

    bool result = deserializeJsonDoc(doc, configServer.arg("plain"));
    if(!validService(doc)) {
        configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_SERVICE);
    } else {
        int serviceConfig = doc[JSON_SETTING_SERVICE_CONFIG];

        if (!validWiFiConfig(doc)) {
            configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_WIFI_CONFIG);
        } else if(serviceConfig == OPTION_REST && !validRESTConfig(doc)) {
            configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_REST_CONFIG);
        } else if(serviceConfig == OPTION_MQTT && !validMQTTConfig(doc)) {
            configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_MQTT_CONFIG);
        } else {
            configServer.send(HTTP_OK, HTTP_TYPE_JSON, HTTP_SUCCESS);
            
            storeConfig(doc);

            WiFi.softAPdisconnect(true);

            delay(5000);

            ESP.reset();
        }
    }
}

void handleConfigServerSetup() {
    configServer.on("/api/config/", HTTP_POST, handleConfig);
    configServer.begin();
}

void handleConfigServer() {
    configServer.handleClient();  
}
