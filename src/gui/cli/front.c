#include "front.h"

void print_rect(int height, int width, int x_bias, int y_bias) {
    // vertical
    for (int i = 0; i < height + 1; i++) {
        if (i == 0) {
            MVADDCH(i + y_bias, x_bias, ACS_ULCORNER);
            MVADDCH(i + y_bias, width + x_bias, ACS_URCORNER);
        } else if (i == height) {
            MVADDCH(i + y_bias, x_bias, ACS_LLCORNER);
            MVADDCH(i + y_bias, width + x_bias, ACS_LRCORNER);
        } else {
            MVADDCH(i + y_bias, x_bias, ACS_VLINE);
            MVADDCH(i + y_bias, width + x_bias, ACS_VLINE);
        }
    }
    // horizontal
    for (int i = 0; i < width; i++) {
        if (i == 0) {
            MVADDCH(y_bias, i + x_bias, ACS_ULCORNER);
        } else {
            MVADDCH(y_bias, i + x_bias, ACS_HLINE);
            MVADDCH(height + y_bias, i + x_bias, ACS_HLINE);
        }
    }
}

void print_stats(GameInfo_t* info) {
    print_rect(2, WIDTH + 3, WIDTH * 2 + 3, 1);
    MVPRINTW(1, WIDTH * 2 + 7, "SCORE");
    MVPRINTW(2, WIDTH * 2 + 7, "%06d", info->score);

    print_rect(2, WIDTH + 3, WIDTH * 2 + 3, 4);
    MVPRINTW(4, WIDTH * 2 + 5, "HIGH SCORE");
    MVPRINTW(5, WIDTH * 2 + 7, "%06d", info->high_score);

    print_rect(2, WIDTH + 3, WIDTH * 2 + 3, 7);
    MVPRINTW(7, WIDTH * 2 + 7, "LEVEL");
    MVPRINTW(8, WIDTH * 2 + 7, "%06d", info->level);

    print_rect(2, WIDTH + 3, WIDTH * 2 + 3, 10);
    MVPRINTW(10, WIDTH * 2 + 7, "SPEED");
    MVPRINTW(11, WIDTH * 2 + 7, "%06d", info->speed);

    print_rect(6, WIDTH + 3, WIDTH * 2 + 3, 13);
    MVPRINTW(13, WIDTH * 2 + 8, "NEXT");
    print_next_fig_overlay(info);
}

void print_next_fig_overlay(GameInfo_t* info) {
    for (int i = 0; i < 2 * N; i += 2) {
        for (int j = 0; j < 2 * N; j += 2) {
            if (info->next[i / 2][j / 2] == 1) {
                MVADDCH(15 + i / 2, WIDTH * 2 + 6 + j, '[');
                MVADDCH(15 + i / 2, WIDTH * 2 + 6 + j + 1, ']');
            }
        }
    }
}

void print_hud(GameInfo_t* info) {
    // FIELD
    print_rect(HEIGHT, WIDTH * 2 + 1, -1, 0);
    // STATS
    print_rect(HEIGHT, WIDTH + 5, WIDTH * 2 + 2, 0);
    print_stats(info);
}

void print_field(GameInfo_t* info) {
    for (int i = 0; i < 2 * HEIGHT; i += 2) {
        for (int j = 0; j < 2 * WIDTH; j += 2) {
            if (info->field[i / 2][j / 2] == 1 || info->field[i / 2][j / 2] == 2) {
                MVADDCH(i / 2, j, '[');
                MVADDCH(i / 2, j + 1, ']');
            }
        }
    }
}

void statistics(GameInfo_t* info) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (info->field[i][j] == 0)
                MVADDCH(i + 1, 43 + j, '.');
            else if (info->field[i][j] == 1)
                MVADDCH(i + 1, 43 + j, '*');
            else
                MVADDCH(i + 1, 43 + j, '#');
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (info->field[i][j] == 0)
                MVADDCH(i + 1, 64 + j, '.');
            else if (info->field[i][j] == 1)
                MVADDCH(i + 1, 64 + j, '*');
        }
    }
}

void print_game_overlay(GameInfo_t* info) {
    clear();
    refresh();
    print_hud(info);
    print_field(info);
}

void display_next(GameInfo_t* info) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            MVPRINTW(25 + i, j, "%d ", info->next[i][j]);
        }
    }
}