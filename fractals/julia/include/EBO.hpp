#ifndef EBO_CLASS_HPP
#define EBO_CLASS_HPP

#include <glad/glad.h>

class EBO {
  GLuint ID;
  EBO(GLuint *indices, GLsizeiptr size);

  void Bind();

  void Unbind();

  void Delete();
};

#endif
