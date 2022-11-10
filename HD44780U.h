#ifndef __HD44780U_H__
#define __HD44780U_h__

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "i2c.h"

#define RS_PIN  0x1
#define RW_PIN  0x2
#define E_PIN   0x4
#define P3_PIN  0x8
#define DB4_PIN 0x10
#define DB5_PIN 0x20
#define DB6_PIN 0x40
#define DB7_PIN 0x80

#define BACKLIGHT_ON 0xc
#define BACKLIGHT_OFF 0x8

short init_hd44780u(const uint8_t i2c_address, uint8_t display_lines);
void backlight_on(const uint8_t i2c_address);
void backlight_off(const uint8_t i2c_address);
void cursor_blink(const uint8_t i2c_address);
void cursor_on(const uint8_t i2c_address);
void cursor_off(const uint8_t i2c_address);
void print_char(const uint8_t i2c_address, const char c);
void print_string(const uint8_t i2c_address, const char *buffer);
#endif