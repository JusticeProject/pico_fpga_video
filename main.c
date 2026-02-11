#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

#include "start_signal.pio.h"
#include "data_transfer.pio.h"

#define JOYSTICK_GPIO 26

#define LED_GPIO 15

#define START_SIGNAL_GPIO 0
static PIO start_pio;
static uint start_sm = 0;

#define DATA_READY_GPIO 1
#define BLUE_GPIO 2
#define GREEN_GPIO 3
static PIO data_pio;
static uint data_sm = 0;

volatile int dma_channel = -1;

volatile uint16_t current_adc = 0;
volatile uint32_t counter = 0;

// frame buffer, 320 x 240 pixels, each pixel uses 2 bits, 8 bits/byte, 320*240*2/8 = 19200
#define FRAME_BUFFER_SIZE_BYTES 19200
uint8_t frame_buf[FRAME_BUFFER_SIZE_BYTES];

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

bool data_transfer_program_init()
{
    uint offset;
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &data_transfer_program, &data_pio, &data_sm, &offset, BLUE_GPIO, 2, true);
    
    if (!success)
    {
        return false;
    }

    pio_gpio_init(data_pio, DATA_READY_GPIO);
    pio_gpio_init(data_pio, BLUE_GPIO);
    pio_gpio_init(data_pio, GREEN_GPIO);
    pio_sm_set_consecutive_pindirs(data_pio, data_sm, DATA_READY_GPIO, 3, true);

    pio_sm_config c = data_transfer_program_get_default_config(offset);
    sm_config_set_set_pins(&c, DATA_READY_GPIO, 1);
    sm_config_set_out_pins(&c, BLUE_GPIO, 2);
    sm_config_set_out_shift(&c, true, false, 8); // shift right (LSB goes out first), no autopull, pull every 8 bits
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    pio_sm_init(data_pio, data_sm, offset, &c);
    pio_sm_set_enabled(data_pio, data_sm, true);

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
    // TODO: what if updating the game state takes too long? the start signal could be sent while still sending data

    // start dma transfer of frame buffer
    //dma_hw->ch[dma_channel].read_addr = &frame_buf;
    //dma_channel_hw_addr(dma_channel)->read_addr = (void*)&frame_buf;
    dma_channel_set_read_addr(dma_channel, &frame_buf, false);
    dma_channel_start(dma_channel);

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

    // setup the PIO, this needs to be done before the DMA setup so we have data_pio and data_sm set to values
    bool success = start_signal_program_init();
    success &= data_transfer_program_init();

    // setup the DMA, we will transfer from the frame buffer to the PIO
    dma_channel = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(data_pio, data_sm, true));
    dma_channel_configure(dma_channel, &c,
        &data_pio->txf[data_sm],        // Destination pointer: the Tx FIFO of the PIO
        &frame_buf,                     // Source pointer: the frame buffer
        FRAME_BUFFER_SIZE_BYTES,        // Number of transfers
        false                           // Don't start yet
    );

    // repeating timer, once every 33ms, ~30Hz
    struct repeating_timer timer;
    add_repeating_timer_ms(-33, timer_interrupt, NULL, &timer);

    // keep looping, accepts commands, blinks an LED
    absolute_time_t ledChangedTime = get_absolute_time();
    while (true)
    {
        int cmd = getchar_timeout_us(0);

        if ('s' == cmd)
        {
            printf("counter = %d  current_adc = %d\n", counter, current_adc);
            printf("success = %d\n", success ? 1 : 0);
        }
        else if ('0' == cmd)
        {
            pio_sm_put(data_pio, data_sm, 0x00);
        }
        else if ('1' == cmd)
        {
            pio_sm_put(data_pio, data_sm, 0x00);
            pio_sm_put(data_pio, data_sm, 0xF0);
        }
        else if ('w' == cmd)
        {
            for (int i = 0; i < FRAME_BUFFER_SIZE_BYTES; i++)
            {
                frame_buf[i] = 0xFF;
            }
        }
        else if ('b' == cmd)
        {
            for (int i = 0; i < FRAME_BUFFER_SIZE_BYTES; i++)
            {
                frame_buf[i] = 0x00;
            }
        }
        else if ('d' == cmd)
        {
            bool busy = dma_channel_is_busy(dma_channel);
            printf("DMA busy = %d\n", busy ? 1 : 0);
            dma_channel_set_read_addr(dma_channel, &frame_buf, false);
            dma_channel_start(dma_channel);
            busy = dma_channel_is_busy(dma_channel);
            printf("DMA busy = %d\n", busy ? 1 : 0);
            sleep_ms(1000);
            busy = dma_channel_is_busy(dma_channel);
            printf("DMA busy = %d\n", busy ? 1 : 0);
        }

        absolute_time_t now = get_absolute_time();
        int64_t diff_us = absolute_time_diff_us(ledChangedTime, now);
        int64_t diff_ms = diff_us / 1000;
        if (diff_ms > 5000)
        {
            ledChangedTime = now;
            gpio_put(LED_GPIO, !gpio_get(LED_GPIO)); // toggle the LED

            // toggle the frame buffer bits
            for (int i = 0; i < FRAME_BUFFER_SIZE_BYTES; i++)
            {
                frame_buf[i] = ~frame_buf[i];
            }
        }
    }
}
