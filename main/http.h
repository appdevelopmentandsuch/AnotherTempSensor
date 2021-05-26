#ifndef HTTP_H
#define HTTP_H
#include <string.h>

#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400

#define HTTP_TYPE_JSON "text/json"

#define HTTP_MISSING_JSON_BODY "{\n\t\"error\":\"JSON body missing\""
#define HTTP_SUCCESS "{\n\t\"success\":true}"
#define HTTP_BAD_SERVICE "{\n\t\"error\":\"Invalid service, select 1 for REST or 2 for MQTT.\"}"
#define HTTP_BAD_WIFI_CONFIG "{\n\t\"error\":\"Missing a wifi configuration, please add one.\"}"
#define HTTP_BAD_REST_CONFIG "{\n\t\"error\":\"Invalid REST config, double check the config and try again.\"}"
#define HTTP_BAD_MQTT_CONFIG "{\n\t\"error\":\"Invalid MQTT config, double check the config and try again.\"}"

#endif