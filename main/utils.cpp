#include "utils.h"
#include <EEPROM.h>
#include <HardwareSerial.h>

serverSettings settings  {
    0,
    "",
    "",
    "",
    1883,
    "",
    "",
    "",
    ""
};

void printConfig(void *data) {
  serverSettings tmp = *(serverSettings*)data;
  Serial.println(tmp.serviceConfig);
  Serial.println(tmp.ssid);
  Serial.println(tmp.pass);
  Serial.println(tmp.mqttBroker);
  Serial.println(tmp.mqttPort);
  Serial.println(tmp.mqttUser);
  Serial.println(tmp.mqttPass);
  Serial.println(tmp.restUser);
  Serial.println(tmp.restPass);
}

void loadConfig(void *data_dest, size_t size) {
    EEPROM.begin(size * 2);
    for(size_t i = 0; i < size; i++) {
        char data = EEPROM.read(i);
        ((char *)data_dest)[i] = data;
    }
    Serial.println("Loaded Config");
    printConfig(&*(serverSettings*)data_dest);
}

bool storeConfig(void *data_source, size_t size) {
  bool stored = false;
  serverSettings tmp = *(serverSettings*)data_source;

  EEPROM.begin(size * 2);
  for(size_t i = 0; i < size; i++) {
    char data = ((char *)data_source)[i];
    EEPROM.write(i, data);
  }
  if(EEPROM.commit()) {
    Serial.println("Wrote config to EEPROM");

    serverSettings testSettings;
    loadConfig(&testSettings, sizeof(testSettings));
    if(!memcmp(&tmp, &testSettings, sizeof(testSettings))) {
      Serial.println("Loaded settings match stored settings");
      printConfig(&testSettings);
      stored = true;
    } else {
      Serial.println("Loaded settings do not match stored settings");
    }
  } else {
    Serial.println("Config could not be written to EEPROM");
  }
  return stored;
}
