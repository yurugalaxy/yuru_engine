//
// Created by kae on 14/08/24.
//

#pragma once

namespace Yuru
{
  struct TextureAttributes
  {
    unsigned int texture {};
    int width {};
    int height {};
    int channels {};
  };

  struct TexturePosition
  {
    float leftX { 0 };
    float rightX { 1 };
    float bottomY { 0 };
    float topY { 1 };
  };

  class Texture
  {
  public:
    void Load(const char* path);
    void SetPosition(int x, int y, int spriteSize, int texWidth, int texHeight);

  public:
    TexturePosition position;

  private:
    TextureAttributes texAttribs;
  };
}