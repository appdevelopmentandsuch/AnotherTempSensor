#include "config.h"
#include "server_config.h"
#include "utils.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include "wifi.h"

ESP8266WebServer configServer(80);

serverSettings defaultSettings {
    0,
    "",
    "",
    "",
    1883,
    "",
    "",
    "",
    ""
};

bool setDefaultConfig() {
    return storeConfig(&defaultSettings, sizeof(defaultSettings));
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
        configServer.send(400, "text/json", "{\n\t\"error\":\"JSON body missing, fill out this body and include in request: {\n\t\"ssid\": \"\",\n\t\"pass\": \"\",\n\t\"mqttBroker\": \"\",\n\t\"mqttPort\": 1883,\n\t\"mqttUser\":\"\",\n\t\"mqttPass\":\"\",\n\t\"restUser\":\"\",\n\t\"restPass\":\"\",\n\t\"service\": 1\n}\"}");
        return;
    }

    DynamicJsonDocument doc(1024);

    deserializeJson(doc, configServer.arg("plain"));
    int config = doc["service"];
    const char* ssid = doc["ssid"];
    const char* pass = doc["pass"];
    const char* mqttBroker = doc["mqttBroker"];
    int mqttPort = doc["mqttPort"];
    const char* mqttUser = doc["mqttUser"];
    const char* mqttPass = doc["mqttPass"];
    const char* restUser = doc["restUser"];
    const char* restPass = doc["restPass"];

    if(config > OPTION_MQTT || config < OPTION_REST) {
        configServer.send(400, "text/json", "{\n\t\"error\":\"Invalid service, select 1 for REST or 2 for MQTT.\"}");
    } else {
        configServer.send(200, "text/json", "{\n\t\"success\":true}");
        serverSettings userConfig {
            config,
            ssid,
            pass,
            mqttBroker,
            mqttPort,
            mqttUser,
            mqttPass,
            restUser,
            restPass
        };

        storeConfig(&userConfig, sizeof(userConfig));
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
