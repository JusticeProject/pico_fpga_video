#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

#include "start_signal.pio.h"

#define JOYSTICK_GPIO 26

volatile uint16_t current_adc = 0;
volatile uint32_t counter = 0;

/*void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    start_signal_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (125000000 / (2 * freq)) - 3;
}*/

bool timer_interrupt(struct repeating_timer *t)
{
    // TODO: send start signal

    counter++;
    current_adc = adc_read();

    // TODO: update game state

    // TODO: start dma transfer of frame buffer, but what if updating the game state takes too long?

    return true; // tell the system to continue the timer
}

int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(JOYSTICK_GPIO);
    adc_select_input(0);

    struct repeating_timer timer;
    add_repeating_timer_ms(-33, timer_interrupt, NULL, &timer);

    // PIO Blinking example
    /*PIO pio = pio0;
    uint offset = pio_add_program(pio, &start_signal_program);
    printf("Loaded program at %d\n", offset);
    blink_pin_forever(pio, 0, offset, PICO_DEFAULT_LED_PIN, 3);*/

    while (true)
    {
        //uint16_t current_adc = adc_read();
        printf("counter = %d  current_adc = %d\n", counter, current_adc);
        //printf("fifo_level_before = %d  fifo_level_after = %d\n", fifo_level_before, fifo_level_after);
        sleep_ms(5000);
    }
}
