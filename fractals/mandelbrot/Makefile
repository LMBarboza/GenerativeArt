CC = g++
CCFLAGS = -std=c++17 -g -Wall -Wextra 
CCLIBS = -lglfw -lGLU -lGL -lm -lX11 -lpthread -ldl
P = main
A = animation

all: $(P) $(A)

$(P): src/$(P).cpp
	$(CC) $(CCFLAGS) src/$(P).cpp src/VAO.cpp src/VBO.cpp src/EBO.cpp src/shader.cpp src/glad.c $(CCLIBS) -o bin/$(P)

$(A): src/$(A).cpp
	$(CC) $(CCFLAGS) src/$(A).cpp src/VAO.cpp src/VBO.cpp src/EBO.cpp src/shader.cpp src/glad.c $(CCLIBS) -o bin/$(A)
	
