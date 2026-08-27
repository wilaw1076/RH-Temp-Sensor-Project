#ifndef TIMER_HPP
#define TIMER_HPP
#include <stdint.h>

void timer_init();
uint16_t timer_get_ticks();
void timer_delay_us(uint16_t us);
void timer_delay_ms(uint16_t ms);

#endif // TIMER_HPP