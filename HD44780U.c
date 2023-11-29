#include "HD44780U.h"

union hd44780u_lines_u {
    struct {
        uint8_t RS : 1;
        uint8_t RW : 1;
        uint8_t E : 1;
        uint8_t BT : 1;
        uint8_t DB4 : 1;
        uint8_t DB5 : 1;
        uint8_t DB6 : 1;
        uint8_t DB7 : 1;
    };
    uint8_t hd44780u_lines;
};
union hd44780u_display_state_u{
    struct {
        uint8_t display_status : 1;
        uint8_t cursor_status : 1;
        uint8_t blink_status : 1;
        uint8_t backlight : 1;
    };
    uint8_t display;
};


static void hd44780u_e_pin_pulse(union hd44780u_lines_u hd44780u_lines);
static void hd44780u_send_4bits_4mode_command(uint8_t command);
static void hd44780u_send_8bits_4mode_command(uint8_t command);
static void hd44780u_write_command(uint8_t data);

static short hd44780u_i2c_addr = 0;
static union hd44780u_display_state_u hd44780u_display_state;


uint8_t init_hd44780u(const uint8_t i2c_address, uint8_t display_lines)
{
    hd44780u_display_state.display = HD44780U_DISPLAY_COMMAND; /* Initialize the internal representation of the lcd display (blink (on/off), cursor (on/off), display (on/off))*/
    hd44780u_i2c_addr = i2c_address;
    i2c_init(i2C_400);

    /** Initialization by instruction according to the datasheet
     * the dalays here do not respect the datasheet. More delay could
     * be crucial depending of the environment the lcd is being used.
    */

    /* We begin by telling the lcd controller to work in 4 bits mode according to the datasheet */
    _delay_ms(50);

    hd44780u_send_4bits_4mode_command(DB4_PIN | DB5_PIN);

    _delay_ms(5);

    hd44780u_send_4bits_4mode_command(DB4_PIN | DB5_PIN);

    _delay_us(150);

    hd44780u_send_4bits_4mode_command(DB4_PIN | DB5_PIN);

    hd44780u_send_4bits_4mode_command(DB5_PIN);

    /** From here we begin to work in 4 bits mode. 4 bits mode activated
     * In order to interface in 4 bits mode, we send 8 bits by group of 2 4 bits commands.
     * The 4 higher bits are sent followed by the 4 lower bits
    */
    hd44780u_send_8bits_4mode_command(HD44780U_2_ROWS | HD44780U_4_BIT_MODE | HD44780U_FONT_5x8);

    hd44780u_send_8bits_4mode_command(HD44780U_DISPLAY_OFF);

    hd44780u_send_8bits_4mode_command(HD44780U_DISPLAY_CLEAR);

    hd44780u_send_8bits_4mode_command(HD44780U_ENTRY_MODE);

    /* Initialization according to the datasheet ends here !!!!*/

    /** From my perspective, when I initialize a display I want it to be turned on, cleared and ready to show input. Therefore I add the following operation (not in the datasheet):
     * Return to home in order to set DDRAM address to 0
     * Turning the display on
     * Turning the backlight on
    */

    hd44780u_return_home();
    hd44780u_display_on();
    hd44780u_backlight_on();


    return HD44780U_INIT_SUCCESS;

    
}
static void hd44780u_send_4bits_4mode_command(uint8_t command)
{
    union hd44780u_lines_u hd44780u_lines;

    hd44780u_lines.hd44780u_lines = command;
    hd44780u_lines.RS = 0;
    hd44780u_lines.RW = 0;
    hd44780u_lines.BT = hd44780u_display_state.backlight ? 1 : 0; /* Enable backlight */

    i2c_transmit(hd44780u_i2c_addr, hd44780u_lines.hd44780u_lines);
    /* Once the data is on the bus, we call safely tell the lcd controller to read the bus */
    hd44780u_e_pin_pulse(hd44780u_lines);

}
static void hd44780u_write_command(uint8_t data)
{
    union hd44780u_lines_u hd44780u_lines;

    hd44780u_lines.hd44780u_lines = data;
    hd44780u_lines.RS = 1;
    hd44780u_lines.RW = 0;
    hd44780u_lines.BT = hd44780u_display_state.backlight ? 1 : 0; /* Enable backlight */

    i2c_transmit(hd44780u_i2c_addr, hd44780u_lines.hd44780u_lines);
    /* Once the data is on the bus, we call safely tell the lcd controller to read the bus */
    hd44780u_e_pin_pulse(hd44780u_lines);

}
static void hd44780u_e_pin_pulse(union hd44780u_lines_u hd44780u_lines)
{
    /* Enable the E pin to indicate an action has to be performed and the data is in the bus */
    hd44780u_lines.E = 1;
    i2c_transmit(hd44780u_i2c_addr, hd44780u_lines.hd44780u_lines);
    _delay_us(1);
    hd44780u_lines.E = 0;
    i2c_transmit(hd44780u_i2c_addr, hd44780u_lines.hd44780u_lines);
    _delay_us(1);
}

