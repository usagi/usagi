#pragma once

namespace usagi::http
{
  // HTTP 1.0
  constexpr auto method_head    = "HEAD";
  constexpr auto method_get     = "GET";
  constexpr auto method_post    = "POST";
  constexpr auto method_put     = "PUT";
  constexpr auto method_delete  = "DELETE";
  // HTTP 1.1
  constexpr auto method_trace   = "TRACE";
  constexpr auto method_options = "OPTIONS";
  constexpr auto method_patch   = "PATCH";
  constexpr auto method_link    = "LINK";
  constexpr auto method_unlink  = "UNLINK";
  
  /// @brief 改行文字列
  constexpr auto http_end_of_line = "\r\n";
  /// @brief 改行文字列2回
  constexpr auto http_separator   = "\r\n\r\n";
  constexpr auto size_of_http_end_of_line = static_cast< std::size_t >( 2 );
  constexpr auto size_of_http_separator   = static_cast< std::size_t >( 4 );
}
