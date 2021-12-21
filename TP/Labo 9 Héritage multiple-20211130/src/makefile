CC17 = g++ --std='c++17' -Wall -Wextra -Wpedantic -fmax-errors=2
CC20 = g++ --std='c++20' -Wall -Wextra -Wpedantic -fmax-errors=2
CC = $(CC17)
FORMATTER = astyle --style=google -s2 -S

.PHONY: main
main: all

.PHONY: all
all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%-std17.out: %-std17.cpp makefile
	$(CC17) $< -o $@ -lfltk

%-std20.out: %-std20.cpp makefile
	$(CC20) $< -o $@ -lfltk

%.out: %.cpp makefile
	$(CC) $< -o $@ -lfltk

.PHONY: format
format: *.cpp
	sed -i 's/, */, /g' $+
	sed -i 's/\([^:]\): *\([^:]\)/\1: \2/g' $+
	sed -i 's/\([a-zA-Z0-9]\) *= *\([a-zA-Z0-9]\)/\1 = \2/g' $+
	$(FORMATTER) $+
	sed -i 's/^\(   *\): /\1  : /' $+
	sed -i '/ virtual /s/ = 0;/ =0;/g' $+
	sed -i '/ for *(/s/(\([a-zA-Z0-9_ ]*\) = \([01]\);/(\1=\2;/' $+
