#include <stdint.h>
#include <stdbool.h>
#include <gb/gb.h>
#include <rand.h>

#include "../maps/Window.h"
#include "Game.h"
#include "Token.h"

const uint8_t BONUS_TILE_POSITIONS[NB_BONUS_TILE] = {4, 8, 14};

void roll_dice(struct Game *game)
{
    game->dice = rand() % 5;
    game->window[DICE_WINDOW_LOCATION] = game->dice + 1;
}

void draw_tokens_on_board(struct Game *game)
{
    for (uint8_t token_number = 0; token_number < NB_TOKEN_PER_PLAYER * NB_PLAYER; token_number++)
    {
        uint8_t player = token_number / NB_TOKEN_PER_PLAYER;
        uint8_t token_index = token_number % NB_TOKEN_PER_PLAYER;
        struct Token *token;

        if (player == 0)
        {
            token = &game->player1_tokens[token_index];
        }
        else
        {
            token = &game->player2_tokens[token_index];
        }

        if (token->position == 0)
        {
            move_token_on_waiting_area(token, player, token_index);
        }
        else if (token->position < 15)
        {
            move_token_on_board(token, player);
        }
        else
        {
            hide_sprite(token->tiles_index[0]);
            hide_sprite(token->tiles_index[1]);
        }
    }
}

bool is_bonus_position(uint8_t position)
{
    for (uint8_t bonus_position = 0; bonus_position < NB_BONUS_TILE; bonus_position++)
    {
        if (position == BONUS_TILE_POSITIONS[bonus_position])
        {
            return true;
        }
    }
    return false;
}

void get_movement_per_token(struct Game *game, uint8_t available_movements[NB_TOKEN_PER_PLAYER])
{
    for (uint8_t index = 0; index < NB_TOKEN_PER_PLAYER; index++)
    {
        available_movements[index] = 0;
    }
    if (game->dice == 0)
    {
        return;
    }

    struct Token *player_tokens;
    struct Token *opponant_tokens;
    if (game->player_turn == 0)
    {
        player_tokens = game->player1_tokens;
        opponant_tokens = game->player2_tokens;
    }
    else
    {
        player_tokens = game->player2_tokens;
        opponant_tokens = game->player1_tokens;
    }

    for (uint8_t token_index = 0; token_index < NB_TOKEN_PER_PLAYER; token_index++)
    {
        struct Token token = player_tokens[token_index];
        uint8_t target_token_position = token.position + game->dice;
        if (target_token_position > 15)
            continue;
        bool position_not_taken = true;
        if (target_token_position != 15)
        {
            for (uint8_t board_token_index = 0; board_token_index < NB_TOKEN_PER_PLAYER; board_token_index++)
            {
                struct Token checked_token = player_tokens[board_token_index];
                if (token_index == board_token_index)
                    continue;
                if (target_token_position == checked_token.position)
                {
                    position_not_taken = false;
                    break;
                };
                checked_token = opponant_tokens[board_token_index];
                if (target_token_position > 4 &&
                    target_token_position < 13 &&
                    target_token_position == checked_token.position)
                {
                    position_not_taken = target_token_position != 8;
                    break;
                }
            }
        }
        if (position_not_taken)
        {
            available_movements[token_index] = target_token_position;
        }
    }
    return;
}

bool has_no_action(uint8_t actions[NB_TOKEN_PER_PLAYER])
{
    for (uint8_t action_idx = 0; action_idx < NB_TOKEN_PER_PLAYER; action_idx++)
    {
        if (actions[action_idx] != 0)
        {
            return false;
        }
    }
    return true;
}

bool is_game_over(struct Game *game)
{
    struct Token *player_tokens;
    if (game->player_turn == 0)
    {
        player_tokens = game->player1_tokens;
    }
    else
    {
        player_tokens = game->player2_tokens;
    }

    for (uint8_t token_index = 0; token_index < NB_TOKEN_PER_PLAYER; token_index++)
    {
        if (player_tokens[token_index].position != BOARD_FINISH_POSITION)
        {
            return false;
        }
    }
    return true;
}
