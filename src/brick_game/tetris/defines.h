#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>
#include <time.h>
#define N 5
#define M 4
#define HEIGHT 20
#define WIDTH 10
#define GET_USER_INPUT getch()
#define INITIAL_TIMEOUT 400
#define BOARDS_BEGIN 2
#define ESCAPE 27
#define ENTER_KEY 10

#define WIN_INIT(time)         \
    {                          \
        initscr();             \
        setlocale(LC_ALL, ""); \
        noecho();              \
        curs_set(0);           \
        keypad(stdscr, TRUE);  \
        timeout(time);         \
    }

typedef enum { Start, Pause, Terminate, Left, Right, Up, Down, Action } UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

#endif