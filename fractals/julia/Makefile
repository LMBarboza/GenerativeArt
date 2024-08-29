CC = g++
CCFLAGS = -std=c++17 -O2 -Wall -Wextra 
CCLIBS = -lglfw -lGLU -lGL -lm -lX11 -lpthread -ldl
P = main


all: $(P)

$(P): src/$(P).cpp
	$(CC) $(CCFLAGS) src/$(P).cpp src/VAO.cpp src/VBO.cpp src/EBO.cpp src/shader.cpp src/glad.c $(CCLIBS) -o bin/$(P)

	
