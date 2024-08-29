#ifndef SHADER_HPP
#define SHADER_HPP

#include <cerrno>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

std::string parseShader(const std::string &filepath);

struct Shader {
  GLuint ID;

  Shader(const std::string &vertexFile, const std::string &fragmentFile);

  void Activate();

  void Delete();
};
#endif
