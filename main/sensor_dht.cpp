#include "constants.h"
#include "sensor_dht.h"
#include "DHT.h"
#include <Arduino.h>

DHT dht(DHT_PIN, DHT_TYPE);

float readTemperature() {
    int tries = 0;
    float temperature = dht.readTemperature();
    while(isnan(temperature) && tries < READ_MAX_TRIES) {
        temperature = dht.readTemperature();
        tries += 1;
    }

    if(isnan(temperature) || tries >= READ_MAX_TRIES) {
        return BAD_READ;
    }

    return temperature;
}

float readHumidity() {
    int tries = 0;
    float humidity = dht.readHumidity();
    while(isnan(humidity) && tries < READ_MAX_TRIES) {
        humidity = dht.readHumidity();
        tries += 1;
    }

    if(isnan(humidity) || tries >= READ_MAX_TRIES) {
        return BAD_READ;
    }
    return humidity;
}