#include <iostream>

#include "stb_image.h"

#include "Texture.hpp"

Texture::Texture(const char* image_source, GLuint image_format)
{
  // OpenGL y axis starts on the bottom and most images start on the top
  // this tells stbi to flip y-axis for us
  stbi_set_flip_vertically_on_load(true);
  // Load texture image
  int width{0};
  int height{0};
  int nrChannels{0};
  unsigned char *data = stbi_load(image_source, &width, &height, &nrChannels, 0);

  // This will create an array of 1 Gluint elements
  glGenTextures(1, &id);
  bind();

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  /**
   * To specify the filtering method between mipmap levels we can replace the original filtering methods with one of the following four options:

    GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
    GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
    GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
    GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.
  */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // Magnification filter does not use mipmaps, which are used only for downscaling
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (data != nullptr)
  {
    // Generate the texture with the loaded image data
    glTexImage2D(
      GL_TEXTURE_2D,  // texture target, generated texture will only affect textures bound to this target
      0,  // mipmap level we want this texture to be generated, we leave the default
      image_format,  // storage format of the resulting texture in OpenGL
      width,  // of the restulting texture
      height, // of the resulting texture
      0,  // legacy stuff, should always be 0
      image_format,  // format of the source image
      GL_UNSIGNED_BYTE,  // format of the source image
      data // actual source image data
    );
    // now the currently bound texture object as the texture image bound to it

    // automatically generate mipmaps for the currently bound texture
    glGenerateMipmap(GL_TEXTURE_2D);
  } else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  // free the image data as it is not used anymore
  stbi_image_free(data);
  
  unbind();
}

void Texture::bind()
{
  glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
  // unbind the texture with binding the default texture (0)
  glBindTexture(GL_TEXTURE_2D, 0);
}