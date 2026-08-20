#ifndef STRUCTS_token_H
#define STRUCTS_token_H

#include <stdint.h>
#include <stdbool.h>

extern struct Token
{
    uint8_t x;
    uint8_t y;
    uint8_t tiles_index[2];
    uint8_t position;
};

extern enum { NB_TOKEN_PER_PLAYER = 7 };

extern struct Token p1_tokens[7];
extern struct Token p2_tokens[7];

void place_token(struct Token *token);
void move_token(struct Token *token, int8_t x, int8_t y);
void scroll_token(struct Token *token, int8_t x, int8_t y);
void init_tokens(const uint8_t tiles_p1[64], const uint8_t tiles_p2[64], struct Token *tokens_p1, struct Token *tokens_p2);
void move_token_on_board(struct Token *token, bool player);
void move_token_on_waiting_area(struct Token *token, bool player, uint8_t order);

#endif // STRUCTS_token_H