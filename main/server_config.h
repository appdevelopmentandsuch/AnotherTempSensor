#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H
#include <string.h>

#define DHT_PIN D5
#define DHT_TYPE DHT11
#define LOOP_DELAY 5000
#define SETUP_DELAY 500
#define VERSION "0.0.0"

#define BUTTON_INPUT 0

#define OPTION_CONFIG 0
#define OPTION_REST 1
#define OPTION_MQTT 2

#define DOC_SIZE 1024

#define DEFAULT_DEVICE_SSID "AnotherTempSensor"
#define DEFAULT_DEVICE_PASS "password"

#define JSON_SETTING_WIFI_SSID "ssid"
#define JSON_SETTING_WIFI_PASS "pass"
#define JSON_SETTING_SERVICE_CONFIG "service"
#define JSON_SETTING_MQTT_BROKER "mqttBroker"
#define JSON_SETTING_MQTT_PORT "mqttPort"
#define JSON_SETTING_MQTT_USER "mqttUser"
#define JSON_SETTING_MQTT_PASS "mqttPass"
#define JSON_SETTING_REST_USER "restUser"
#define JSON_SETTING_REST_PASS "restPass"

#endif