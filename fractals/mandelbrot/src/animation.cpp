#define GLFW_INCLUDE_NONE
// #include "../include/EBO.hpp"
#include "../include/VAO.hpp"
#include "../include/VBO.hpp"
#include "../include/julia.hpp"
#include "../include/shader.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

GLfloat rectangle_data[]{-1.0F, -1.0F, 0.0F, 1.0F,  -1.0F, 0.0F,
                         1.0F,  1.0F,  0.0F, 1.0F,  1.0F,  0.0F,
                         -1.0F, 1.0F,  0.0F, -1.0F, -1.0F, 0.0F};

JuliaData julia_data{1.0F, 0.0F, 0.0F, 200};

float cReal;
float cImag;
float a = 0.0;

constexpr float scaleC = 0.8;

// c = 0.7885(cosa + isina)
//

void euler(float a) {
  cReal = scaleC * cos(a);
  cImag = scaleC * sin(a);
}

auto lastActionTime = std::chrono::high_resolution_clock::now();
const double actionInterval = 2;

std::unordered_map<int, bool> keyStates;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  if (action == GLFW_PRESS) {
    keyStates[key] = true;
  } else if (action == GLFW_RELEASE) {
    keyStates[key] = false;
  }
}

void processKeyStates() {
  const float scaleFactor = 0.1F * julia_data.scale;
  auto currentTime = std::chrono::high_resolution_clock::now();
  double elapsed =
      std::chrono::duration<double>(currentTime - lastActionTime).count();
  if (elapsed >= actionInterval) {
    if (keyStates[GLFW_KEY_K]) {
      julia_data.y += scaleFactor;
    }
    if (keyStates[GLFW_KEY_J]) {
      julia_data.y -= scaleFactor;
    }
    if (keyStates[GLFW_KEY_L]) {
      julia_data.x += scaleFactor;
    }
    if (keyStates[GLFW_KEY_H]) {
      julia_data.x -= scaleFactor;
    }
    if (keyStates[GLFW_KEY_Z]) {
      julia_data.scale -= scaleFactor;
    }
    if (keyStates[GLFW_KEY_U]) {
      julia_data.scale += scaleFactor;
    }

    if (keyStates[GLFW_KEY_C]) {
      a += 0.01;
      euler(a);
    }
    if (keyStates[GLFW_KEY_I]) {
      julia_data.max_iter++;
    }
  }
}

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Julia Set", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  Shader shaderProgram("shaders/shader.vert", "shaders/bonito.frag");
  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(rectangle_data, sizeof(rectangle_data));

  VAO1.LinkVBO(VBO1, 0);
  VAO1.Unbind();
  VBO1.Unbind();

  glfwSetKeyCallback(window, key_callback);
  while (!glfwWindowShouldClose(window)) {
    // glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    processKeyStates();
    shaderProgram.Activate();
    glUniform1f(0, WINDOW_WIDTH);
    glUniform1f(1, WINDOW_HEIGHT);
    glUniform2f(2, -2.0F * julia_data.scale + julia_data.x,
                1.0F * julia_data.scale + julia_data.x);
    glUniform2f(3, -1.0F * julia_data.scale + julia_data.y,
                1.0F * julia_data.scale + julia_data.y);
    glUniform1ui(4, julia_data.max_iter);
    glUniform2f(5, cReal, cImag);
    VAO1.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  VAO1.Delete();
  VBO1.Delete();
  shaderProgram.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
