#ifndef TWI_HPP
#define TWI_HPP

#include <stdint.h>

void twi_init();
bool twi_start_condition();
bool twi_send_address(uint8_t address);
void twi_stop();
bool twi_write(uint8_t data);

#endif