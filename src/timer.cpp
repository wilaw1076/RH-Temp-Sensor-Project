#include "timer.hpp"
#include <stdint.h>
#include <avr/io.h>


void timer_init()
{
    // Timer1 normal mode
    TCCR1A = 0;

    // Prescaler = 8
    //
    // 16 MHz / 8 = 2 MHz
    //
    // Therefore:
    // 1 timer tick = 0.5 us
    TCCR1B = (1 << CS11);

    // Start counter at zero
    TCNT1 = 0;
}


uint16_t timer_get_ticks()
{
    return TCNT1;
}


void timer_delay_us(uint16_t us)
{
    uint16_t start = TCNT1;

    // At 16 MHz with /8 prescaler:
    //
    // 2 ticks = 1 us
    uint16_t ticks = us * 2;

    while ((uint16_t)(TCNT1 - start) < ticks)
    {
        // Wait
    }
}


void timer_delay_ms(uint16_t ms)
{
    while (ms--)
    {
        timer_delay_us(1000);
    }
}