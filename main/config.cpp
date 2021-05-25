#include "config.h"
#include "server_config.h"
#include "utils.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

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

void resetConfig() {
    storeStruct(&defaultSettings, sizeof(defaultSettings));

    ESP.reset();
}

void handleConfig() {
    // if (configServer.hasArg("plain")== false){ //Check if body received
    //     configServer.send(200, "text/plain", "Body not received");
    //     return;
    // }

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
        configServer.send(400, "text/plain", "Invalid service, must select 0 for REST, or 1 for MQTT, restarting...");
    } else {
        configServer.send(200, "text/json", "{success:true}");
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

        storeStruct(&userConfig, sizeof(userConfig));
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
