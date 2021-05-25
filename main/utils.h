#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef struct {
  int serviceConfig;
  const char* ssid;
  const char* pass;
  const char* mqttBroker;
  int mqttPort;
  const char* mqttUser;
  const char* mqttPass;
  const char* restUser;
  const char* restPass;
} serverSettings;

extern serverSettings settings;

void storeStruct(void *data_source, size_t size);
void loadStruct(void *data_dest, size_t size);

#endif