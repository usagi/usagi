#pragma once

#include "macro.hxx"
#include "structured_response_type.hxx"

#include <boost/optional.hxx>

#include <string>

namespace usagi::http
{
  using namespace std;
  using boost::optional;
  
  /// @brief HTTP RESPONSE 文字列を HEADER と BODY に分離する
  /// @param in RESPONSE 文字列
  /// @note 出力のメンバー headers の key は全て小文字に自動的に統一される
  USAGI_HTTP_STATIC inline auto split_response( const string& in )
    -> structured_response_type;
  
  USAGI_HTTP_STATIC inline auto split_response_optional( const string& in )
    -> optional< structured_response_type >;
  
  USAGI_HTTP_STATIC inline auto split_response_optional( const optional< string >& in )
    -> optional< structured_response_type >;
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "split_response.cxx"
#endif