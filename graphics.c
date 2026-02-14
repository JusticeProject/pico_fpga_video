#include "hardware/dma.h"
#include "hardware/pio.h"

#include "start_signal.pio.h"
#include "data_transfer.pio.h"

#include "graphics.h"

//*************************************************************************************************

#define START_SIGNAL_GPIO 0
static PIO start_pio;
static uint start_sm = 0;

#define DATA_READY_GPIO 1
#define BLUE_GPIO 2
#define GREEN_GPIO 3
static PIO data_pio;
static uint data_sm = 0;

volatile int dma_channel = -1;

// frame buffer size: 320 x 240 pixels, each pixel uses 2 bits, 8 bits/byte, 320*240*2/8 = 19200
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

bool init_graphics()
{
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

    return success;
}

//*************************************************************************************************

void prepare_for_data_tx()
{
    // send the start signal
    pio_sm_put(start_pio, start_sm, 1);
}

//*************************************************************************************************

void do_data_tx()
{
    // start dma transfer of frame buffer
    dma_channel_set_read_addr(dma_channel, &frame_buf, false);
    dma_channel_start(dma_channel);
}

//*************************************************************************************************

void fill_screen(uint8_t color)
{
    uint8_t pattern = (color << 6) | (color << 4) | (color << 2) | (color);

    for (int i = 0; i < FRAME_BUFFER_SIZE_BYTES; i++)
    {
        frame_buf[i] = pattern;
    }
}

//*************************************************************************************************

void draw_pixel(uint16_t x, uint16_t y, uint8_t color)
{
    // input bounds checking
    if (x >= X_SCREEN_WIDTH || y >= Y_SCREEN_HEIGHT)
    {
        return;
    }

    int index = y*80 + (x >> 2); // 80 bytes per horz line, 4 pixels per byte, shift right by 2 is divide by 4
    int blue_index = (x & 0x3) << 1; // (remainder when dividing by 4) * (2)
    //int green_index = blue_index + 1;

    frame_buf[index] &= ~(0x3 << blue_index); // clear the bits
    frame_buf[index] |= (color << blue_index); // set the bits
}

//*************************************************************************************************

void draw_horz_line(uint16_t x_start, uint16_t y_start, uint16_t length, uint8_t color)
{
    // TODO: can I speed this up with memset?

    /*for (int i = 0; i < length; i++)
    {
        draw_pixel(x_start + i, y_start, color);
    }*/

    uint16_t x_stop = x_start + length;
    for (uint16_t x = x_start; x < x_stop; x++)
    {
        draw_pixel(x, y_start, color);
    }
}

//*************************************************************************************************

void fill_rect(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint8_t color)
{
    uint16_t y_stop = y_start + height;
    for (uint16_t y = y_start; y < y_stop; y++)
    {
        draw_horz_line(x_start, y, width, color);
    }
}
