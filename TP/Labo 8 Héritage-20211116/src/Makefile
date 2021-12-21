# Shell command to compile:
# g++ --std='c++17' -Wall -Wextra -Wpedantic -fmax-errors=2 lab8.cpp -o lab8.out -lfltk
CC = g++ --std='c++17' -Wall -Wextra -Wpedantic -fmax-errors=2

.PHONY: main
main: lab8.out

.PHONY: all
all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp makefile
	$(CC) $< -o $@ -lfltk
