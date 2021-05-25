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

void storeStruct(void *data_source, size_t size)
{
  serverSettings tmp = *(serverSettings*)data_source;
  Serial.println(tmp.serviceConfig);
  Serial.println(tmp.ssid);
  Serial.println(tmp.pass);
  Serial.println(tmp.mqttBroker);
  Serial.println(tmp.mqttPort);
  Serial.println(tmp.mqttUser);
  Serial.println(tmp.mqttPass);
  Serial.println(tmp.restUser);
  Serial.println(tmp.restPass);
  Serial.print("Wrote size "); Serial.println(size);
  EEPROM.begin(size * 2);
  for(size_t i = 0; i < size; i++)
  {
    char data = ((char *)data_source)[i];
    EEPROM.write(i, data);
  }
  if(EEPROM.commit()) {
    Serial.println("Wrote config to EEPROM");
  } else {
    Serial.println("Config could not be written to EEPROM");
  }
}

void loadStruct(void *data_dest, size_t size)
{
    EEPROM.begin(size * 2);
    for(size_t i = 0; i < size; i++)
    {
        char data = EEPROM.read(i);
        ((char *)data_dest)[i] = data;
    }
}