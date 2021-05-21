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
  EEPROM.begin(512);
  dht.begin();
  pinMode(BUTTON_INPUT, INPUT);

  handleWifiSetup();

  int configAddr = readInt(ADDRESS_SERVICE_CONFIG);

  setupConfig[currentConfig].func();
}

void loop() {
  runConfig[currentConfig].func();

  if(digitalRead(BUTTON_INPUT) == LOW) {
    resetConfig();
  }
}
