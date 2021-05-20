#include "config.h"
#include "mqtt.h"
#include "rest.h"
#include "secrets.h"
#include "sensor_dht.h"
#include "server_config.h"
#include "wifi.h"
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);

  handleWifiSetup();

  setupConfig[config].func();

  dht.begin();
}

void loop() {
  if (isConnected()) {
    runConfig[config].func();
  }
}

