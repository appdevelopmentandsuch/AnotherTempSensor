#include "utils.h"
#include <EEPROM.h>
#include <HardwareSerial.h>
#include <string.h>

typedef struct {
  int service_config;
  String ssid;
  String pass;
  String mqtt_broker;
  int mqtt_port;
  String mqtt_user;
  String mqtt_pass;
  String rest_user;
  String rest_pass;
} settings_t __attribute__ ((packed));

void storeStruct(void *data_source, size_t size)
{
  EEPROM.begin(size * 2);
  for(size_t i = 0; i < size; i++)
  {
    char data = ((char *)data_source)[i];
    EEPROM.write(i, data);
  }
  EEPROM.commit();
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