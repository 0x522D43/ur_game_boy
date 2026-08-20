#include <stdint.h>
#include <stdbool.h>
#include <gb/gb.h>

#include "Token.h"

void place_token(struct Token *token)
{
    move_sprite(token->tiles_index[0], token->x, token->y);
    move_sprite(token->tiles_index[1], token->x + 8, token->y);
}

void move_token(struct Token *token, int8_t x, int8_t y)
{
    token->x = x;
    token->y = y;
    place_token(token);
}

void scroll_token(struct Token *token, int8_t x, int8_t y)
{
    for (int i = 0; i < 2; i++)
    {
        scroll_sprite(token->tiles_index[i], x, y);
    }
    token->x += x;
    token->y += y;
}

struct Token p1_tokens[7];
struct Token p2_tokens[7];

void init_tokens(const uint8_t tiles_p1[64], const uint8_t tiles_p2[64], struct Token *tokens_p1, struct Token *tokens_p2)
{
    SPRITES_8x16;
    set_sprite_data(0, 4, tiles_p1);
    set_sprite_data(4, 4, tiles_p2);

    for (int player_token_index = 0; player_token_index < NB_TOKEN_PER_PLAYER; player_token_index++)
    {

        for (int tile_index = 0; tile_index < 2; tile_index++)
        {
            tokens_p1[player_token_index].tiles_index[tile_index] = player_token_index * 2 + tile_index;
            set_sprite_tile(tokens_p1[player_token_index].tiles_index[tile_index], tile_index * 2);

            tokens_p2[player_token_index].tiles_index[tile_index] = NB_TOKEN_PER_PLAYER * 2 + player_token_index * 2 + tile_index;
            set_sprite_tile(tokens_p2[player_token_index].tiles_index[tile_index], 4 + tile_index * 2);
        }

        move_token_on_waiting_area(&tokens_p1[player_token_index], 0, player_token_index);
        move_token_on_waiting_area(&tokens_p2[player_token_index], 1, player_token_index);

        // delay(100);
    }
}

void move_token_on_waiting_area(struct Token *token, bool player, uint8_t order)
{
    uint8_t player_offset = 0;
    if (player)
    {
        player_offset = 80;
    }
    token->x = 12 + player_offset + order % 2 * 8;
    token->y = (order + 1) * 16 + 16;
    token->position = 0;

    place_token(token);
}

void move_token_on_board(struct Token *token, bool player)
{
    uint8_t position = token->position;
    if (position < 1)
    {
        return;
    }
    else if (position <= 4)
    {
        token->x = 5 * 8 + player * 16 * 2;
        token->y = (11 - position * 2) * 8;
    }
    else if (position <= 12)
    {
        token->x = 56;
        token->y = (position - 4) * 16 + 8;
    }
    else if (position <= 14)
    {
        token->x = 5 * 8 + player * 16 * 2;
        token->y = (11 + 16 * 6 - position * 2) * 8;
    }
    if (position >= 15)
    {
        token->x = 0;
        token->y = 0;
    }

    place_token(token);
}