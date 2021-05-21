#ifndef CONFIG_H
#define CONFIG_H

#include "mqtt.h"
#include "rest.h"
#include "server_config.h"
#include "stdint.h"

void handleConfigServerSetup();
void handleConfigServer();
void resetConfig();
void setServiceConfig(int service);

extern uint8_t currentConfig;

typedef struct { 
  uint8_t itemConfig;
  void (*func)(void);
} configDictionary;

const configDictionary setupConfig[] {
  {OPTION_REST, &handleServerSetup},
  {OPTION_MQTT, &handleMQTTSetup},
  {OPTION_CONFIG, &handleConfigServerSetup},
};

const configDictionary runConfig[] {
  {OPTION_REST, &handleServer},
  {OPTION_MQTT, &handleMQTT},
  {OPTION_CONFIG, &handleConfigServer},
};

#endif