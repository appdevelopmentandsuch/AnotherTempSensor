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

  loadStruct(&settings, sizeof(settings));

  handleWifiSetup();

  setupConfig[settings.serviceConfig].func();
}

void loop() {
  runConfig[settings.serviceConfig].func();

  if(digitalRead(BUTTON_INPUT) == LOW) {
    resetConfig();
  }
}
