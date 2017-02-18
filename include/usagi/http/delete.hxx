#pragma once

#include "macro.hxx"
#include "request.hxx"
#include "split_response.hxx"

namespace usagi::http
{
  /// @note delete は定義できないので delete_ と変則
  USAGI_HTTP_STATIC inline auto delete_( const string& url )
    -> structured_response_type
  ;
  
  USAGI_HTTP_STATIC inline auto delete_optional( const string& url )
    -> optional< structured_response_type >
  ;
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "get.cxx"
#endif