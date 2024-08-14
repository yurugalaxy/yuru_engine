//
// Created by kae on 14/08/24.
//

#pragma once
#include <memory>

namespace Yuru
{
  template<typename T>
  using Shared = std::shared_ptr<T>;

  template<typename T>
  using Unique = std::unique_ptr<T>;
}
