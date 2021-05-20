#include "secrets.h"
#include "server_config.h"
#include "wifi.h"
#include <ESP8266WiFi.h>

char pass[] = SECRET_PASS;
char ssid[] = SECRET_SSID;

// Wifi Connection Methods
void handleWifiSetup() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(SETUP_DELAY);
  }

  Serial.println("WiFi connected.");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
}

bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
}