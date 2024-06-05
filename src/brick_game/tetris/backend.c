#include "backend.h"

static int FIGURES[7][N][N] = {{{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

                               {{0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

                               {{0, 0, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

                               {{0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

                               {{0, 0, 0, 0, 0}, {0, 0, 1, 1, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

                               {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},

                               {{0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}};

void userInput(UserAction_t action, bool hold) {
    (void)hold;
    static int iter_cnt = 0;
    GameInfo_t* info = get_instance(NULL);

    if (iter_cnt > 0 && !can_spawn_figure(info)) action = Terminate;

    switch (action) {
        case Start:
            init_game_params(info);
            break;
        case Pause:
            process_pause(info);
            break;
        case Terminate:
            check_highscore(info);
            set_highscore_to_file(info);
            exit(1);
            break;
        case Left:
            horizontal_move(info, -1);
            break;
        case Right:
            horizontal_move(info, 1);
            break;
        case Up:
            rotate_figure(info);
            break;
        case Down:
            if (!check_attach(info)) {
                move_down(info);
            } else {
                stone_cur_fig_spawn_next(info);
            }
            break;
        case Action:
            break;
    }

    if (action != Down && iter_cnt % 2 == 0) {
        if (!check_attach(info)) {
            move_down(info);
        } else {
            stone_cur_fig_spawn_next(info);
        }
    }
    shift_lines(info);
    check_lvlup(info);
    iter_cnt++;
}

void stone_cur_fig_spawn_next(GameInfo_t* info) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (info->field[i][j] == 1) {
                info->field[i][j] = 2;
            }
        }
    }

    int x_start_bias = 2;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (info->next[i][j] == 1) info->field[i][j + x_start_bias] = 1;
        }
    }

    get_next_figure(info);
}

GameInfo_t updateCurrentState() {
    GameInfo_t* info_p = get_instance(NULL);
    return *info_p;
}

int get_highscore_from_file() {
    FILE* fp = NULL;
    int x = 0;
    char filename[] = "highscore.txt";
    if ((fp = fopen(filename, "r")) != NULL) {
        fscanf(fp, "%d", &x);
    }

    if (fp) fclose(fp);

    return x;
}

void set_highscore_to_file(GameInfo_t* info) {
    FILE* fp = NULL;
    int x = info->high_score;
    char filename[] = "highscore.txt";
    if ((fp = fopen(filename, "w")) != NULL) {
        fprintf(fp, "%d", x);
    }
    fclose(fp);
}

void init_game_params(GameInfo_t* info) {
    info->level = 1;
    info->high_score = get_highscore_from_file();
    info->speed = INITIAL_TIMEOUT;
    info->pause = 0;
    info->score = 0;
    info->field = calloc(HEIGHT, sizeof(int**));
    info->next = NULL;

    for (int i = 0; i < HEIGHT; i++) {
        info->field[i] = calloc(WIDTH, sizeof(int*));
    }
    get_next_figure(info);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (info->next[i][j] == 1) {
                info->field[i][j] = 1;
            }
        }
    }
    get_next_figure(info);
}

bool check_attach(GameInfo_t* info) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (info->field[i][j] == 1 && i == HEIGHT - 1)
                return true;
            else if (info->field[i][j] == 1 && info->field[i + 1][j] == 2)
                return true;
        }
    }

    return false;
}

void move_down(GameInfo_t* info) {
    int arr_i_coords[M] = {};
    int arr_j_coords[M] = {};
    int ind_cnt = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (info->field[i][j] == 1) {
                arr_i_coords[ind_cnt] = i;
                arr_j_coords[ind_cnt] = j;
                ind_cnt++;
                info->field[i][j] = 0;
                info->field[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < M; i++) {
        info->field[arr_i_coords[i] + 1][arr_j_coords[i]] = 1;
    }
}

// direction can be either 1 or -1, where 1 means right and -1 - left.
void horizontal_move(GameInfo_t* info, int direction) {
    int is_able_make_move = 1;
    for (int i = 0; i < HEIGHT && is_able_make_move; i++) {
        for (int j = 0; j < WIDTH && is_able_make_move; j++) {
            if (info->field[i][j] == 1) {
                if (j + direction < 0 || j + direction == WIDTH)
                    is_able_make_move = 0;
                else if (info->field[i][j + direction] == 2)
                    is_able_make_move = 0;
            }
        }
    }

    if (is_able_make_move) {
        int arr_i_coords[M] = {};
        int arr_j_coords[M] = {};
        int ind_cnt = 0;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (info->field[i][j] == 1) {
                    arr_i_coords[ind_cnt] = i;
                    arr_j_coords[ind_cnt] = j;
                    ind_cnt++;
                    info->field[i][j] = 0;
                    info->field[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < M; i++) {
            info->field[arr_i_coords[i]][arr_j_coords[i] + direction] = 1;
        }
    }
}

GameInfo_t* get_instance(GameInfo_t* inf) {
    static GameInfo_t* game_info = {0};
    if (inf == NULL) return game_info;
    game_info = inf;
    return game_info;
}

void get_next_figure(GameInfo_t* a) {
    if (a->next) {
        for (int i = 0; i < N; i++) {
            free(a->next[i]);
        }
        free(a->next);
    }

    a->next = calloc(N, sizeof(int**));

    for (int i = 0; i < N; i++) {
        a->next[i] = calloc(N, sizeof(int*));
    }

    int fig_num = rand() % 7;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (FIGURES[fig_num][i][j] == 1) a->next[i][j] = 1;
        }
    }
}

void free_struct(GameInfo_t* info) {
    for (int i = 0; i < HEIGHT; i++) {
        free(info->field[i]);
    }
    free(info->field);

    if (info->next) {
        for (int i = 0; i < N; i++) {
            if (info->next[i]) free(info->next[i]);
        }

        free(info->next);
    }
}

void rotate_figure(GameInfo_t* info) {
    int arr_i_coords[M] = {0};
    int arr_j_coords[M] = {0};
    int ind_cnt = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (info->field[i][j] == 1) {
                arr_i_coords[ind_cnt] = i;
                arr_j_coords[ind_cnt] = j;
                ind_cnt++;
                info->field[i][j] = 0;
                info->field[i][j] = 0;
            }
        }
    }
    int a[N][N] = {0};
    int min_idx_i = min(arr_i_coords, M);
    int min_idx_j = min(arr_j_coords, M);

    for (int i = 0; i < M; i++) {
        a[arr_i_coords[i] - min_idx_i][arr_j_coords[i] - min_idx_j] = 1;
    }

    for (int i = 0; i < N / 2; i++) {
        for (int j = i; j < N - i - 1; j++) {
            int temp = a[i][j];
            a[i][j] = a[N - 1 - j][i];
            a[N - 1 - j][i] = a[N - 1 - i][N - 1 - j];
            a[N - 1 - i][N - 1 - j] = a[j][N - 1 - i];
            a[j][N - 1 - i] = temp;
        }
    }

    int can_rotate = 1;
    int y_bias = 1;
    int x_bias = 2;

    for (int i = 0; i < N && can_rotate; i++) {
        for (int j = 0; j < N && can_rotate; j++) {
            if (a[i][j] == 1 && (info->field[min_idx_i + i - y_bias][min_idx_j + j - x_bias] == 1 ||
                                 info->field[min_idx_i + i - y_bias][min_idx_j + j - x_bias] == 2)) {
                can_rotate = 0;
            } else if (a[i][j] == 1 && (min_idx_i + i - y_bias > HEIGHT - 1 || min_idx_j + j - x_bias > WIDTH - 1 || min_idx_j + j - x_bias < 0 ||
                                        min_idx_i + i - y_bias < 0)) {
                can_rotate = 0;
            }
        }
    }

    if (can_rotate) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (a[i][j] == 1) {
                    info->field[min_idx_i + i - y_bias][min_idx_j + j - x_bias] = 1;
                }
            }
        }
    } else {
        for (int i = 0; i < M; i++) {
            info->field[arr_i_coords[i]][arr_j_coords[i]] = 1;
        }
    }
}

