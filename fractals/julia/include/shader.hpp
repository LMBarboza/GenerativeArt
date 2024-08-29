#ifndef SHADER_HPP
#define SHADER_HPP

#include <cerrno>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

std::string get_file_contents(const char *filename);

struct Shader {
  GLuint ID;

  Shader(const char *vertexFile, const char *fragmentFile);

  void Activate();

  void Delete();
};
#endif
