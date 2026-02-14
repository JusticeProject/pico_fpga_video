#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"

#include "game.h"
#include "graphics.h"

#define JOYSTICK_GPIO 26

#define LED_GPIO 15

volatile uint32_t counter0 = 0;
volatile uint32_t counter1 = 0;

volatile uint16_t joystick_global = 2048; // initialized to the middle of the 12 bit ADC
spin_lock_t *joystick_spinlock;

//*************************************************************************************************

bool timer_interrupt_io(struct repeating_timer *t)
{
    counter0++;

    // tell the FPGA we are about to send pixel data
    prepare_for_data_tx();

    // read the joystick
    uint16_t current_adc = adc_read(); // takes 96 cycles (at 48MHz?) = 2us
    spin_lock_unsafe_blocking(joystick_spinlock); // does not disable interrupts
    joystick_global = current_adc;
    spin_unlock_unsafe(joystick_spinlock);

    // send pixel data to FPGA
    do_data_tx();

    return true; // tell the system to continue the timer
}

//*************************************************************************************************

bool timer_interrupt_game(struct repeating_timer *t)
{
    counter1++;

    spin_lock_unsafe_blocking(joystick_spinlock);
    uint16_t joystick = joystick_global;
    spin_unlock_unsafe(joystick_spinlock);

    // update game state and update the frame buffer
    update_game_state(joystick);

    return true; // tell the system to continue the timer
}

//*************************************************************************************************

void core1_entry()
{
    // initialize game state
    init_game();

    // repeating timer, once every 16800us, ~60Hz
    // each core has its own timer pool and its own interrupt
    struct repeating_timer timer;
    add_repeating_timer_us(-16800, timer_interrupt_game, NULL, &timer);

    while (true)
    {
        // nothing else to do
        sleep_ms(1000);
    }
}

//*************************************************************************************************

int main()
{
    // overclocking to 250MHz, done first before anything else that relies on the clock
    set_sys_clock_khz(250000, true);

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

    // spinlock is used to synchronize between the two cores
    int spin_num = spin_lock_claim_unused(true);
    joystick_spinlock = spin_lock_init(spin_num);

    // launch the core that handles the game loop
    multicore_launch_core1(core1_entry);

    // this will stagger the two timer interrupts
    sleep_ms(2);

    // Repeating timer once every 16800us (~60Hz) for the I/O which includes 
    // reading the joystick and sending data to the FPGA.
    // We are trying to match the FPGA timing which refreshes the screen every:
    // (1/25MHz) * 800 * 525 = 0.0168 seconds = 16800 us
    struct repeating_timer timer;
    add_repeating_timer_us(-16800, timer_interrupt_io, NULL, &timer);

    // keep looping, accepts commands, blinks an LED
    absolute_time_t ledChangedTime = get_absolute_time();
    uint8_t currentColor = BLACK;
    while (true)
    {
        int cmd = getchar_timeout_us(0);

        if ('s' == cmd)
        {
            printf("joystick_global = %d, counter0 = %d, counter1 = %d\n", joystick_global, counter0, counter1);
            printf("clk_sys = %d, success = %d\n", clock_get_hz(clk_sys), success ? 1 : 0);
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
            draw_pixel(X_SCREEN_WIDTH-1, Y_SCREEN_HEIGHT-1, WHITE);
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
