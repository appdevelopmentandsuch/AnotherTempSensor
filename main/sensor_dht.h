#ifndef SENSOR_DHT_H
#define SENSOR_DHT_H

#include "DHT.h"

extern DHT dht;

float readTemperature();
float readHumidity();

#endif