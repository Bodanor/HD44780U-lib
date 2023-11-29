#ifndef __HD44780U_H__
#define __HD44780U_h__

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "i2c.h"


#define HD44780U_INIT_SUCCESS 0
#define HD44780U_INIT_FAIL 1

#define RS_PIN  0x01
#define RW_PIN  0x02
#define E_PIN   0x04
#define BT_PIN  0x08
#define DB4_PIN 0x10
#define DB5_PIN 0x20
#define DB6_PIN 0x40
#define DB7_PIN 0x80

#define HD44780U_2_ROWS 0x08
#define HD44780U_4_BIT_MODE 0x20
#define HD44780U_FONT_5x8     0x00
#define HD44780U_FONT_5x10    0x04
#define HD44780U_DISPLAY_CLEAR 0x01
#define HD44780U_ENTRY_MODE 0x06

#define HD44780U_DISPLAY_OFF 0x08
#define HD44780U_DISPLAY_COMMAND 0x8
#define HD44780U_DISPLAY_ON 0x04
#define HD44780U_DISPLAY_CURSOR_ON 0x02
#define HD44780U_DISPLAY_BLINK_ON 0x01

#define HD44780U_HOME 0x2

// TODO : Implement display shift as well as move cursor(x, y)

uint8_t init_hd44780u(const uint8_t i2c_address, uint8_t display_lines);
void hd44780u_clear_display();
void hd44780u_display_on();
void hd44780u_display_off();
void hd44780u_cursor_on();
void hd44780u_cursor_off();
void hd44780u_blink_on();
void hd44780u_blink_off();
void hd44780u_backlight_on();
void hd44780u_backlight_off();
void hd44780u_return_home();
void hd44780u_print_char(uint8_t character);
void hd44780u_print_string(char *s_source);
#endif