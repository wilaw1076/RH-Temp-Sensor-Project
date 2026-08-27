#include <usart.hpp>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

void usart_init(uint32_t baud)
{
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;

    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    // Enable transmitter
    UCSR0B = (1 << TXEN0);

    // 8 data bits, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_write_byte(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until the transmit buffer is empty
    UDR0 = data; // Send the character
}

void usart_printString(const char* text) {
    while (*text) {
        usart_write_byte(*text++);
    }
}

void usart_printInt(uint32_t value){
    char buffer[12];
    ultoa(value, buffer, 10); // Convert unsigned long to string in base 10
    usart_printString(buffer);
}