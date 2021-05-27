#include "config.h"
#include "constants.h"
#include "mqtt.h"
#include "rest.h"
#include "sensor_dht.h"
#include "utils.h"
#include "wifi.h"
#include <EEPROM.h>
#include <HardwareSerial.h>

int serviceConfig = OPTION_CONFIG;

void setup() {
  Serial.begin(115200);
  dht.begin();
  EEPROM.begin(DOC_SIZE);

  pinMode(BUTTON_INPUT, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_INPUT), resetConfig, RISING);

  settings = loadConfig();

  serviceConfig = settings[JSON_KEY_SERVICE_CONFIG];

  handleWifiSetup();

  setupConfig[serviceConfig].func();
}

void loop() {
  runConfig[serviceConfig].func();
}
