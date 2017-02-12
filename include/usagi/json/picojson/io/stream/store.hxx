#pragma once

#include "../../type.hxx"

namespace usagi::json::picojson::io::stream
{
  static inline auto store( const value_type& v, std::ostream_iterator< char > out )
  {
    v.serialize( out );
  }
}