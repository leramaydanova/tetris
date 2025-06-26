SHELL = /bin/sh

CFLAGS = -Wall -Wextra -Werror -std=c11 -g
LDFLAGS = -lcheck -lm
SRC = brick_game/tetris/backend.c gui/cli/frontend.c brick_game.c
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage

OS_NAME = $(shell uname)

ifeq ($(OS_NAME),Linux)
OPEN = xdg-open
LDFLAGS += -lsubunit
endif

ifeq ($(OS_NAME),Darwin)
OPEN = open
endif

.PHONY: all build/libtetris.a install uninstall test gcov_report dvi dist clang_format clang_check valgrind clean

all: clear install

build/libtetris.a: $(SRC)
	mkdir -p build
	gcc $(CFLAGS) -c $(SRC)
	ar rcs build/libtetris.a *.o
	mv *.o build

install: clean build/libtetris.a
	echo 0 > build/high_score.txt
	gcc $(CFLAGS) brick_game.c -Lbuild -ltetris $(LDFLAGS) -lncurses -o build/tetris

run: clean install
	cd build && ./tetris

uninstall:
	rm -rf build

clean:
	rm -rf build gcov
	rm -f tests/unit_test tests/high_score.txt 
	rm -rf tests/unit_test*/
	find dvi ! -name 'documentation.tex' ! -name 'Doxyfile' ! -name 'dvi' -delete
	rm -f tetris.tar.gz
	rm -f *.gc*

dvi:
	doxygen dvi/Doxyfile
	xelatex -output-directory=dvi dvi/documentation.tex
	$(OPEN) dvi/html/index.html

dist: install
	tar -czvf tetris.tar.gz Makefile brick_game gui tests brick_game.c brick_game.h Diagram.pdf

test: clean build/libtetris.a
	echo 0 > tests/high_score.txt
	gcc $(CFLAGS) tests/unit_test.c -Lbuild -ltetris $(LDFLAGS) -lncurses -o tests/unit_test
	cd tests && ./unit_test

gcov_report: clean build/libtetris.a
	mkdir -p gcov
	echo 0 > gcov/high_score.txt
	gcc $(CFLAGS) $(GCOV_FLAGS) tests/unit_test.c brick_game/tetris/backend.c -Lbuild -ltetris $(LDFLAGS) -lncurses -o gcov/gcov_test
	cd gcov && ./gcov_test
	lcov --capture --directory . --output-file gcov/coverage.info
	lcov --remove gcov/coverage.info '*tests/*' --output-file gcov/coverage.info
	genhtml -o gcov/report gcov/coverage.info
	$(OPEN) gcov/report/tetris/index.html

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c *.h
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h
	clang-format -i tests/*.c tests/*.h
	rm -f .clang-format

clang_check:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h
	clang-format -n brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -n gui/cli/*.c gui/cli/*.h
	clang-format -n tests/*.c tests/*.h
	rm -f .clang-format

valgrind: test
	cd tests && valgrind --tool=memcheck --leak-check=yes ./unit_test

clear:
	clear

# install gcovr: sudo apt install gcovr
# install ncurses: sudo apt-get install libncurses5-dev libncursesw5-dev
# install xelatex: sudo apt-get install texlive-xetex (or sudo apt-get install texlive-latex-extra)