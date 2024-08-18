#include "texture.hpp"

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>

namespace Lyonesse::Texture
{
  uint32_t Generate(const char * path, const int offset)
  {
    uint32_t textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    GLenum format;
    switch (channels)
    {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        format = GL_RGB;
    }

    glActiveTexture(GL_TEXTURE0 + offset);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Texure wrapping & filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
    std::cout << "Texture " << textureID << " created.\n";

    std::cout << "Bound texture " << textureID << " to texture unit " << offset <<  ".\n";

    return textureID;
  }

  void SetActive(const int offset)
  {
    glActiveTexture(GL_TEXTURE0 + offset);
  }
}
