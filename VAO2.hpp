#pragma once

#include <vector>

#include "glad/glad.h"

/**
 * VAO2 created to avoid breaking existing API and changes are:
 * - no EBO
 * - the vertices array does only include vertices and textures (so stride of 5)
 * 
 * TODO: find a better OOD, I will wait until other examples
 */

struct VAO2 final {
  GLuint id;
  // in C++ C array are passed by pointer
  VAO2(const std::vector<float>& vertices);
  void bind();
  void unbind();
};