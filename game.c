#include "game.h"
#include "graphics.h"

//*************************************************************************************************

#define PADDLE_WIDTH 24
#define PADDLE_HEIGHT 4
#define PADDLE_MOVEMENT_PER_UPDATE_SLOW 1
#define PADDLE_MOVEMENT_PER_UPDATE_FAST 2
const uint16_t MAX_PADDLE_X_POSITION = X_WIDTH - PADDLE_WIDTH;
volatile uint16_t paddle_x_position = 0;
volatile uint16_t paddle_y_position = 0;

#define BRICK_WIDTH 32
#define BRICK_HEIGHT 15
#define BRICK_COLUMNS 8
#define BRICK_ROWS 6
#define BRICK_SPACING 7
#define TOP_MARGIN 20

//*************************************************************************************************

uint8_t get_row_color(int row)
{
    switch (row)
    {
        case 0:
        case 3:
            return BLUE;
        case 1:
        case 4:
            return GREEN;
        case 2:
        case 5:
            return WHITE;
        default:
            return WHITE;
    }
}

//*************************************************************************************************

void init_game()
{
    // init the paddle
    paddle_x_position = (X_WIDTH >> 1) - (PADDLE_WIDTH >> 1);
    paddle_y_position = Y_HEIGHT - (PADDLE_HEIGHT << 1) - 8;
    fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

    // init the bricks
    // TODO: need to store a bool array that says whether or not each brick is still visible
    // TODO: reduce the brick height by a bit, then increase the spacing, need separate spacing for x and y?
    for (int row = 0; row < BRICK_ROWS; row++)
    {
        uint8_t color = get_row_color(row);

        for (int column = 0; column < BRICK_COLUMNS; column++)
        {
            uint16_t x = BRICK_SPACING * (1 + column) + BRICK_WIDTH * column;
            uint16_t y = TOP_MARGIN + BRICK_SPACING * row + BRICK_HEIGHT * row;
            fill_rect(x, y, BRICK_WIDTH, BRICK_HEIGHT, color);
        }
    }
}

//*************************************************************************************************

void update_paddle(uint16_t joystick)
{
    if (joystick > 3500)
    {
        // joystick was moved all the way to the right
        
        // only do something if there is room to move it further right
        if (paddle_x_position < MAX_PADDLE_X_POSITION)
        {
            // erase the old one first
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);

            // update the paddle position
            paddle_x_position += PADDLE_MOVEMENT_PER_UPDATE_FAST;

            // draw the new one
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
        }
    }
    else if (joystick > 2500)
    {
        // joystick was moved partially to the right

        // only do something if there is room to move it further right
        if (paddle_x_position < MAX_PADDLE_X_POSITION)
        {
            // erase the old one first
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);

            // update the paddle position
            paddle_x_position += PADDLE_MOVEMENT_PER_UPDATE_SLOW;

            // draw the new one
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
        }
    }
    else if (joystick < 500)
    {
        // joystick was moved all the way to the left

        // only do something if there is room to move it further left
        if (paddle_x_position > 0)
        {
            // erase the old one first
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);

            // find the new position, using either a full step or partial step
            uint16_t step = 
                (paddle_x_position >= PADDLE_MOVEMENT_PER_UPDATE_FAST) ? PADDLE_MOVEMENT_PER_UPDATE_FAST : paddle_x_position;
            paddle_x_position -= step;

            // draw the new one
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
        }
    }
    else if (joystick < 1500)
    {
        // joystick was moved partially to the left

        // only do something if there is room to move it further left
        if (paddle_x_position > 0)
        {
            // erase the old one first
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);

            // find the new position, using either a full step or partial step
            uint16_t step = 
                (paddle_x_position >= PADDLE_MOVEMENT_PER_UPDATE_SLOW) ? PADDLE_MOVEMENT_PER_UPDATE_SLOW : paddle_x_position;
            paddle_x_position -= step;

            // draw the new one
            fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
        }
    }
    else
    {
        // joystick is in the neutral position

        // TODO: remove this, it's just simulating the appearance of the ball
        fill_rect(X_WIDTH >> 1, Y_HEIGHT >> 1, 2, 2, WHITE);
    }
}
