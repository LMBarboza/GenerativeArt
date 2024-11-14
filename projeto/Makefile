CC = g++
CCFLAGS = -std=c++17 -g -Wall -Wextra 
CCLIBS = -lglfw -lGLU -lGL -lm -lX11 -lpthread -ldl
P = main

all: $(P) 

$(P): src/$(P).cpp
	$(CC) $(CCFLAGS) src/$(P).cpp src/glad.c $(CCLIBS) -o $(P)
