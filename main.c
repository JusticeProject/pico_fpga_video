#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

#include "start_signal.pio.h"

#define JOYSTICK_GPIO 26

#define LED_GPIO 15

#define START_SIGNAL_GPIO 0
static PIO start_pio;
static uint start_sm = 0;

volatile uint16_t current_adc = 0;
volatile uint32_t counter = 0;

//*************************************************************************************************

bool start_signal_program_init()
{
    uint offset;
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &start_signal_program, &start_pio, &start_sm, &offset, START_SIGNAL_GPIO, 1, true);
    
    if (!success)
    {
        return false;
    }

    pio_gpio_init(start_pio, START_SIGNAL_GPIO);
    pio_sm_set_consecutive_pindirs(start_pio, start_sm, START_SIGNAL_GPIO, 1, true);
    pio_sm_config c = start_signal_program_get_default_config(offset);
    sm_config_set_set_pins(&c, START_SIGNAL_GPIO, 1);
    pio_sm_init(start_pio, start_sm, offset, &c);
    pio_sm_set_enabled(start_pio, start_sm, true);

    return true;
}

//*************************************************************************************************

bool timer_interrupt(struct repeating_timer *t)
{
    // send start signal
    pio_sm_put(start_pio, start_sm, 1);

    counter++;
    current_adc = adc_read();

    // TODO: update game state

    // TODO: start dma transfer of frame buffer, but what if updating the game state takes too long?

    return true; // tell the system to continue the timer
}

//*************************************************************************************************

int main()
{
    stdio_init_all();

    // blink an LED so we know the program is running
    gpio_init(LED_GPIO);
    gpio_set_dir(LED_GPIO, GPIO_OUT);

    adc_init();
    adc_gpio_init(JOYSTICK_GPIO);
    adc_select_input(0);

    struct repeating_timer timer;
    add_repeating_timer_ms(-33, timer_interrupt, NULL, &timer);

    bool success = start_signal_program_init();

    while (true)
    {
        printf("counter = %d  current_adc = %d\n", counter, current_adc);
        printf("success = %d\n", success ? 1 : 0);
        gpio_put(LED_GPIO, 0);
        sleep_ms(1000);
        gpio_put(LED_GPIO, 1);
        sleep_ms(1000);
    }
}
