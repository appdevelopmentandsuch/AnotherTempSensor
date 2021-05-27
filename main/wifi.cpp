#include "config.h"
#include "constants.h"
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
  setDefaultConfig();
  WiFi.softAP(DEFAULT_DEVICE_SSID, DEFAULT_DEVICE_PASS, WIFI_CHANNEL, WIFI_VISIBLE, WIFI_MAX_CONNECTIONS);
}

void handleWifiSetup() {
  const char* ssid = settings[JSON_KEY_WIFI_SSID]; 
  const char* pass = settings[JSON_KEY_WIFI_PASS]; 

  while(!WiFi.disconnect()) {
    delay(SETUP_DELAY);
  }

  if(ssid != "" && pass != "") {
    Serial.print("Connecting with ssid: \""); Serial.print(ssid); Serial.print("\" and pass: \""); Serial.print(pass); Serial.println("\"");
    WiFi.begin(ssid, pass);

    uint tries = 0;
      while (!isConnected() && tries < 10) {
        delay(SETUP_DELAY);
        tries += 1;
      }

    if(isConnected() && tries < 10) {
      Serial.println("WiFi connected.");
      Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
      int serviceConfig = settings[JSON_KEY_SERVICE_CONFIG];
      if(serviceConfig > OPTION_MQTT || serviceConfig < OPTION_REST) {
        settings[JSON_KEY_SERVICE_CONFIG] = OPTION_CONFIG;
        storeConfig(settings);
      }
    } else {
      handleNoWifi();
    }
  } else {
    handleNoWifi();
  }
}
