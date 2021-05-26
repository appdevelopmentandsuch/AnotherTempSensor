#include "config.h"
#include "mqtt.h"
#include "rest.h"
#include "sensor_dht.h"
#include "constants.h"
#include "utils.h"
#include "wifi.h"
#include <EEPROM.h>

int serviceConfig = OPTION_CONFIG;

void setup() {
  Serial.begin(115200);
  dht.begin();
  EEPROM.begin(DOC_SIZE);
  pinMode(BUTTON_INPUT, INPUT);
  attachInterrupt(BUTTON_INPUT, resetConfig, RISING);

  settings = loadConfig();

  serviceConfig = settings[JSON_KEY_SERVICE_CONFIG];

  handleWifiSetup();

  setupConfig[serviceConfig].func();
}

void loop() {
  runConfig[serviceConfig].func();
}
