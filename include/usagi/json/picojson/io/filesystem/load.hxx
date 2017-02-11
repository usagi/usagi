#pragma once

#include "../../type.hxx"
#include "type.hxx"
#include "../stream/load.hxx"

#include <boost/optional.hpp>

#include <fstream>

namespace usagi::json::picojson::io::filesystem
{
  static inline auto load( const path_type& filesystem_path )
    -> value_type
  {
    std::ifstream f;
    f.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    f.open( filesystem_path.string() );
    
    return stream::load( std::istreambuf_iterator< char >( f ) );
  }
  
  static inline auto load_optional( const filesystem_path& in )
    -> boost::optional< value_type >
  { 
    try
    { return load( in ); }
    catch ( ... )
    { return { }; }
  }
}