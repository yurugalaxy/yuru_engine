//
// Created by kae on 14/08/24.
//

#pragma once

namespace Yuru
{

  class Texture
  {
  public:
   Texture(const char* path, bool transparency);

  public:
    unsigned int textureID {};

  private:
    int m_width {};
    int m_height {};
    int m_channels {};
    bool m_transparent {};
  };
}