#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/camera.hpp"
#include "../include/shader_m.hpp"
#include <cmath>
#include <vector>

// Include or implement your noise function library here

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 50.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// OpenGL variables
GLuint terrainVAO, terrainVBO, terrainIBO;
std::vector<float> vertices;
std::vector<unsigned int> indices;

// Terrain settings
int gridSize = 100;
float spacing = 1.0f;
glm::vec3 lastCenter = camera.Position;

// Function to get height from noise
float getHeight(float x, float z) {
  float scale = 0.1f; // Adjust for larger/smaller terrain features
  return noise(scale * x, scale * z) * 20.0f; // Adjust the amplitude
}

// Terrain generation function
void generateTerrain(float centerX, float centerZ) {
  vertices.clear();
  indices.clear();

  for (int i = -gridSize; i < gridSize; ++i) {
    for (int j = -gridSize; j < gridSize; ++j) {
      float x = centerX + i * spacing;
      float z = centerZ + j * spacing;
      float y = getHeight(x, z);

      // Add vertex
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
    }
  }

  // Generate indices for triangle strips
  for (int i = 0; i < gridSize - 1; ++i) {
    for (int j = 0; j < gridSize - 1; ++j) {
      int startIndex = i * gridSize + j;

      indices.push_back(startIndex);
      indices.push_back(startIndex + gridSize);
      indices.push_back(startIndex + 1);

      indices.push_back(startIndex + 1);
      indices.push_back(startIndex + gridSize);
      indices.push_back(startIndex + gridSize + 1);
    }
  }

  // Update OpenGL buffers
  glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0],
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_DYNAMIC_DRAW);
}

// Input handling
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Mouse callback
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // Reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// Scroll callback
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

int main() {
  // Initialize GLFW and OpenGL context
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dynamic Terrain", NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glEnable(GL_DEPTH_TEST);

  Shader heightMapShader("heightmap.vs", "heightmap.fs");

  // Set up vertex data and buffers
  glGenVertexArrays(1, &terrainVAO);
  glGenBuffers(1, &terrainVBO);
  glGenBuffers(1, &terrainIBO);

  glBindVertexArray(terrainVAO);

  glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), NULL,
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               NULL, GL_DYNAMIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  generateTerrain(camera.Position.x, camera.Position.z);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glm::vec3 cameraPos = camera.Position;
    if (glm::distance(cameraPos, lastCenter) > gridSize / 2) {
      generateTerrain(cameraPos.x, cameraPos.z);
      lastCenter = cameraPos;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    heightMapShader.use();
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    heightMapShader.setMat4("projection", projection);
    heightMapShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    heightMapShader.setMat4("model", model);

    glBindVertexArray(terrainVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &terrainVAO);
  glDeleteBuffers(1, &terrainVBO);
  glDeleteBuffers(1, &terrainIBO);

  glfwTerminate();
  return 0;
}
