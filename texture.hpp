//
// Created by kae on 14/08/24.
//

#pragma once

#include <cstdint>

namespace Yuru::Texture
{
  uint32_t Generate(const char * path, int offset);
  void SetActive(int offset);
}

