#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"
#include "mqtt.h"
#include "rest.h"
#include "server_config.h"
#include "stdint.h"

uint8_t config = OPTION_MQTT;

typedef struct { 
  uint8_t config;
  void (*func)(void);
} configDictionary;

const configDictionary setupConfig[] {
  {OPTION_REST, &handleServerSetup},
  {OPTION_MQTT, &handleMQTTSetup},
};

const configDictionary runConfig[] {
  {OPTION_REST, &handleServer},
  {OPTION_MQTT, &handleMQTT},
};

#endif