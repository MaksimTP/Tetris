#ifndef FRONT_H
#define FRONT_H
#include "../../brick_game/tetris/backend.h"

#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

void print_next_fig_overlay(GameInfo_t* info);
void print_rect(int height, int width, int x_bias, int y_bias);
void print_stats(GameInfo_t* info);
void print_hud(GameInfo_t* info);
void print_game_overlay(GameInfo_t* info);

#endif