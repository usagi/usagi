#pragma once

#include "macro.hxx"
#include "request.hxx"
#include "split_response.hxx"

namespace usagi::http
{
  USAGI_HTTP_STATIC inline auto post( const string& url, const void* data, const size_t data_size )
    -> structured_response_type
  { return split_response( request( url, method_post, data, data_size ) ); }
  
  USAGI_HTTP_STATIC inline auto post_optional( const string& url, const void* data, const size_t data_size )
    -> optional< structured_response_type >
  {
    if ( const auto response = request_optional( url, method_post, data, data_size ) )
      return split_response_optional( response );
    return { };
  }
  
  template < typename T >
  static inline auto post( const string& url, const T& data )
    -> structured_response_type
  { return split_response( request( url, method_post, data ) ); }
  
  template < typename T >
  static inline auto post_optional( const string& url, const T& data )
    -> optional< structured_response_type >
  {
    if ( const auto response = request_optional( url, method_post, data ) )
      return split_response_optional( response );
    return { };
  }
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "get.cxx"
#endif