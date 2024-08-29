#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP

#include "VBO.hpp"
#include <glad/glad.h>

struct VAO {
  GLuint ID;
  VAO();

  void LinkVBO(VBO &VBO, GLuint layout);

  void Bind();

  void Unbind();

  void Delete();
};
#endif
