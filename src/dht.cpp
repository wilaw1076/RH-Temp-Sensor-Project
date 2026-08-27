#include <dht.hpp>
#include <timer.hpp>
#include <usart.hpp>
#include <stdint.h>
#include <avr/io.h>

#define DHT_DDRG *(volatile uint8_t*) 0x33
#define DHT_PORTG *(volatile uint8_t*) 0x34
#define DHT_PIN *(volatile uint8_t*) 0x32
#define DHT_BIT PG5
#define DHT_TICKS_PER_US 2

//helper Functions
static void dht_input(){
    DHT_DDRG &= ~(1 << DHT_BIT);
}

static void dht_output(){
    DHT_DDRG |= (1 << DHT_BIT);
}

static void dht_drive_low(){
    DHT_PORTG &= ~(1 << DHT_BIT);
}

static bool dht_is_high(){
    return (DHT_PIN & (1 << DHT_BIT)) != 0;
}

static bool dht_wait_for_level(bool high, uint16_t timeout_us)
{
    uint16_t start = timer_get_ticks();

    uint16_t timeout_ticks = timeout_us * DHT_TICKS_PER_US;

    while (dht_is_high() != high)
    {
        uint16_t elapsed = (uint16_t)(timer_get_ticks() - start);

        if (elapsed >= timeout_ticks)
        {
            return false;
        }
    }
    return true;
}

static bool dht_start()
{
    dht_drive_low();
    dht_output();

    timer_delay_ms(18);

    dht_input();

    timer_delay_us(30);

    if (!dht_wait_for_level(false, 100))
    {
        usart_printString("FAIL: no initial LOW\r\n");
        return false;
    }

    if (!dht_wait_for_level(true, 100))
    {
        usart_printString("FAIL: no response HIGH\r\n");
        return false;
    }

    if (!dht_wait_for_level(false, 100))
    {
        usart_printString("FAIL: no first data LOW\r\n");
        return false;
    }
    return true;
}

static bool dht_read_bit(uint8_t* bit)
{
    if (!dht_wait_for_level(true, 100)) // Wait for DATA to become HIGH
    {
        return false;
    }

    uint16_t start = timer_get_ticks(); // Start timing the HIGH period

    // Wait until DATA becomes LOW again.
    if (!dht_wait_for_level(false, 120))
    {
        return false;
    }

    uint16_t elapsed = (uint16_t)(timer_get_ticks() - start); // Calculate the elapsed time in ticks

    if (elapsed > 100)
    {
        *bit = 1;
    }
    else
    {
        *bit = 0;
    }
    return true;
}

static bool dht_read_byte(uint8_t* value)
{
    *value = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t bit;

        if (!dht_read_bit(&bit))
        {
            return false;
        }
        *value <<= 1;

        if (bit)
        {
            *value |= 1;
        }
    }
    return true;
}

void dht_init()
{
    // PORTG5 = 0.
    //
    // When PG5 is an input, this also keeps
    // the ATmega internal pull-up disabled.
    DHT_DDRG &= ~(1 << DHT_BIT);

    // Start with the MCU not driving DATA.
    DHT_PORTG |= (1 << DHT_BIT);

    if (dht_is_high())
    {
        usart_printString("Idle: HIGH\r\n");
    }
    else
    {
        usart_printString("Idle: LOW\r\n");
    }
}

bool dht_read(DHT11_Data* result)
{
    if (result == nullptr)
    {
        return false;
    }

    uint8_t data[5] = {0};

    // Start communication and verify that
    // the DHT11 responds.
    if (!dht_start())
    {
        return false;
    }

    // Receive all five bytes.
    for (uint8_t i = 0; i < 5; i++)
    {
        if (!dht_read_byte(&data[i]))
        {
            return false;
        }
    }
    
    uint8_t checksum =
        data[0] +
        data[1] +
        data[2] +
        data[3];

    if (checksum != data[4])
    {
        return false;
    }

    result->humidity = data[0];
    result->temperature = data[2];

    return true;
}