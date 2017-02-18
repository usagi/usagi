#pragma once

#include "macro.hxx"
#include "request.hxx"
#include "split_response.hxx"

namespace usagi::http
{
  USAGI_HTTP_STATIC inline auto head( const string& url )
    -> structured_response_type
  ;
  
  USAGI_HTTP_STATIC inline auto head_optional( const string& url )
    -> optional< structured_response_type >
  ;
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "head.cxx"
#endif