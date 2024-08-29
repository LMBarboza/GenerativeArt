#include "../include/shader.hpp"
#include <cstdio>
#include <sstream>
#include <string>

std::string parseShader(const std::string &filepath) {
  std::ifstream stream(filepath);
  std::string line;
  std::stringstream ss;
  while (getline(stream, line)) {
    ss << line << "\n";
  }
  return ss.str();
}

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile) {
  std::string vertexCode = parseShader(vertexFile);
  std::string fragmentCode = parseShader(fragmentFile);

  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate() { glUseProgram(ID); }

void Shader::Delete() { glDeleteProgram(ID); }
