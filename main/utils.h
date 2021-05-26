#ifndef UTILS_H
#define UTILS_H

#include <ArduinoJson.h>
#include <stdlib.h>
#include <string.h>

extern DynamicJsonDocument settings;
extern char defaultConfig[];

bool setDefaultServerConfig();
bool storeConfig(DynamicJsonDocument doc);
DynamicJsonDocument loadConfig();
bool deserializeJsonDoc(DynamicJsonDocument doc, char str[]);
bool deserializeJsonDoc(DynamicJsonDocument doc, String str);

#endif