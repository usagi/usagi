#pragma once

#include "head.hxx"

namespace usagi::http
{
  auto head( const string& url )
    -> structured_response_type
  { return split_response( request( url, method_head ) ); }
  
  auto head_optional( const string& url )
    -> optional< structured_response_type >
  {
    if ( const auto response = request_optional( url, method_head ) )
      return split_response_optional( response );
    return { };
  }
}