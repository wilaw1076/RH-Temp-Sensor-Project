#include "twi.hpp"
#include <avr/io.h>

void twi_init(){
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); //Set prescaler to 1
    TWCR = (1 << TWEN); //Enable TWI
    TWBR = 72; //Set bit rate to 100kHz
}

bool twi_start_condition()
{
    TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT); //Send START condition

    while (!(TWCR & (1 << TWINT))) //waits for TWINT flag to be set, indicating that the START condition has been transmitted
    {
    }

    if ((TWSR & 0xF8) == 0x08) //Check if START condition was transmitted successfully
    {
        return true;
    }

    return false;
}

bool twi_send_address(uint8_t address)
{
    TWDR = (address << 1); //

    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    uint8_t status = TWSR & 0xF8; 

    if (status == 0x18) 
    {
        return true;
    }

    return false;
}

void twi_stop()
{
    TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT); //Send STOP condition

    while (TWCR & (1 << TWSTO))
    {
    }
}

bool twi_write(uint8_t data)
{
    TWDR = data;

    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    uint8_t status = TWSR & 0xF8;

    if (status == 0x28)
    {
        return true;
    }

    return false;
}

