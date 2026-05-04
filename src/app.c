#include "app.h"
#include "config.h"
#include "gpio.h"
#include "pins.h"
#include "uart.h"
#include "millis.h"
#include "keypad.h"
#include <string.h>
#include <util/delay.h>

#define CMD_BUFFER_SIZE 32
#define MAX_PIN_LEN 10

static char cmd_buffer[CMD_BUFFER_SIZE];
static unsigned char cmd_index = 0;

typedef enum 
{
    IDLE,
    INPUT_AWAIT,
    ACCESS_GRANTED

} state_t;

uint8_t last_button_state = 1;
state_t current_state = IDLE;
uint32_t start_time;
uint32_t last_blink_time;
uint32_t now;
uint8_t index = 0;
char key;
char entered_pin[4];
char correct_pin[4] = {'1', '2', '3', '4'};


static void process_command(const char *cmd)
{
    if (strcmp(cmd, "led on") == 0)
    {
        gpio_pin_high(&LED_PORT, LED_PIN);
        uart_write_string("LED is ON\n");
    }
    else if (strcmp(cmd, "led off") == 0)
    {
        gpio_pin_low(&LED_PORT, LED_PIN);
        uart_write_string("LED is OFF\n");
    }
    else if (strcmp(cmd, "led toggle") == 0)
    {
        gpio_pin_toggle(&LED_PORT, LED_PIN);
        uart_write_string("LED toggled\n");
    }
    else if (strcmp(cmd, "help") == 0)
    {
        uart_write_string("Commands: help, led on, led off, led toggle\n");
    }
    else
    {
        uart_write_string("Unknown command\n");
    }
}

void handle_idle(){

    gpio_pin_high(&RED_LED_PORT, RED_LED_PIN);
    gpio_pin_low(&GREEN_LED_PORT, GREEN_LED_PIN);

    static char last_key = NO_KEY;
    char k = keypad_scan();

    if (k == 'A' && last_key != 'A') {

        start_time = millis();
        last_blink_time = start_time;
        current_state = INPUT_AWAIT;
    }

    last_key = k;
}

void handle_input()
{
    now = millis();

    if (now - last_blink_time > 500) {
        gpio_pin_toggle(&RED_LED_PORT, RED_LED_PIN);
        last_blink_time = now;
    }

    if (now - start_time >= 5000){
        current_state = IDLE;
        index = 0;
    }

    key = keypad_scan();

    static uint32_t last_key_time = 0;

    if (key != NO_KEY && (millis() - last_key_time > 200)) {
        last_key_time = millis();

        uart_write_string("Pressed ");
        uart_write_char(key);
        uart_write_string("\n");

        gpio_pin_high(&GREEN_LED_PORT, GREEN_LED_PIN);

        _delay_ms(100);
        gpio_pin_low(&GREEN_LED_PORT, GREEN_LED_PIN);

        if (index < 4) {
            entered_pin[index] = key;
            index++;
        }

        
        if (index == 4){
            bool is_correct = true;
            for (int i = 0; i < 4; i++) {
                if (entered_pin[i] != correct_pin[i]) {
                    is_correct = false;
                    break;
                }
            }
            if (is_correct) {
                current_state = ACCESS_GRANTED;
                start_time = millis();
            }
            else {
                current_state = IDLE;
            }
            index = 0;
        }
    }
}

void handle_access()
{
    gpio_pin_low(&RED_LED_PORT, RED_LED_PIN);
    gpio_pin_high(&GREEN_LED_PORT, GREEN_LED_PIN);

    if (millis() - start_time >= 3000){
        current_state = IDLE;
    }
}

void app_init(void)
{
    gpio_pin_output(&LED_DDR, LED_PIN);
    gpio_pin_low(&LED_PORT, LED_PIN);

    gpio_pin_output(&RED_LED_DDR, RED_LED_PIN);
    gpio_pin_output(&GREEN_LED_DDR, GREEN_LED_PIN);

    gpio_pin_input_pullup(&BUTTON_DDR, &BUTTON_PORT, BUTTON_PIN);

    keypad_init();

    millis_init();

    uart_init(UART_BAUDRATE);
    uart_write_string("System ready\n");
    uart_write_string("Type: help\n");
}

void app_run(void)
{
    char c;

    while (1)
    {
        while (uart_read_char(&c))
        {
            uart_write_char(c);

            if (c == '\r' || c == '\n')
            {
                uart_write_string("\n");

                if (cmd_index > 0)
                {
                    cmd_buffer[cmd_index] = '\0';
                    process_command(cmd_buffer);
                    cmd_index = 0;
                }
            }
            else
            {
                if (cmd_index < (CMD_BUFFER_SIZE - 1))
                {
                    cmd_buffer[cmd_index++] = c;
                }
                else
                {
                    cmd_index = 0;
                    uart_write_string("\nCommand too long\n");
                }
            }
        }

        switch (current_state) 
        {
            case IDLE:
                handle_idle();
                break;
            
            case INPUT_AWAIT:
                handle_input();
                break;
            
            case ACCESS_GRANTED:
                handle_access();
                break;
        }
    }
}
