#pragma once

#include "macro.hxx"
#include "request.hxx"
#include "split_response.hxx"

namespace usagi::http
{
  auto post( const string& url, const void* data, const size_t data_size )
    -> structured_response_type
  { return split_response( request( url, method_post, data, data_size ) ); }
  
  auto post_optional( const string& url, const void* data, const size_t data_size )
    -> optional< structured_response_type >
  {
    if ( const auto response = request_optional( url, method_post, data, data_size ) )
      return split_response_optional( response );
    return { };
  }
}