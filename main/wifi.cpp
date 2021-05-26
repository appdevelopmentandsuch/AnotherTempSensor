#include "config.h"
#include "server_config.h"
#include "utils.h"
#include "wifi.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>

// Wifi Connection Methods
bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void handleNoWifi() {
  Serial.println("Unable to connect to Wifi, starting device Wifi...");
  const char* defaultSSID = DEFAULT_DEVICE_SSID;
  const char* defaultPass = DEFAULT_DEVICE_PASS;
  setDefaultConfig();
  WiFi.softAP(defaultSSID, defaultPass, 9, 0, 1);
}

void handleWifiSetup() {
  
  const char* ssid = settings[JSON_SETTING_WIFI_SSID]; 
  const char* pass = settings[JSON_SETTING_WIFI_PASS]; 

  if(ssid != "" && pass != "") {
    WiFi.begin(ssid, pass);

    uint tries = 0;
      while (!isConnected() && tries < 10) {
        delay(SETUP_DELAY);
        tries += 1;
      }

    if(isConnected() && tries < 10) {
      Serial.println("WiFi connected.");
      Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
      int serviceConfig = settings["serviceConfig"];
      if(serviceConfig > OPTION_MQTT || serviceConfig < OPTION_REST) {
        settings["serviceConfig"] = OPTION_CONFIG;
        storeConfig(settings);
      }
    } else {
      handleNoWifi();
    }
  } else {
    handleNoWifi();
  }
}
