#include <avr/io.h>
#include "HD44780U.h"
#include "i2c.h"
#include "uart.h"

int main()
{
    init_hd44780u(0x27, 0);
    backlight_on(0x27);
    cursor_blink(0x27);

    USART_Init(B9600);
    while(1)
    {
        print_char(0x27, USART_Receive());
    }


}