int min(int* arr, int length) {
    int min_ = arr[0];
    for (int i = 0; i < length; i++) {
        if (arr[i] < min_) {
            min_ = arr[i];
        }
    }
    return min_;
}

void process_pause(GameInfo_t* info) {
    info->pause = 1;
    while (info->pause) {
        char ch = getch();
        if (ch == ESCAPE) {
            break;
        }
    }
    info->pause = 0;
}

void check_highscore(GameInfo_t* info) {
    if (info->score > info->high_score) info->high_score = info->score;
}

void shift_lines(GameInfo_t* info) {
    int cnt_lines = 0;
    int lines_for_lvlup = 0;
    for (int i = 0; i < HEIGHT; i++) {
        if (all_values_equal(info->field[i], WIDTH)) {
            if (is_twos_row(info, i)) lines_for_lvlup++;
            clear_row(info, i);
            cnt_lines++;
        }
    }

    if (cnt_lines) {
        for (int i = HEIGHT - 1; i != 0; i--) {
            if (is_zero_row(info, i)) {
                for (int j = 0; j < WIDTH; j++) {
                    swap_values(&info->field[i][j], &info->field[i - 1][j]);
                }
            }
        }
    }
    up_score(info, lines_for_lvlup);
}

void swap_values(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void clear_row(GameInfo_t* info, int i) {
    for (int j = 0; j < WIDTH; j++) {
        info->field[i][j] = 0;
    }
}

bool is_zero_row(GameInfo_t* info, int i) {
    for (int j = 0; j < WIDTH; j++) {
        if (info->field[i][j] != 0) return false;
    }
    return true;
}

bool is_twos_row(GameInfo_t* info, int i) {
    for (int j = 0; j < WIDTH; j++) {
        if (info->field[i][j] != 2) return false;
    }
    return true;
}

bool all_values_equal(int* arr, int length) {
    for (int i = 0; i < length - 1; i++) {
        if (arr[i] != arr[i + 1]) return false;
    }
    return true;
}

void up_score(GameInfo_t* info, int cnt_lines) {
    switch (cnt_lines) {
        case 1:
            info->score += 100;
            break;
        case 2:
            info->score += 300;
            break;
        case 3:
            info->score += 700;
            break;
        case 4:
            info->score += 1500;
            break;
    }
    check_highscore(info);
}

void check_lvlup(GameInfo_t* info) {
    int max_score_to_lvlup = 6000;
    int lvlup_interval = 600;
    int lvlprev = info->level;

    if (info->score < max_score_to_lvlup) {
        info->level = info->score / lvlup_interval + 1;
    }

    int lvlcur = info->level;

    info->speed -= 10 * (lvlcur - lvlprev);
}

bool can_spawn_figure(GameInfo_t* info) {
    bool res = true;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (info->field[i][j] == 2) {
                res = false;
            }
        }
    }
    return res;
}

UserAction_t key_to_action(char ch) {
    UserAction_t r;
    switch (ch) {
        case 3:  // K_UP
            r = Up;
            break;
        case 2:  // K_DOWn
            r = Down;
            break;
        case 4:  // K_LEFT
            r = Left;
            break;
        case 5:  // K_RIGHT
            r = Right;
            break;
        case ENTER_KEY:
            r = Start;
            break;
        case ESCAPE:
            r = Terminate;
            break;
        default:
            r = Action;
            break;
    }
    return r;
}