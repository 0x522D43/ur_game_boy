#ifndef STRUCTS_game_H
#define STRUCTS_game_H

#include <stdint.h>
#include <stdbool.h>

#include "Token.h"

enum
{
    NB_PLAYER = 2,
    NB_BONUS_TILE = 3,
    BOARD_FINISH_POSITION = 15,
    BOARD_START_POSITION = 0,
};

extern struct Game
{
    uint8_t dice;
    bool player_turn;
    uint8_t *background;
    uint8_t *window;
    struct Token *player1_tokens;
    struct Token *player2_tokens;
};

void roll_dice(struct Game *game);
void draw_tokens_on_board(struct Game *game);
void get_movement_per_token(struct Game *game, uint8_t available_movements[NB_TOKEN_PER_PLAYER]);
bool has_no_action(uint8_t actions[NB_TOKEN_PER_PLAYER]);
bool is_game_over(struct Game *game);

#endif // STRUCTS_game_H