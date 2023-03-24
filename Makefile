CC = clang
EXE = bin/rayc
SRC = src/main.c
FLAGS = -g3 -std=c99 -Wall -Wextra
WIGNOREFLAGS = -Wno-unused-parameter -Wno-unused-variable
INCLUDE =
LIBS =
LFLAGS = -lSDL2

.PHONY: build run libs

build:
	mkdir -p bin
	$(CC) -o $(EXE) $(FLAGS) $(WIGNOREFLAGS) $(INCLUDE) $(SRC) $(LIBS) $(LFLAGS)

run: build
	$(EXE)

libs:
