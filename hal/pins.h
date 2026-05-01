#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_PIN   PB5

#define RED_LED_PORT PORTD
#define RED_LED_DDR  DDRD
#define RED_LED_PIN  PD3

#define GREEN_LED_PORT PORTD
#define GREEN_LED_DDR DDRD
#define GREEN_LED_PIN PD2

#define BUTTON_PINR PINB
#define BUTTON_PORT PORTB
#define BUTTON_DDR DDRB
#define BUTTON_PIN PB4

#define KEY_DDR DDRB
#define KEY_PORT PORTB
#define KEY_ROW_1 PB3
#define KEY_ROW_2 PB2
#define KEY_ROW_3 PB1
#define KEY_ROW_4 PB0

#define COLUMN_DDR DDRD
#define COLUMN_PINR PIND
#define COLUMN_PORT PORTD
#define COLUMN_1 PD7
#define COLUMN_2 PD6
#define COLUMN_3 PD5
#define COLUMN_4 PD4

#endif