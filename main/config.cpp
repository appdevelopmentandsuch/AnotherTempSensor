#include "config.h"
#include "http.h"
#include "server_config.h"
#include "utils.h"
#include "wifi.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer configServer(80);

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
    int serviceConfig = doc[JSON_SETTING_SERVICE_CONFIG];
    const char* ssid = doc[JSON_SETTING_WIFI_SSID];
    const char* pass = doc[JSON_SETTING_WIFI_PASS];
    const char* mqttBroker = doc[JSON_SETTING_MQTT_BROKER];
    int mqttPort = doc[JSON_SETTING_MQTT_PORT];
    const char* mqttUser = doc[JSON_SETTING_MQTT_USER];
    const char* mqttPass = doc[JSON_SETTING_MQTT_PASS];
    const char* restUser = doc[JSON_SETTING_REST_USER];
    const char* restPass = doc[JSON_SETTING_REST_PASS];

    if(serviceConfig > OPTION_MQTT || serviceConfig < OPTION_REST) {
        configServer.send(HTTP_BAD_REQUEST, HTTP_TYPE_JSON, HTTP_BAD_SERVICE);
    } else {
        configServer.send(HTTP_OK, HTTP_TYPE_JSON, HTTP_SUCCESS);
        
        storeConfig(doc);

        WiFi.softAPdisconnect(true);

        delay(5000);

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
