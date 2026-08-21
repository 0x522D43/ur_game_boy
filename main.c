#include <gb/gb.h>
#include <gbdk/font.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "assets/ur_tile_standard.h"
#include "assets/ur_tile_bonus.h"
#include "assets/ur_tile_token_p1.h"
#include "assets/ur_tile_token_p2.h"
#include "assets/ur_tile_select.h"

#include "maps/Background.h"
#include "maps/Window.h"

#include "structs/Token.h"
#include "structs/Game.h"

const uint8_t FONT_TILE_SIZE = 37;

void setup_font(void)
{
    font_t min_font;
    font_init();
    min_font = font_load(font_min);
    font_set(min_font);
}

void display_game_over(bool player)
{
    uint8_t speed = 100;

    delay(speed);
    BGP_REG = 0XE4;
    delay(speed);
    BGP_REG = 0XF9;
    delay(speed);
    BGP_REG = 0XFE;
    delay(speed);
    BGP_REG = 0XFF;

    HIDE_SPRITES;
    HIDE_WIN;
    uint8_t endgame_background[18 * 20] = {0x00};
    endgame_background[65] = 0x25;
    endgame_background[74] = 0x27;
    endgame_background[105] = 0x26;
    endgame_background[114] = 0x28;

    uint8_t background_next_postion = FONT_TILE_SIZE + ur_tile_standard_TILE_COUNT + ur_tile_bonus_TILE_COUNT;
    if (player)
    {
        set_bkg_data(background_next_postion, ur_tile_token_p1_TILE_COUNT, ur_tile_token_p2_tiles);
    }
    else
    {
        set_bkg_data(background_next_postion, ur_tile_token_p2_TILE_COUNT, ur_tile_token_p1_tiles);
    }

    for (uint8_t i = 0; i < 20; i += 2)
    {
        endgame_background[i] = 0x2D;
        endgame_background[i + 1] = 0x2F;
        endgame_background[i + 20] = 0x2E;
        endgame_background[i + 20 + 1] = 0x30;

        endgame_background[i + 16 * 20] = 0x2D;
        endgame_background[i + 1 + 16 * 20] = 0x2F;
        endgame_background[i + 20 + 16 * 20] = 0x2E;
        endgame_background[i + 20 + 1 + 16 * 20] = 0x30;
    }

    for (uint8_t i = 2; i < 16; i += 2)
    {
        endgame_background[i * 20] = 0x2D;
        endgame_background[i * 20 + 1] = 0x2F;
        endgame_background[i * 20 + 20] = 0x2E;
        endgame_background[i * 20 + 20 + 1] = 0x30;

        endgame_background[i * 20 + 18] = 0x2D;
        endgame_background[i * 20 + 1 + 18] = 0x2F;
        endgame_background[i * 20 + 20 + 18] = 0x2E;
        endgame_background[i * 20 + 20 + 1 + 18] = 0x30;
    }

    const char *message = "\n\n\n\n       P  WIN\n\n\n\n\n    PRESS  START\n         TO\n     PLAY AGAIN";
    uint8_t idx = 0;
    for (uint8_t msg_idx = 0; msg_idx < 66; msg_idx++)
    {
        if (message[msg_idx] == '\n')
        {
            idx += 20 - (idx % 20);
        }
        else if (message[msg_idx] == ' ')
        {
            idx++;
        }
        else
        {
            endgame_background[idx] = message[msg_idx] - 'A' + 11;
            idx++;
        }
    }
    endgame_background[88] = player + 2;

    set_bkg_tiles(0, 0, 20, 18, endgame_background);

    delay(speed);
    BGP_REG = 0XFF;
    delay(speed);
    BGP_REG = 0XFE;
    delay(speed);
    BGP_REG = 0XF9;
    delay(speed);
    BGP_REG = 0XE4;

    while (!(joypad() & J_START))
        ;
}

void pause_game(uint8_t *joy_input)
{
    DISPLAY_OFF;
    uint8_t last_joy_input = *joy_input;
    while (!(*joy_input & J_START && !(last_joy_input & J_START)))
    {
        last_joy_input = *joy_input;
        *joy_input = joypad();
        vsync();
    }
    DISPLAY_ON;
}

