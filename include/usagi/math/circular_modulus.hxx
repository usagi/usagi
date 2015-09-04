#pragma once

namespace usagi
{
  namespace math
  {
    template < typename TX, typename TY >
    inline static auto circular_modulus( const TX x, const TY y )
    {
      const auto r = x % y;
      return r < 0 ? r + y : r;
    }
  }
}
