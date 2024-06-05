#include "tests_backend.h"

START_TEST(s21_key_to_action) {
    char chars[] = {3, 2, 4, 5, 10, 27, 0};
    char actions[] = {Up, Down, Left, Right, Start, Terminate, Action};

    for (int i = 0; i < 7; i++) {
        ck_assert_int_eq(actions[i], key_to_action(chars[i]));
    }
}
END_TEST

START_TEST(s21_userInput) {
    GameInfo_t info = {0};
    GameInfo_t *infp_ = get_instance(&info);
    infp_->high_score++;
    char actions[] = {Start, Up, Down, Left, Right, Action};
    for (int i = 0; i < 6; i++) {
        userInput(actions[i], false);
    }
}
END_TEST

START_TEST(s21_updateCurrentState) {
    GameInfo_t info = {0};
    get_instance(&info);
    GameInfo_t info_1 = updateCurrentState();
    info_1.score++;
}
END_TEST

START_TEST(s21_all_values_equal) {
    int arr[] = {1, 1, 1, 1, 1};
    int length = 5;
    ck_assert_int_eq(true, all_values_equal(arr, length));
}
END_TEST

START_TEST(s21_swap_values) {
    int a = 5;
    int b = 4;
    swap_values(&a, &b);
    ck_assert_int_eq(a, 4);
    ck_assert_int_eq(b, 5);
}
END_TEST

START_TEST(s21_min) {
    int arr[] = {1, 2, 3, 4, 5};
    int length = 5;
    ck_assert_int_eq(1, min(arr, length));
}
END_TEST

START_TEST(s21_init_game_params) {
    GameInfo_t info = {0};
    init_game_params(&info);
    ck_assert_int_eq(1, info.level);
    ck_assert_int_eq(INITIAL_TIMEOUT, info.speed);
    ck_assert_int_eq(0, info.score);
    ck_assert_int_eq(2000, info.high_score);
    ck_assert_int_eq(0, info.pause);

    free_struct(&info);
}
END_TEST

START_TEST(s21_is_zero_row) {
    GameInfo_t info = {0};
    init_game_params(&info);
    ck_assert_int_eq(true, is_zero_row(&info, 10));
    free_struct(&info);
}
END_TEST

START_TEST(s21_set_highscore_to_file) {
    GameInfo_t info = {0};
    init_game_params(&info);
    set_highscore_to_file(&info);
    ck_assert_int_eq(info.high_score, get_highscore_from_file());
    free_struct(&info);
}
END_TEST

START_TEST(s21_clear_row) {
    GameInfo_t info = {0};
    init_game_params(&info);
    clear_row(&info, 0);
    ck_assert_int_eq(true, is_zero_row(&info, 0));
    free_struct(&info);
}
END_TEST

START_TEST(s21_up_score) {
    GameInfo_t info = {0};
    init_game_params(&info);
    info.score = 0;
    for (int i = 1; i < 5; i++) {
        up_score(&info, i);
    }
    ck_assert_int_eq(2600, info.score);
    free_struct(&info);
}
END_TEST

START_TEST(s21_check_lvlup) {
    GameInfo_t info = {0};
    init_game_params(&info);
    info.score = 700;
    check_lvlup(&info);
    ck_assert_int_eq(2, info.level);
    ck_assert_int_eq(390, info.speed);
    free_struct(&info);
}
END_TEST

START_TEST(s21_is_twos_row) {
    GameInfo_t info = {0};
    init_game_params(&info);
    ck_assert_int_eq(false, is_twos_row(&info, 2));
    ck_assert_int_eq(false, is_twos_row(&info, 3));
    free_struct(&info);
}
END_TEST

START_TEST(s21_check_attach) {
    GameInfo_t info = {0};
    init_game_params(&info);
    ck_assert_int_eq(false, check_attach(&info));
    free_struct(&info);
}
END_TEST

START_TEST(s21_move_down) {
    GameInfo_t info = {0};
    init_game_params(&info);
    move_down(&info);
    ck_assert_int_eq(is_zero_row(&info, 0), true);
    free_struct(&info);
}
END_TEST

START_TEST(s21_stone_cur_fig_spawn_next) {
    GameInfo_t info = {0};
    init_game_params(&info);
    info.field[5][5] = 1;
    stone_cur_fig_spawn_next(&info);
    ck_assert_int_eq(2, info.field[5][5]);
    free_struct(&info);
}
END_TEST

Suite *s21_tetris_suite(void) {
    Suite *suite = suite_create("s21_tetris");
    TCase *tc_core = tcase_create("s21_tetris");

    tcase_add_test(tc_core, s21_key_to_action);
    tcase_add_test(tc_core, s21_userInput);
    tcase_add_test(tc_core, s21_updateCurrentState);
    tcase_add_test(tc_core, s21_all_values_equal);
    tcase_add_test(tc_core, s21_swap_values);
    tcase_add_test(tc_core, s21_min);
    tcase_add_test(tc_core, s21_init_game_params);
    tcase_add_test(tc_core, s21_is_zero_row);
    tcase_add_test(tc_core, s21_set_highscore_to_file);
    tcase_add_test(tc_core, s21_up_score);
    tcase_add_test(tc_core, s21_check_lvlup);
    tcase_add_test(tc_core, s21_is_twos_row);
    tcase_add_test(tc_core, s21_clear_row);
    tcase_add_test(tc_core, s21_check_attach);
    tcase_add_test(tc_core, s21_move_down);
    tcase_add_test(tc_core, s21_stone_cur_fig_spawn_next);

    suite_add_tcase(suite, tc_core);
    return suite;
}

void run_testcase(Suite *testcase) {
    static int count_testcase = 1;

    if (count_testcase > 0) {
        printf("\n");
        count_testcase++;
    }

    SRunner *sr = srunner_create(testcase);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    srunner_free(sr);
}

void run_tests() {
    Suite *list_cases[] = {s21_tetris_suite(), NULL};

    for (Suite **current_testcase = list_cases; *current_testcase != NULL; current_testcase++) {
        run_testcase(*current_testcase);
    }
}

int main() {
    run_tests();

    return 0;
}
