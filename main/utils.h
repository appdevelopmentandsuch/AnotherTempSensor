#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <ArduinoJson.h>

extern DynamicJsonDocument settings;
extern char defaultConfig[];

bool setDefaultServerConfig();
bool storeConfig(DynamicJsonDocument doc);
DynamicJsonDocument loadConfig();

#endif