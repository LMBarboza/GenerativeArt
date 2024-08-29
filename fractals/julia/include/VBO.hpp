#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

struct VBO {
  GLuint ID;
  VBO(GLfloat *vertices, GLsizeiptr size);

  void Bind();
  void Unbind();
  void Delete();
};

#endif
