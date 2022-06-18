#pragma once

#include <vector>

#include "glad/glad.h"

struct VAO final {
  GLuint id;
  // in C++ C array are passed by pointer
  VAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
  void bind();
  void unbind();
};