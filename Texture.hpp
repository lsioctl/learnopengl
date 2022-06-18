#pragma once

#include "glad/glad.h"

struct Texture final {
  Texture(const char* image_source, GLuint image_format);
  GLuint id{0};
  void bind();
};