#pragma once

#include "../../type.hxx"

namespace usagi::json::picojson::io::string
{
  static inline auto store( const value_type& in, string_type& out )
  {
    out = in.serialize();
  }
  
  static inline auto store_noexcept( const value_type& in, string_type& out )
    noexcept
    -> bool
  {
    try
    { store( in, out ); }
    catch ( ... )
    { return false; }
    return true;
  }
}