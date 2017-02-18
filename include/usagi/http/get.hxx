#pragma once

#include "macro.hxx"
#include "request.hxx"
#include "split_response.hxx"

namespace usagi::http
{
  USAGI_HTTP_STATIC inline auto get( const string& url )
    -> structured_response_type
  ;
  
  USAGI_HTTP_STATIC inline auto get_optional( const string& url )
    -> optional< structured_response_type >
  ;
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "get.cxx"
#endif