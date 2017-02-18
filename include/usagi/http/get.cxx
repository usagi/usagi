#pragma once

#include "get.hxx"

namespace usagi::http
{
  auto get( const string& url )
    -> structured_response_type
  { return split_response( request( url, method_get ) ); }
  
  auto get_optional( const string& url )
    -> optional< structured_response_type >
  {
    if ( const auto response = request_optional( url, method_get ) )
      return split_response_optional( response );
    return { };
  }
}