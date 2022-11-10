#include "HD44780U.h"

static short HD44780U_send_nibble(uint8_t hd_address, char data);
short init_hd44780u(const uint8_t i2c_address, uint8_t display_lines)
{
    _delay_ms(16);

    i2c_init(i2C_400);
    
    
    HD44780U_send_nibble(i2c_address, DB4_PIN | DB5_PIN);
    _delay_ms(5);

    HD44780U_send_nibble(i2c_address, DB4_PIN | DB5_PIN);
    _delay_us(110);

    HD44780U_send_nibble(i2c_address, DB4_PIN | DB5_PIN);
     _delay_us(50);
    
    HD44780U_send_nibble(i2c_address, DB5_PIN);
    _delay_us(50);

    HD44780U_send_nibble(i2c_address, 0b00100000);
    _delay_ms(50);
    HD44780U_send_nibble(i2c_address, 0b10000000);

    _delay_ms(50);

    /* Display Off */
    HD44780U_send_nibble(i2c_address, 0);
    _delay_ms(50);
    HD44780U_send_nibble(i2c_address, 0b00000000);

    _delay_ms(50);

    /* Display Clear */
    HD44780U_send_nibble(i2c_address, 0);
    _delay_ms(50);
    HD44780U_send_nibble(i2c_address, 0);

    _delay_ms(50);
    HD44780U_send_nibble(i2c_address, 0);
    _delay_ms(50);
    HD44780U_send_nibble(i2c_address, 0b01100000);

}

static short HD44780U_send_nibble(uint8_t hd_address, char data)
{
    i2c_transmit(hd_address, data);
    i2c_transmit(hd_address, data | E_PIN);
    _delay_us(0.5);
    i2c_transmit(hd_address, data &~E_PIN);
    _delay_us(0.5);

}

void backlight_on(const uint8_t i2c_address)
{
    HD44780U_send_nibble(i2c_address, 0);
    HD44780U_send_nibble(i2c_address, BACKLIGHT_ON);

}
void backlight_off(const uint8_t i2c_address)
{
    HD44780U_send_nibble(i2c_address, 0);
    HD44780U_send_nibble(i2c_address, 0b10000000);
}

void cursor_on(const uint8_t i2c_address)
{
    HD44780U_send_nibble(i2c_address, 0);
    HD44780U_send_nibble(i2c_address, 0b11101000);
}

void cursor_off(const uint8_t i2c_address)
{
    HD44780U_send_nibble(i2c_address, 0);
    HD44780U_send_nibble(i2c_address, 0b11001000);
}

void cursor_blink(const uint8_t i2c_address)
{
    HD44780U_send_nibble(i2c_address, 0);
    HD44780U_send_nibble(i2c_address, 0b11111000);

}

void print_char(const uint8_t i2c_address, const char c)
{

    HD44780U_send_nibble(i2c_address, (c & 0xf0) | P3_PIN | RS_PIN);
    HD44780U_send_nibble(i2c_address, (c << 4) | P3_PIN | RS_PIN);
}
void print_string(const uint8_t i2c_address, const char *buffer)
{
    while (*buffer != '\0')
        print_char(i2c_address, *buffer++);
}