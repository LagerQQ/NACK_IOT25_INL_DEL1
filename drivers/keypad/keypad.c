#include "keypad.h"
#include "config.h"
#include "pins.h"
#include "gpio.h"
#include <util/delay.h>
#include "uart.h"

uint8_t read_column(void) {
    // uint8_t col = 
       if (!(COLUMN_PINR & (1 << COLUMN_1))) return 1;
       if (!(COLUMN_PINR & (1 << COLUMN_2))) return 2;
       if (!(COLUMN_PINR & (1 << COLUMN_3))) return 4;
       if (!(COLUMN_PINR & (1 << COLUMN_4))) return 8;
    
    // if (col == 1 || col == 2 || col == 4 || col == 8) {
    //     return col;
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

char keypad_scan(void)
{
    uint8_t col;
    char key = NO_KEY;
    static char last_returned = NO_KEY;

    // R1
    set_all_rows_high();
    _delay_us(5);
    gpio_pin_low(&KEY_PORT, KEY_ROW_1);
    _delay_us(5);

    uint8_t c1 = read_column();
    _delay_us(5);
    uint8_t c2 = read_column();
    _delay_us(5);
    uint8_t c3 = read_column();

    col = (c1 == c2 || c1 == c3) ? c1 :
          (c2 == c3)             ? c2 : 0;

    if (col == 1) key = '1';
    else if (col == 2) key = '2';
    else if (col == 4) key = '3';
    else if (col == 8) key = 'A';

    if (key == NO_KEY) {
        // R2
        set_all_rows_high();
        _delay_us(5);
        gpio_pin_low(&KEY_PORT, KEY_ROW_2);
        _delay_us(5);

        c1 = read_column();
        _delay_us(5);
        c2 = read_column();
        _delay_us(5);
        c3 = read_column();

        col = (c1 == c2 || c1 == c3) ? c1 :
              (c2 == c3)             ? c2 : 0;

        if (col == 1) key = '4';
        else if (col == 2) key = '5';
        else if (col == 4) key = '6';
        else if (col == 8) key = 'B';
    }

    if (key == NO_KEY) {
        // R3
        set_all_rows_high();
        _delay_us(5);
        gpio_pin_low(&KEY_PORT, KEY_ROW_3);
        _delay_us(5);

        c1 = read_column();
        _delay_us(5);
        c2 = read_column();
        _delay_us(5);
        c3 = read_column();

        col = (c1 == c2 || c1 == c3) ? c1 :
              (c2 == c3)             ? c2 : 0;

        if (col == 1) key = '7';
        else if (col == 2) key = '8';
        else if (col == 4) key = '9';
        else if (col == 8) key = 'C';
    }

    if (key == NO_KEY) {
        // R4
        set_all_rows_high();
        _delay_us(5);
        gpio_pin_low(&KEY_PORT, KEY_ROW_4);
        _delay_us(5);

        c1 = read_column();
        _delay_us(5);
        c2 = read_column();
        _delay_us(5);
        c3 = read_column();

        col = (c1 == c2 || c1 == c3) ? c1 :
              (c2 == c3)             ? c2 : 0;

        if (col == 1) key = '*';
        else if (col == 2) key = '0';
        else if (col == 4) key = '#';
        else if (col == 8) key = 'D';
    }

    set_all_rows_high();

    if (key != NO_KEY && key != last_returned) {
        last_returned = key;
        return key;
    }

    if (key == NO_KEY) {
        last_returned = NO_KEY;
    }

    return NO_KEY;
}