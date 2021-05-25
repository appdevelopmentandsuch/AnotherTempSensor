#ifndef UTILS_H
#define UTILS_H
#include <string.h>

settings_t settings;

String readString(int addr);
int readInt(int addr);
int writeString(int addr, String data);
int writeInt(int addr, int data);

#endif