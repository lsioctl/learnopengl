#pragma once

#include "glad/glad.h"

struct Texture final {
  Texture(const char* image_source);
  GLuint id{0};
  void bind();
};