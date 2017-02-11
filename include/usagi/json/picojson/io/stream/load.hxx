#pragma once

#include "../../type.hxx"
#include "type.hxx"

#include <boost/optional.hpp>

namespace usagi::json::picojson::io::stream
{
  static inline auto load( std::istream_iterator< char > in )
    -> value_type
  {
    std::string e;
    value_type v;
    
    ::picojson::parse( v, in, std::istream_iterator< char >(), &e );
    
    if ( not e.empty() )
      throw std::runtime_error( e );
    
    return v;
  }
}