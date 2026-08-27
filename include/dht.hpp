#ifndef DHT11_HPP
#define DHT11_HPP

#include <stdint.h>

struct DHT11_Data
{
    uint8_t humidity;
    uint8_t temperature;
};

void dht_init();

bool dht_read(DHT11_Data* result);

#endif // DHT11_HPP