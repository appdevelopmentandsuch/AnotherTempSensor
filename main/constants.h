#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string.h>

#define DHT_PIN D5
#define DHT_TYPE DHT11
#define LOOP_DELAY 5000
#define SETUP_DELAY 500
#define VERSION "0.0.0"

#define WIFI_CHANNEL 9
#define WIFI_VISIBLE 0
#define WIFI_MAX_CONNECTIONS 1

#define BUTTON_INPUT 0

#define OPTION_CONFIG 0
#define OPTION_REST 1
#define OPTION_MQTT 2

#define ADDRESS_CONFIG 0

#define DOC_SIZE 1024

#define DEFAULT_DEVICE_SSID "AnotherTempSensor"
#define DEFAULT_DEVICE_PASS "password"

#define JSON_KEY_WIFI_SSID "ssid"
#define JSON_KEY_WIFI_PASS "pass"
#define JSON_KEY_SERVICE_CONFIG "service"
#define JSON_KEY_MQTT_BROKER "mqttBroker"
#define JSON_KEY_MQTT_PORT "mqttPort"
#define JSON_KEY_MQTT_USER "mqttUser"
#define JSON_KEY_MQTT_PASS "mqttPass"
#define JSON_KEY_REST_USER "restUser"
#define JSON_KEY_REST_PASS "restPass"

#define JSON_KEY_TEMPERATURE "temperature"
#define JSON_KEY_HUMIDITY "humidity"
#define JSON_KEY_IDENTIFIER "identifier"
#define JSON_KEY_VERSION "version"

#define ENDPOINT_READ_ALL "/api/read/all/"
#define ENDPOINT_READ_TEMPERATURE "/api/read/temperature/"
#define ENDPOINT_READ_HUMIDITY "/api/read/humidity/"
#define ENDPOINT_INFO "/api/info/"

#define ENDPOINT_CONFIG "/api/config/"

#endif