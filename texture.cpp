#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "texture.hpp"
namespace Yuru
{
  void Texture::Load(const char* path)
  {
    glGenTextures(1, &texAttribs.texture);
    glBindTexture(GL_TEXTURE_2D, texAttribs.texture);

    //Texure wrapping & filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path
                                    , &texAttribs.width
                                    , &texAttribs.height
                                    , &texAttribs.channels
                                    , 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D
                  , 0
                  ,GL_RGB
                  , texAttribs.width
                  , texAttribs.height
                  , 0
                  , GL_RGB
                  , GL_UNSIGNED_BYTE
                  , data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
    }
    stbi_image_free(data);
  }

  void Texture::SetPosition(const int x, const int y, const int spriteSize, const int texWidth, const int texHeight)
  {
    position.posX = x;
    position.posY = y;
    position.leftX =    x * spriteSize        / static_cast<float>(texWidth);
    position.rightX =   (x + 1) * spriteSize  / static_cast<float>(texWidth);
    position.bottomY =  y * spriteSize        / static_cast<float>(texHeight);
    position.topY =     (y + 1) * spriteSize  / static_cast<float>(texHeight);

  }
}