uint8_t turn(struct Game *game)
{
    uint8_t played_action = 0;
    bool is_rolling_dice = true;
    bool is_showing_skip = false;
    uint8_t nb_dice_roll = 0;
    bool is_choosing_action = false;
    bool can_only_skip = false;
    bool need_window_redraw = false;
    bool is_end_of_turn = false;
    uint16_t frame = 0;
    uint8_t selected_action_index = 0;

    uint8_t last_joy_input = 0x00;

    show_player_turn(game->window, game->player_turn);
    set_win_tiles(0, 0, 6, 14, game->window);
    draw_tokens_on_board(game);

    uint8_t actions[NB_TOKEN_PER_PLAYER] = {0};

    while (1)
    {
        uint8_t joy_input = joypad();
        if (joy_input & J_START && !(last_joy_input & J_START))
        {
            pause_game(&joy_input);
            last_joy_input = joy_input;
        }

        if (is_rolling_dice && frame % 5 == 0)
        {
            roll_dice(game);
            need_window_redraw = true;
            nb_dice_roll++;

            if (nb_dice_roll >= 20)
            {
                is_rolling_dice = false;
            }
        }

        if (!is_choosing_action && !is_rolling_dice)
        {
            is_choosing_action = true;
            get_movement_per_token(game, actions);
            can_only_skip = has_no_action(actions);
            while (!can_only_skip && actions[selected_action_index] == 0)
            {
                selected_action_index = (selected_action_index + 1) % NB_TOKEN_PER_PLAYER;
            }
        }

        if (is_choosing_action && can_only_skip)
        {
            if (frame % 10 == 0)
            {
                need_window_redraw = true;
                is_showing_skip = !is_showing_skip;
                show_skip_turn(game->window, is_showing_skip);
            }
            if (joy_input & J_A && !(last_joy_input & J_A))
            {
                need_window_redraw = true;
                is_end_of_turn = true;
                show_skip_turn(game->window, false);
            }
        }
        else if (is_choosing_action)
        {
            struct Token *player_token;
            struct Token *opponant_tokens;
            if (game->player_turn == 0)
            {
                player_token = &game->player1_tokens[selected_action_index];
                opponant_tokens = game->player2_tokens;
            }
            else
            {
                player_token = &game->player2_tokens[selected_action_index];
                opponant_tokens = game->player1_tokens;
            }

            if (frame % 40 == 0)
            {
                if (player_token->position == 0)
                {
                    move_token_on_waiting_area(player_token, game->player_turn, selected_action_index);
                }
                else
                {
                    move_token_on_board(player_token, game->player_turn);
                }
            }
            else if (frame % 40 == 20)
            {
                hide_sprite(player_token->tiles_index[0]);
                hide_sprite(player_token->tiles_index[1]);
            }

            if (joy_input & J_A && !(last_joy_input & J_A))
            {
                played_action = actions[selected_action_index];
                player_token->position = played_action;
                if (player_token->position > 4 && player_token->position < 13 && player_token->position != 8)
                {
                    for (uint8_t opponant_token_index = 0; opponant_token_index < NB_TOKEN_PER_PLAYER; opponant_token_index++)
                    {
                        struct Token *opponant_token = &opponant_tokens[opponant_token_index];
                        if (opponant_token->position == player_token->position)
                        {
                            opponant_token->position = 0;
                            move_token_on_waiting_area(opponant_token, !game->player_turn, opponant_token_index);
                            break;
                        }
                    }
                }
                move_token_on_board(player_token, game->player_turn);
                is_end_of_turn = true;
            }
            else if (joy_input & J_UP && !(last_joy_input & J_UP))
            {
                if (player_token->position == 0)
                {
                    move_token_on_waiting_area(player_token, game->player_turn, selected_action_index);
                }
                else
                {
                    move_token_on_board(player_token, game->player_turn);
                }
                do
                {
                    selected_action_index = (selected_action_index + NB_TOKEN_PER_PLAYER - 1) % NB_TOKEN_PER_PLAYER;
                } while (actions[selected_action_index] == 0);
            }
            else if (joy_input & J_DOWN && !(last_joy_input & J_DOWN))
            {
                if (player_token->position == 0)
                {
                    move_token_on_waiting_area(player_token, game->player_turn, selected_action_index);
                }
                else
                {
                    move_token_on_board(player_token, game->player_turn);
                }
                do
                {
                    selected_action_index = (selected_action_index + 1) % NB_TOKEN_PER_PLAYER;
                } while (actions[selected_action_index] == 0);
            }
        }

        if (need_window_redraw)
        {
            set_win_tiles(0, 0, 6, 14, game->window);
            need_window_redraw = false;
        }

        if (is_end_of_turn)
        {
            break;
        }
        last_joy_input = joy_input;
        frame++;
        vsync();
    }
    return played_action;
}

void main(void)
{
    set_win_tiles(0, 0, 6, 14, map_window);
    move_win(116, 8);
    SHOW_WIN;

    initrand(DIV_REG);
    setup_font();
    while (1)
    {
        init_tokens(ur_tile_token_p1_tiles, ur_tile_token_p2_tiles, p1_tokens, p2_tokens);

        setup_window();
        setup_background(FONT_TILE_SIZE);

        SHOW_BKG;
        SHOW_WIN;
        SHOW_SPRITES;

        delay(100);
        BGP_REG = 0XFF;
        delay(100);
        BGP_REG = 0XFE;
        delay(100);
        BGP_REG = 0XF9;
        delay(100);
        BGP_REG = 0XE4;

        struct Game game;
        game.dice = 0;
        game.player_turn = 1;
        game.background = map_background_board;
        game.window = map_window;
        game.player1_tokens = p1_tokens;
        game.player2_tokens = p2_tokens;

        game.dice = 3;
        show_dice_value(game.window, game.dice);

        draw_tokens_on_board(&game);
        set_win_tiles(0, 0, 6, 14, game.window);

        do
        {
            game.player_turn = !game.player_turn;
            uint8_t played_action = turn(&game);
            if (played_action == 4 || played_action == 8 || played_action == 14)
            {
                game.player_turn = !game.player_turn;
            }
            delay(1);

        } while (!is_game_over(&game));

        delay(100);
        display_game_over(game.player_turn);

        if (joypad() & J_START)
        {
            delay(100);
            BGP_REG = 0XE4;
            delay(100);
            BGP_REG = 0XF9;
            delay(100);
            BGP_REG = 0XFE;
            delay(100);
            BGP_REG = 0XFF;
            delay(200);
        }
    }
}
