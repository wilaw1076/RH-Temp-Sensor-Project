#include "pcf8574.hpp"
#include "twi.hpp"

static uint8_t pcf8574_address;

void pcf8574_init(uint8_t address)
{
    pcf8574_address = address;
}

bool pcf8574_write(uint8_t data)
{
    if (!twi_start_condition())
    {
        return false;
    }

    if (!twi_send_address(pcf8574_address))
    {
        twi_stop();
        return false;
    }

    if (!twi_write(data))
    {
        twi_stop();
        return false;
    }

    twi_stop();
    return true;
}