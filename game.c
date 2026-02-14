#include "game.h"
#include "graphics.h"

//*************************************************************************************************

#define PADDLE_WIDTH 24
#define PADDLE_HEIGHT 4
#define PADDLE_MOVEMENT_PER_UPDATE_SLOW 1
#define PADDLE_MOVEMENT_PER_UPDATE_FAST 2
const uint16_t MAX_PADDLE_X_POSITION = X_SCREEN_WIDTH - PADDLE_WIDTH;
volatile uint16_t paddle_x_position = 0;
volatile uint16_t paddle_y_position = 0;

//*************************************************************************************************

#define BRICK_WIDTH 32
#define BRICK_HEIGHT 12
#define BRICK_COLUMNS 8
#define BRICK_ROWS 6
#define BRICK_SPACING_X 7
#define BRICK_SPACING_Y 10
#define TOP_MARGIN 20

typedef struct
{
    uint16_t x_left;
    uint16_t x_right;
    uint16_t y_top;
    uint16_t y_bottom;
    bool visible;
}brick_info_t;

volatile brick_info_t bricks[BRICK_ROWS][BRICK_COLUMNS];

//*************************************************************************************************

#define BALL_DIAMETER 3
#define BALL_SPEED 2

volatile int16_t ball_x_position = 0;
volatile int16_t ball_y_position = 0;

volatile int8_t ball_x_speed = 0;
volatile int8_t ball_y_speed = 0;

//*************************************************************************************************

typedef enum
{
    STATE_POWERUP = 0,
    STATE_RUNNING = 1
}state_t;

state_t game_state = STATE_POWERUP;

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
    paddle_x_position = (X_SCREEN_WIDTH >> 1) - (PADDLE_WIDTH >> 1);
    paddle_y_position = Y_SCREEN_HEIGHT - (PADDLE_HEIGHT << 1) - 8;
    fill_rect(paddle_x_position, paddle_y_position, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

    // init the bricks
    for (int row = 0; row < BRICK_ROWS; row++)
    {
        uint8_t color = get_row_color(row);

        for (int column = 0; column < BRICK_COLUMNS; column++)
        {
            uint16_t x = BRICK_SPACING_X * (1 + column) + BRICK_WIDTH * column;
            uint16_t y = TOP_MARGIN + BRICK_SPACING_Y * row + BRICK_HEIGHT * row;
            fill_rect(x, y, BRICK_WIDTH, BRICK_HEIGHT, color);

            brick_info_t binfo = 
                {.x_left = x, .x_right = x + BRICK_WIDTH - 1, .y_top = y, .y_bottom = y + BRICK_HEIGHT - 1, .visible = true};
            bricks[row][column] = binfo;
        }
    }

    // init the ball
    ball_x_position = paddle_x_position + (PADDLE_WIDTH >> 1);
    ball_y_position = paddle_y_position - BALL_DIAMETER;
    fill_rect(ball_x_position, ball_y_position, BALL_DIAMETER, BALL_DIAMETER, WHITE);
}

//*************************************************************************************************

void check_brick_collision()
{
    for (int row = 0; row < BRICK_ROWS; row++)
    {
        for (int column = 0; column < BRICK_COLUMNS; column++)
        {
            brick_info_t brick = bricks[row][column];

            if (!brick.visible)
            {
                continue;
            }

            if ((ball_x_position > brick.x_left) &&   // TODO: (ball_x_position + BALL_DIAMETER - 1) > brick.x_left
                (ball_x_position < brick.x_right) && 
                (ball_y_position > brick.y_top) &&    // TODO: (ball_y_position + BALL_DIAMETER - 1) > brick.y_top
                (ball_y_position < brick.y_bottom))
            {
                brick.visible = false;
                fill_rect(brick.x_left, brick.y_top, BRICK_WIDTH, BRICK_HEIGHT, BLACK);

                // TODO: need to determine what side of the brick was hit, that will determine which speed will flip
                /*if ()
                {
                    // ball is moving up and left, only reverse the y?
                }
                else if ()
                {
                }
                ball_x_speed = -ball_x_speed;
                ball_y_speed = -ball_y_speed;*/

                // no need to check the other bricks if this one was hit
                return;
            }
        }
    }
}

//*************************************************************************************************

void update_ball()
{
    // erase ball
    fill_rect(ball_x_position, ball_y_position, BALL_DIAMETER, BALL_DIAMETER, BLACK);

    ball_x_position += ball_x_speed;
    if (ball_x_position < 0)
    {
        ball_x_speed = BALL_SPEED;
        ball_x_position += (ball_x_speed << 1);
    }
    else if (ball_x_position >= X_SCREEN_WIDTH)
    {
        ball_x_speed = -BALL_SPEED;
        ball_x_position += (ball_x_speed << 1);
    }

    ball_y_position += ball_y_speed;
    if (ball_y_position < 0)
    {
        ball_y_speed = BALL_SPEED;
        ball_y_position += (ball_y_speed << 1);
    }
    else if (ball_y_position >= Y_SCREEN_HEIGHT)
    {
        ball_y_speed = -BALL_SPEED;
        ball_y_position += (ball_y_speed << 1);
    }

    // TODO: bounce off the paddle, check_paddle_collision()

    // hide any bricks that are hit, which will also reverse the speed
    check_brick_collision();

    // draw new position of ball
    fill_rect(ball_x_position, ball_y_position, BALL_DIAMETER, BALL_DIAMETER, WHITE);
}

//*************************************************************************************************

bool update_paddle(uint16_t joystick)
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
            return true;
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
            return true;
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
            return true;
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
            return true;
        }
    }
    else
    {
        // joystick is in the neutral position
        return false;
    }

    return false;
}

//*************************************************************************************************

void update_game_state(uint16_t joystick)
{
    bool paddle_moved = update_paddle(joystick);

    // TODO: make paddle wider? slow down ball?

    switch (game_state)
    {
        case STATE_POWERUP:
            if (paddle_moved)
            {
                game_state = STATE_RUNNING;
                ball_x_speed = -BALL_SPEED;
                ball_y_speed = -BALL_SPEED;
            }
            break;
        case STATE_RUNNING:
            update_ball();
            break;
    }
}
