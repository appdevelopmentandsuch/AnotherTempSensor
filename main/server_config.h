#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H
#include <string.h>

#define DHT_PIN D5
#define DHT_TYPE DHT11
#define LOOP_DELAY 5000
#define MQTT_BROKER "";
#define MQTT_PORT 1883
#define SETUP_DELAY 500
#define VERSION "0.0.0"

#define BUTTON_INPUT 0

#define OPTION_REST 0
#define OPTION_MQTT 1
#define OPTION_CONFIG 2

#define ADDRESS_SSID 0
#define ADDRESS_PASS ADDRESS_SSID + sizeof(String)
#define ADDRESS_MQTT_BROKER ADDRESS_PASS + sizeof(String)
#define ADDRESS_MQTT_USER ADDRESS_MQTT_BROKER + sizeof(String)
#define ADDRESS_MQTT_PASS ADDRESS_MQTT_USER + sizeof(String)
#define ADDRESS_REST_USER ADDRESS_MQTT_PASS + sizeof(String)
#define ADDRESS_REST_PASS ADDRESS_REST_USER + sizeof(String)
#define ADDRESS_SERVICE_CONFIG ADDRESS_REST_PASS + sizeof(String)

#endif