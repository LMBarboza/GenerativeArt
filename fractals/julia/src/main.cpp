#define GLFW_INCLUDE_NONE
// #include "../include/EBO.hpp"
#include "../include/VAO.hpp"
#include "../include/VBO.hpp"
#include "../include/julia.hpp"
#include "../include/shader.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

GLfloat rectangle_data[]{-1.0F, -1.0F, 0.0F, 1.0F,  -1.0F, 0.0F,
                         1.0F,  1.0F,  0.0F, 1.0F,  1.0F,  0.0F,
                         -1.0F, 1.0F,  0.0F, -1.0F, -1.0F, 0.0F};

int main() {
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

  glViewport(0, 0, 800, 800);
  Shader shaderProgram("shaders/shader.vert", "shaders/bonito.frag");
  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(rectangle_data, sizeof(rectangle_data));

  VAO1.LinkVBO(VBO1, 0);
  VAO1.Unbind();
  VBO1.Unbind();

  JuliaData julia_data{1.0F, 0.0F, 0.0F, 200};
  while (!glfwWindowShouldClose(window)) {
    // glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.Activate();
    glUniform1f(0, WINDOW_WIDTH);
    glUniform1f(1, WINDOW_HEIGHT);
    glUniform2f(2, -2.0F * julia_data.scale + julia_data.x,
                1.0F * julia_data.scale + julia_data.x);
    glUniform2f(3, -1.0F * julia_data.scale + julia_data.y,
                1.0F * julia_data.scale + julia_data.y);
    glUniform1ui(4, julia_data.max_iter);
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
