#include "brick_game/tetris/backend.h"
#include "gui/cli/front.h"

void game_loop() {
    char ch = '0';
    int was_entered = 0;
    int time_out = INITIAL_TIMEOUT;
    while (ch != 'q' && ch != 'Q') {
        timeout(time_out);
        ch = GET_USER_INPUT;
        if (!was_entered && ch != ENTER_KEY) {
            continue;
        } else
            was_entered = 1;

        userInput(key_to_action(ch), false);
        GameInfo_t info = updateCurrentState();
        print_game_overlay(&info);
        display_next(&info);
        time_out = info.speed;
        if (ch == 'q' || ch == 'Q') {
            free_struct(&info);
            break;
        }
    }
}

int main() {
    srand(time(0));
    WIN_INIT(15000);
    GameInfo_t info = {0};
    get_instance(&info);
    game_loop();
    endwin();
    return 0;
}
