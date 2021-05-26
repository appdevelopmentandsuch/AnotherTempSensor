#include "config.h"
#include "mqtt.h"
#include "rest.h"
#include "secrets.h"
#include "sensor_dht.h"
#include "server_config.h"
#include "utils.h"
#include "wifi.h"
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(BUTTON_INPUT, INPUT);

  loadConfig(&settings, sizeof(settings));

  handleWifiSetup();

  // Serial.println(settings.serviceConfig);
  // Serial.println(settings.ssid);
  // Serial.println(settings.pass);

  setupConfig[settings.serviceConfig].func();
}

void loop() {
  runConfig[settings.serviceConfig].func();

  if(digitalRead(BUTTON_INPUT) == LOW) {
    resetConfig();
  }
}
