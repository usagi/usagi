#pragma once

#include <cstdint>
#include <iostream>

namespace usagi
{
  namespace container
  {
    struct indices_2d final
    {
      std::size_t x = 0;
      std::size_t y = 0;
    };
    
    inline static decltype(auto) operator<<( std::ostream& o , const indices_2d& indices )
    { return o << indices.x << " " << indices.y; }
  }
}
