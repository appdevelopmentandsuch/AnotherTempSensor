#include "utils.h"
#include <EEPROM.h>
#include <HardwareSerial.h>

const char* readString(int add) {
  const char* result;
  EEPROM.get(add,result);

  Serial.print("Reading "); Serial.print(result); Serial.print(" at "); Serial.print(add); Serial.println("|");

  return result;
}

int readInt(char add) {
  int result;
  EEPROM.get(add,result);

  Serial.print("Reading "); Serial.print(result); Serial.print(" at "); Serial.print(add); Serial.println("|");

  return result;
}

void writeString(int add, const char* data)
{
  EEPROM.put(add, data);
  EEPROM.commit();
  readString(add);
}

void writeInt(char add, int data)
{
  EEPROM.put(add, data);
  EEPROM.commit();
  readInt(add);
}