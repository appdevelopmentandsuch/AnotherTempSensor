#include "config.h"
#include "secrets.h"
#include "server_config.h"
#include "utils.h"
#include "wifi.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>

// Wifi Connection Methods
bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void handleWifiSetup() {
  const char* ssid = readString(ADDRESS_SSID);
  const char* pass = readString(ADDRESS_PASS);

  WiFi.begin(ssid, pass);

  uint tries = 0;

  while (!isConnected() && tries < 10) {
    delay(SETUP_DELAY);
    tries += 1;
  }

  if(isConnected() && tries < 10) {
    Serial.println("WiFi connected.");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  } else {
    setServiceConfig(OPTION_CONFIG);
    Serial.println("Unable to connect to Wifi, starting device Wifi...");
    const char* defaultSSID = SECRET_DEVICE_SSID;
    const char* defaultPass = SECRET_DEVICE_PASS;
    WiFi.softAP(defaultSSID, defaultPass, 9, 0, 1);
  }
}
