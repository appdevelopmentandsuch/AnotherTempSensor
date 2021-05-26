#ifndef HTTP_H
#define HTTP_H
#include <string.h>

#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400

#define HTTP_TYPE_JSON "text/json"

#define HTTP_MISSING_JSON_BODY "{\n\t\"error\":\"JSON body missing\""
#define HTTP_SUCCESS "{\n\t\"success\":true}"
#define HTTP_BAD_SERVICE "{\n\t\"error\":\"Invalid service, select 1 for REST or 2 for MQTT.\"}"

#endif