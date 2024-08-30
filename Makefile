PACKAGE_NAME = tetris
VERSION = 1.0
DIST_DIR = $(PACKAGE_NAME)-$(VERSION)
CFLAGS=-std=c11 -pedantic -Wall -Werror -Wextra
OS := $(shell uname -s)

ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
	TEST_LIBS = -lcheck -lsubunit -lrt -lm -pthread
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
	TEST_LIBS = -lcheck
endif

all: install

install: style tetris_game frontend
	mkdir game
	gcc ./obj/* -o ./game/BrickGame -lcurses

uninstall:
	rm -rf ./game

dvi:
	latex ./documentation.tex

dist: $(DIST_DIR).tar.gz

$(DIST_DIR).tar.gz: $(DIST_DIR)
	tar -czf $@ $^

$(DIST_DIR):
	mkdir $@
	cp -r brick_game $@/
	cp -r gui $@/
	cp brick_game.c $@/
	cp documentation.tex $@/
	cp Makefile $@/

frontend:
	gcc $(CFLAGS) -c ./gui/cli/frontend.c -o ./obj/frontend.o

tetris_game:
	mkdir ./obj
	gcc $(CFLAGS) -c ./brick_game.c -o ./obj/brick_game.o
	gcc $(CFLAGS) -c ./brick_game/tetris/src/fsm.c -o ./obj/fsm.o
	gcc $(CFLAGS) -c ./brick_game/tetris/src/backend.c -o ./obj/backend.o

test:
	gcc -std=c11 -Wall -Werror -Wextra -o test.out ./brick_game/tetris/src/* ./tests/* $(TEST_LIBS)
	./test.out

gcov_report:
	gcc -std=c11 -Wall -Werror -Wextra --coverage ./brick_game/tetris/src/* ./tests/tests.c -o gcov_test $(TEST_LIBS)
	chmod +x gcov_test
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	$(OPEN_CMD) ./report/index.html
	rm -rf gcov*


style:
	clang-format -i ./brick_game/tetris/inc/* ./brick_game/tetris/src/* ./gui/cli/* ./tests/* *.c

rebuild: clean uninstall style all

clean:
	rm -rf ./obj ./test.out ./report ./documentation.dvi $(DIST_DIR) $(DIST_DIR).tar.gz