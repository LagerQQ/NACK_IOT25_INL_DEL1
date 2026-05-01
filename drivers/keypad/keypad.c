#include "keypad.h"
#include "config.h"
#include "pins.h"
#include "gpio.h"
#include <util/delay.h>
#include "uart.h"

uint8_t read_column(void) {
    uint8_t col = 
        ((COLUMN_PINR & (1 << COLUMN_1)) ? 1 : 0) |
        ((COLUMN_PINR & (1 << COLUMN_2)) ? 2 : 0) |
        ((COLUMN_PINR & (1 << COLUMN_3)) ? 4 : 0) |
        ((COLUMN_PINR & (1 << COLUMN_4)) ? 8 : 0);
    
    if (col == 1 || col == 2 || col == 4 || col == 8) {
        return col;
    }
    
    return 0;
}

void keypad_init(void) {
    gpio_pin_output(&KEY_DDR, KEY_ROW_1);
    gpio_pin_output(&KEY_DDR, KEY_ROW_2);
    gpio_pin_output(&KEY_DDR, KEY_ROW_3);
    gpio_pin_output(&KEY_DDR, KEY_ROW_4);
    
    gpio_pin_high(&KEY_PORT, KEY_ROW_1);
    gpio_pin_high(&KEY_PORT, KEY_ROW_2);
    gpio_pin_high(&KEY_PORT, KEY_ROW_3);
    gpio_pin_high(&KEY_PORT, KEY_ROW_4);
    
    gpio_pin_input_pullup(&COLUMN_DDR, &COLUMN_PORT, COLUMN_1);
    gpio_pin_input_pullup(&COLUMN_DDR, &COLUMN_PORT, COLUMN_2);
    gpio_pin_input_pullup(&COLUMN_DDR, &COLUMN_PORT, COLUMN_3);
    gpio_pin_input_pullup(&COLUMN_DDR, &COLUMN_PORT, COLUMN_4);
    
}

void set_all_rows_high(void){
    gpio_pin_high(&KEY_PORT, KEY_ROW_1);
    gpio_pin_high(&KEY_PORT, KEY_ROW_2);
    gpio_pin_high(&KEY_PORT, KEY_ROW_3);
    gpio_pin_high(&KEY_PORT, KEY_ROW_4);
}

char keypad_scan(void) {
    
    uint8_t col;

    set_all_rows_high();
    _delay_us(5);

    //debugg
    // uart_write_char((COLUMN_PINR & (1 << COLUMN_1)) ? 'H' : 'L');
    // uart_write_char((COLUMN_PINR & (1 << COLUMN_2)) ? 'H' : 'L');
    // uart_write_char((COLUMN_PINR & (1 << COLUMN_3)) ? 'H' : 'L');
    // uart_write_char((COLUMN_PINR & (1 << COLUMN_4)) ? 'H' : 'L');
    // uart_write_string("\n");

    //R1
    gpio_pin_low(&KEY_PORT, KEY_ROW_1);
    _delay_us(5);

    col = read_column();

    if (col == 1) return '1';
    if (col == 2) return '2';
    if (col == 4) return '3';
    if (col == 8) return 'A';

    set_all_rows_high();
    _delay_us(5);

    //R2
    gpio_pin_low(&KEY_PORT, KEY_ROW_2);
    _delay_us(5);

    col = read_column();

    if (col == 1) return '4';
    if (col == 2) return '5';
    if (col == 4) return '6';
    if (col == 8) return 'B';

    set_all_rows_high();
    _delay_us(5);

    //R3
    gpio_pin_low(&KEY_PORT, KEY_ROW_3);
    _delay_us(5);

    col = read_column();

    if (col == 1) return '7';
    if (col == 2) return '8';
    if (col == 4) return '9';
    if (col == 8) return 'C';

    set_all_rows_high();
    _delay_us(5);

    //R4
    gpio_pin_low(&KEY_PORT, KEY_ROW_4);
    _delay_us(5);

    col = read_column();

    if (col == 1) return '*';
    if (col == 2) return '0';
    if (col == 4) return '#';
    if (col == 8) return 'D';

    set_all_rows_high();
    _delay_us(5);

    return NO_KEY;
}