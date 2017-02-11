#pragma once

#include "type.hxx"

#include <boost/optional.hpp>

#include <fstream>

namespace usagi::json::picojson::io::filesystem
{
  static inline auto store( const value_type& in, const path_type& filesystem_path )
    -> void
  {
    std::ofstream f;
    f.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    f.open( filesystem_path.string() );
    f << in;
  }
  
  static inline auto store_noexcept( const value_type& in, const path_type& filesystem_path )
    noexcept
    -> bool
  {
    try
    { store( in, filesystem_path ); }
    catch ( ... )
    { return false; }
    return true;
  }
}