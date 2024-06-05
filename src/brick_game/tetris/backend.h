#ifndef BACKEND_H
#define BACKEND_H

#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>

#include "defines.h"

UserAction_t key_to_action(char ch);
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
GameInfo_t* get_instance(GameInfo_t* inf);
void get_next_figure(GameInfo_t* a);
void free_struct(GameInfo_t* info);
void display_next(GameInfo_t* info);
void rotate_figure(GameInfo_t* info);
void init_game_params(GameInfo_t* info);
void process_pause(GameInfo_t* info);
void move_down(GameInfo_t* info);
void horizontal_move(GameInfo_t* info, int direction);
void stone_cur_fig_spawn_next(GameInfo_t* info);
bool check_attach(GameInfo_t* info);
void check_highscore(GameInfo_t* info);
bool all_values_equal(int* arr, int length);
void clear_row(GameInfo_t* info, int i);
bool is_zero_row(GameInfo_t* info, int i);
void up_score(GameInfo_t* info, int cnt_lines);
void shift_lines(GameInfo_t* info);
void swap_values(int* a, int* b);
int min(int* arr, int length);
bool is_twos_row(GameInfo_t* info, int i);
int get_highscore_from_file();
void set_highscore_to_file(GameInfo_t* info);
void check_lvlup(GameInfo_t* info);
bool can_spawn_figure(GameInfo_t* info);

#endif  // BACKEND_H