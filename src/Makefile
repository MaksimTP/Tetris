CFLAGS = -std=c11 -Wall -Wextra -Werror	
TESTS = tests/*.c
FLAG_TEST = -lcheck -lsubunit -lm
LIBS = -lncurses 
TARGETS = brick_game/tetris/backend.c 


all: clean install gcov_report

dvi:
	open instruction.md

dist: clean install
	mkdir build && mv tetris build
	cp instruction.md build
	tar cvzf Tetris.tgz build/

test: clean
	gcc $(CFLAGS) $(TARGETS) gui/cli/front.c $(TESTS) $(LIBS) $(FLAG_TEST) -o tetris && ./tetris

gcov_report:
	gcc --coverage $(TARGETS) $(TESTS) $(LIBS) $(FLAG_TEST) -o greport
	./greport
	lcov -t "s21_tetris" -o s21_tetris.info -c -d .
	genhtml -o ./report s21_tetris.info
	open ./report/index.html


install: clean
	gcc $(CFLAGS) $(TARGETS) gui/cli/front.c main.c -lncurses -o tetris

uninstall: clean

check_style: clean
	clang-format -n brick_game/tetris/*.h brick_game/tetris/*.c tests/*.c tests/*.h main.c gui/cli/*.h gui/cli/*.c -style='{BasedOnStyle: Google, ColumnLimit: 150, IndentWidth: 4}'

style: clean
	clang-format -i brick_game/tetris/*.h brick_game/tetris/*.c tests/*.c tests/*.h main.c gui/cli/*.h gui/cli/*.c -style='{BasedOnStyle: Google, ColumnLimit: 150, IndentWidth: 4}'

clean:
	rm -rf brick_game/tetris/*.o gui/cli/*.o *.o *.gcno *.gcda *.info gcov_report report greport tetris build Tetris.tgz