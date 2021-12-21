CC = g++ --std='c++17' -Wall -Wextra -Wpedantic
FORMATTER = astyle --style=google -s2 -S

.PHONY: all
all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp makefile
	$(CC) $< -o $@ -lfltk

.PHONY: format
format: *.cpp
	sed -i 's/, */, /g' $+
	sed -i 's/\([^:]\): *\([^:]\)/\1: \2/g' $+
	sed -i 's/\([a-zA-Z0-9]\) *= *\([a-zA-Z0-9]\)/\1 = \2/g' $+
	$(FORMATTER) $+
	sed -i 's/^\(   *\): /\1  : /' $+
	sed -i 's/operator =/operator=/g' $+
