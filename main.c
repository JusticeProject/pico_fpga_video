#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

#include "graphics.h"

#define JOYSTICK_GPIO 26

#define LED_GPIO 15

volatile uint16_t current_adc = 0;
volatile uint32_t counter = 0;

//*************************************************************************************************

bool timer_interrupt(struct repeating_timer *t)
{
    // tell the FPGA we are about to send pixel data
    prepare_for_data_tx();

    counter++;
    current_adc = adc_read();

    // TODO: update game state
    // TODO: what if updating the game state takes too long? the start signal could be sent while still sending data
    if (current_adc > 3500)
    {
        fill_screen(BLUE);
    }
    else if (current_adc < 500)
    {
        fill_screen(GREEN);
    }
    else
    {
        draw_pixel(X_WIDTH >> 1, Y_HEIGHT >> 1, WHITE);
    }

    // send pixel data to FPGA
    do_data_tx();

    return true; // tell the system to continue the timer
}

//*************************************************************************************************

int main()
{
    stdio_init_all();

    // we will blink this LED so we know the program is running
    gpio_init(LED_GPIO);
    gpio_set_dir(LED_GPIO, GPIO_OUT);

    // setup the ADC
    adc_init();
    adc_gpio_init(JOYSTICK_GPIO);
    adc_select_input(0);

    // init graphics
    bool success = init_graphics();

    // repeating timer, once every 33ms, ~30Hz
    struct repeating_timer timer;
    add_repeating_timer_ms(-33, timer_interrupt, NULL, &timer);

    // keep looping, accepts commands, blinks an LED
    absolute_time_t ledChangedTime = get_absolute_time();
    uint8_t currentColor = BLACK;
    while (true)
    {
        int cmd = getchar_timeout_us(0);

        if ('s' == cmd)
        {
            printf("counter = %d  current_adc = %d\n", counter, current_adc);
            printf("success = %d\n", success ? 1 : 0);
        }
        else if ('w' == cmd)
        {
            fill_screen(WHITE);
        }
        else if ('b' == cmd)
        {
            fill_screen(BLACK);
        }
        else if ('d' == cmd)
        {
            draw_pixel(0, 0, WHITE);
        }
        else if ('e' == cmd)
        {
            draw_pixel(X_WIDTH-1, Y_HEIGHT-1, WHITE);
        }

        absolute_time_t now = get_absolute_time();
        int64_t diff_us = absolute_time_diff_us(ledChangedTime, now);
        int64_t diff_ms = diff_us / 1000;
        if (diff_ms > 5000)
        {
            ledChangedTime = now;
            gpio_put(LED_GPIO, !gpio_get(LED_GPIO)); // toggle the LED

            // change the screen color
            currentColor++;
            currentColor &= 0x3;
            //fill_screen(currentColor);
        }

        sleep_ms(1);
    }
}