static void hd44780u_send_8bits_4mode_command(uint8_t command)
{
    hd44780u_send_4bits_4mode_command(command & 0xf0); /* Send the first upper nibble*/
    hd44780u_send_4bits_4mode_command(command << 4); /* Send the following lower nibble*/
}

void hd44780u_clear_display()
{
    hd44780u_send_8bits_4mode_command(HD44780U_DISPLAY_CLEAR);
    _delay_ms(2); /* This instruction takes the most cycles for the lcd display */
}

void hd44780u_display_on()
{
    hd44780u_display_state.display = hd44780u_display_state.display | HD44780U_DISPLAY_ON;
    hd44780u_send_8bits_4mode_command(hd44780u_display_state.display);
}
void hd44780u_display_off()
{
    hd44780u_display_state.display = hd44780u_display_state.display &(~HD44780U_DISPLAY_ON);
    hd44780u_send_8bits_4mode_command(hd44780u_display_state.display);
}
void hd44780u_cursor_on()
{
    hd44780u_display_state.display = hd44780u_display_state.display | HD44780U_DISPLAY_CURSOR_ON;
    hd44780u_send_8bits_4mode_command(hd44780u_display_state.display);
}
void hd44780u_cursor_off()
{
    hd44780u_display_state.display = hd44780u_display_state.display &(~HD44780U_DISPLAY_CURSOR_ON);
    hd44780u_send_8bits_4mode_command(hd44780u_display_state.display);
}
void hd44780u_blink_on()
{
    hd44780u_display_state.display = hd44780u_display_state.display | HD44780U_DISPLAY_BLINK_ON;
    hd44780u_send_8bits_4mode_command(hd44780u_display_state.display);
}
void hd44780u_blink_off()
{
    hd44780u_display_state.display = hd44780u_display_state.display &(~HD44780U_DISPLAY_BLINK_ON);
    hd44780u_send_8bits_4mode_command(hd44780u_display_state.display);
}

void hd44780u_backlight_on()
{
 hd44780u_display_state.backlight = 1;
 hd44780u_send_8bits_4mode_command(0x0);
}
void hd44780u_backlight_off()
{
    hd44780u_display_state.backlight = 0;
    hd44780u_send_8bits_4mode_command(0x0);
}

void hd44780u_return_home()
{
    hd44780u_send_8bits_4mode_command(HD44780U_HOME);
    _delay_ms(2); /* This instruction takes the most cycles for the lcd display */
}

void hd44780u_print_char(uint8_t character)
{
    hd44780u_write_command(character & 0xf0); /* Send the first upper nibble*/
    //_delay_ms(50);
    hd44780u_write_command(character << 4); /* Send the following lower nibble*/
    //_delay_ms(50);
}
void hd44780u_print_string(char *s_source)
{
    while (*s_source != '\0')
        hd44780u_print_char(*s_source++);
}