CC = g++
CCFLAGS = -std=c++17 -O2 -Wall -Wextra 
CCLIBS = -lglfw -lGLU -lGL -lm -lX11 -lpthread -ldl
P = main


all: $(P)

$(P): $(P).cpp
	$(CC) $(CCFLAGS) src/$(P).cpp src/VAO.hpp src/VBO.hpp src/EBO.hpp src/shader.hpp src/glad.c $(CCLIBS) -o bin/$(P)

	
