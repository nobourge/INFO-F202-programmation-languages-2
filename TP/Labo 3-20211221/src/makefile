CC = g++-10 --std='c++20' -Wall -Wextra -Wpedantic

.PHONY: all
all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp makefile
	$(CC) $< -o $@ -lfltk
