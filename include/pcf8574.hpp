#ifndef PCF8574_HPP
#define PCF8574_HPP

#include <stdint.h>

void pcf8574_init(uint8_t address);
bool pcf8574_write(uint8_t data);

#endif // PCF8574_HPP