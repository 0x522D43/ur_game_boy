
#ifndef MAPS_window_H
#define MAPS_window_H

#include <stdint.h>
#include <stdbool.h>

extern uint8_t map_window[6 * 14];
extern const uint8_t skip_message[6];

enum
{
    DICE_WINDOW_LOCATION = 21,
    PLAYER_WINDOW_LOCATION = 58,
    WINDOW_WIDTH = 6,
    WINDOW_HIGHT = 14,
    SKIP_WINDOW_LOCATION = WINDOW_WIDTH * (WINDOW_HIGHT - 1),
    DICE_VALUE_TILE_OFFSET = 1,
};

void setup_window(void);
void show_dice_value(uint8_t *window, uint8_t dice_value);
void show_player_turn(uint8_t *window, bool player);
void show_skip_turn(uint8_t *window, bool display);

#endif // MAPS_window_H