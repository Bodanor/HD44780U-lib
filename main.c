#include <avr/io.h>
#include "HD44780U.h"
#include "i2c.h"
#include "uart.h"

int main()
{
    init_hd44780u(0x27, 0);
    hd44780u_print_string("Hello World !");
    
}