#include "server_config.h"
#include "utils.h"
#include <EEPROM.h>
#include <HardwareSerial.h>
#include <StreamUtils.h>

DynamicJsonDocument settings(DOC_SIZE);
char defaultConfig[] = "{\n\t\"ssid\": \"\",\n\t\"pass\": \"\",\n\t\"mqttBroker\": \"\",\n\t\"mqttPort\": 1883,\n\t\"mqttUser\":\"\",\n\t\"mqttPass\":\"\",\n\t\"restUser\":\"\",\n\t\"restPass\":\"\",\n\t\"service\": 1\n}\"}";

// bool deserializeJsonDoc(DynamicJsonDocument doc, char str[]) {
//   DeserializationError error = deserializeJson(doc, str);

//   if (error) {
//     Serial.print(F("deserializeJson() failed: "));
//     Serial.println(error.f_str());
//     return false;
//   }

//   return true;
// }

bool setDefaultServerConfig() {
  DynamicJsonDocument doc(DOC_SIZE);
  DeserializationError error = deserializeJson(doc, defaultConfig);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }

  doc["service"] = OPTION_CONFIG;

  return storeConfig(doc);
}

void printConfig(DynamicJsonDocument doc) {
  int serviceConfig = doc["service"];
  const char* ssid = doc["ssid"];
  const char* pass = doc["pass"];
  const char* mqttBroker = doc["mqttBroker"];
  int mqttPort = doc["mqttPort"];
  const char* mqttUser = doc["mqttUser"];
  const char* mqttPass = doc["mqttPass"];
  const char* restUser = doc["restUser"];
  const char* restPass = doc["restPass"];

  Serial.println("\n===========================================");
  Serial.print("serviceConfig: "); Serial.println(serviceConfig);
  Serial.print("ssid: "); Serial.println(ssid);
  Serial.print("pass: "); Serial.println(pass);
  Serial.print("mqttBroker: "); Serial.println(mqttBroker);
  Serial.print("mqttPort: "); Serial.println(mqttPort);
  Serial.print("mqttUser: "); Serial.println(mqttUser);
  Serial.print("mqttPass: "); Serial.println(mqttPass);
  Serial.print("restUser: "); Serial.println(restUser);
  Serial.print("restPass: "); Serial.println(restPass);
  Serial.println("\n===========================================");
}

DynamicJsonDocument loadConfig() {
  DynamicJsonDocument doc(DOC_SIZE);
  EepromStream eepromStream(0, DOC_SIZE);
  deserializeJson(doc, eepromStream);
  printConfig(doc);
  return doc;
}

bool storeConfig(DynamicJsonDocument doc) {
  EepromStream eepromStream(0, DOC_SIZE);
  serializeJson(doc, eepromStream);
  if(EEPROM.commit()) {
    Serial.println("Wrote config to EEPROM");
    printConfig(loadConfig());
  } else {
    Serial.println("Config was not written to EEPROM");
  }
}
