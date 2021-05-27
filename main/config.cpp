#include "config.h"
#include "http.h"
#include "constants.h"
#include "utils.h"
#include "wifi.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer configServer(80);

bool validExpectedService(DynamicJsonDocument doc, int service) {
    if (doc.containsKey(JSON_KEY_SERVICE_CONFIG)) {
        int config = doc[JSON_KEY_SERVICE_CONFIG];
        return config == service;
    }
    return false; 
}

bool validService(DynamicJsonDocument doc) {
    if (doc.containsKey(JSON_KEY_SERVICE_CONFIG)) {
        int config = doc[JSON_KEY_SERVICE_CONFIG];
        return config <= OPTION_MQTT && 
        config >= OPTION_REST;
    }
    return false; 
}

bool validWiFiConfig(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_KEY_WIFI_SSID) &&
        doc.containsKey(JSON_KEY_WIFI_PASS);
}

bool validRESTConfig(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_KEY_REST_USER) &&
        doc.containsKey(JSON_KEY_REST_PASS) &&
        validExpectedService(doc, OPTION_REST);
}

bool validMQTTConfig(DynamicJsonDocument doc) {
    return doc.containsKey(JSON_KEY_MQTT_BROKER) &&
        doc.containsKey(JSON_KEY_MQTT_PORT) &&
        doc.containsKey(JSON_KEY_MQTT_USER) &&
        doc.containsKey(JSON_KEY_MQTT_PASS) &&
        validExpectedService(doc, OPTION_MQTT);
}

bool setDefaultConfig() {
    return setDefaultServerConfig();
}

ICACHE_RAM_ATTR void resetConfig() {
    Serial.println("Resetting");
    bool stored = setDefaultConfig();

    if(stored) {
        while(!WiFi.disconnect()) {
            delay(100);
        }

        while(!WiFi.softAPdisconnect(true)) {
            delay(100);
        }

        detachInterrupt(digitalPinToInterrupt(BUTTON_INPUT));

        ESP.reset();
    }
}

void handleConfig() {
    if (configServer.hasArg("plain") == false){
        configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON,  HTTP_MISSING_JSON_BODY);
        return;
    }

    DynamicJsonDocument doc(DOC_SIZE);

    DeserializationError error = deserializeJson(doc, configServer.arg("plain"));

    if(error) {
        configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_ERROR_PARSING_BODY);
    } else if(!validService(doc)) {
        configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_SERVICE);
    } else {
        int serviceConfig = doc[JSON_KEY_SERVICE_CONFIG];

        if (!validWiFiConfig(doc)) {
            configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_WIFI_CONFIG);
        } else if(serviceConfig == OPTION_REST && !validRESTConfig(doc)) {
            configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_REST_CONFIG);
        } else if(serviceConfig == OPTION_MQTT && !validMQTTConfig(doc)) {
            configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_MQTT_CONFIG);
        } else {
            bool stored = storeConfig(doc);
            if(stored) {
                configServer.send(HTTP_OK, HTTP_TYPE_JSON, HTTP_SUCCESS);
                
                delay(500); //Adding slight delay in order to send the response
                
                configServer.close();
                while(!WiFi.disconnect()) {
                    delay(100);
                }

                while(!WiFi.softAPdisconnect(true)) {
                    delay(100);
                }

                detachInterrupt(digitalPinToInterrupt(BUTTON_INPUT));

                ESP.reset();
            } else {
                configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_ERROR_UNABLE_TO_STORE_CONFIG);
            }
        }
    }
}

void handleConfigServerSetup() {
    configServer.on(ENDPOINT_CONFIG, HTTP_POST, handleConfig);
    configServer.begin();
}

void handleConfigServer() {
    configServer.handleClient();  
}
