#pragma once

#include "macro.hxx"
#include "request.hxx"
#include "split_response.hxx"

namespace usagi::http
{
  auto put( const string& url, const void* data, const size_t data_size )
    -> structured_response_type
  { return split_response( request( url, method_put, data, data_size ) ); }
  
  auto put_optional( const string& url, const void* data, const size_t data_size )
    -> optional< structured_response_type >
  {
    if ( const auto response = request_optional( url, method_put, data, data_size ) )
      return split_response_optional( response );
    return { };
  }
}