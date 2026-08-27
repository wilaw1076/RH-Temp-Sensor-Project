#include "usart.hpp"
#include "timer.hpp"
#include "dht.hpp"
#include "twi.hpp"
#include "pcf8574.hpp"
#include "lcd1602.hpp"

int main(void)
{
    timer_init();
    usart_init(9600);

    dht_init();

    twi_init();
    lcd_init();

    timer_delay_ms(2000);

    while (1)
    {
        DHT11_Data reading;

        if (dht_read(&reading))
        {
            lcd_set_cursor(0, 0);
            lcd_print("Temp: ");
            lcd_print_int(reading.temperature);
            lcd_print("C");

            lcd_set_cursor(0, 1);
            lcd_print("RH: ");
            lcd_print_int(reading.humidity);
            lcd_print("%");

            usart_printString("Temperature: ");
            usart_printInt(reading.temperature);
            usart_printString("C\r\n");

            usart_printString("Humidity: ");
            usart_printInt(reading.humidity);
            usart_printString("%\r\n");
        }
        else
        {
            lcd_set_cursor(0, 0);
            lcd_print("DHT11 Error");
        }

        timer_delay_ms(2000);
    }

    return 0;
}