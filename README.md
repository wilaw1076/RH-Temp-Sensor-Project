# Bare-Metal Temperature & Humidity Monitor

A bare-metal C++ environmental monitoring system built on the ATmega2560.
The project reads temperature and humidity from a DHT11 sensor and displays
the values on an I2C LCD1602 without using the Arduino framework.

## Features

- Register-level GPIO control for DHT11 communication
- Timer1-based microsecond pulse measurement
- DHT11 40-bit protocol decoding and checksum validation
- Custom USART driver for serial debugging
- Custom TWI/I2C driver operating at 100 kHz
- PCF8574 I/O expander driver
- LCD1602 4-bit display driver
- Bare-metal `main()` with no `setup()` / `loop()`

## Hardware

- Arduino Mega 2560 / ATmega2560
- DHT11 temperature and humidity sensor
- LCD1602
- PCF8574 I2C backpack

## Software Architecture

main.cpp
├── dht.cpp
├── timer.cpp
├── usart.cpp
├── twi.cpp
├── pcf8574.cpp
└── lcd1602.cpp

## Communication

### DHT11

The ATmega2560 communicates with the DHT11 using GPIO on PG5.
Timer1 runs with a /8 prescaler, providing 0.5 us timer resolution for
measuring DHT11 pulse widths.

### I2C LCD

The ATmega2560 communicates with the PCF8574 using its hardware TWI
peripheral at 100 kHz. The PCF8574 drives the LCD1602 in 4-bit mode.

## Example Output

LCD:

Temp: 22C
RH: 64%

Serial:

Temperature: 22C
Humidity: 64%

## Build

Built with PlatformIO using the AVR toolchain.

No Arduino framework APIs are used.
