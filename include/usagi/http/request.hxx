#pragma once

#include "macro.hxx"
#include "io_service.hxx"

#ifndef DISABLE_USAGI_HTTP_JSON
  #include <usagi/json/picojson/type.hxx>
#endif

#include <string>
#include <stdexcept>
#include <vector>

#include <boost/optional.hpp>

namespace usagi::http
{
  /// @brief 単純な HEAD や GET あるいは BODY にデータを詰めた REQUEST を行い RESPONSE を得る
  /// @param url URL
  /// @param method "HEAD" や "GET" など。 method_head などの定義済みの文字列を用いる事を推奨。
  /// @param data REQUEST の BODY に詰めたいデータがある場合の先頭アドレス値
  /// @param data_size data の長さ [bytes]
  /// @extra_header REQUEST のヘッダーに追加したい文字列
  USAGI_HTTP_USAGI_HTTP_STATIC inline auto request
  ( const string&  url
  , const string&  method
  , const void*    data
  , const size_t   data_size
  , const string&  extra_header = ""
  ) -> string;
  
  USAGI_HTTP_STATIC inline auto request
  ( const string&  url
  , const string&  method
  ) -> string;
  
  /// @brief data が vector< T > の場合の syntax sugar
  /// @tparam T string または POD 型を内包する vector
  /// @note 厳密には .data() で先頭アドレス、 .size() で長さ [bytes]、 [ 0 ] で先頭オブジェクトを取得可能な型 T に対して動作する。
  template < typename T >
  static inline auto request
  ( const string&  url
  , const string&  method
  , const T&       data
  , const string&  extra_header = ""
  ) -> string
  { return request( url, method, reinterpret_cast< const char* >( data.data() ), data.size() * sizeof( decltype( data[ 0 ] ) ), extra_header ); }
  
#ifndef DISABLE_USAGI_HTTP_JSON
  /// @brief data が picojson::value の場合の syntax sugar
  /// @note 自動的に extra_header の冒頭に "Content-Type: application/json\r\n" を追加する
  USAGI_HTTP_STATIC inline auto request
  ( const string&  url
  , const string&  method
  , const ::usagi::json::picojson::value_type& data
  , const string&  extra_header = ""
  ) -> string;
#endif
  
  USAGI_HTTP_STATIC inline auto request_optional
  ( const string&  url
  , const string&  method
  , const void*    data         = nullptr
  , const size_t   data_size    = 0
  , const string&  extra_header = ""
  ) -> optional< string >;
  
  /// @brief data が string の場合の syntax sugar
  USAGI_HTTP_STATIC inline auto request_optional
  ( const string&  url
  , const string&  method
  , const string&  data
  , const string&  extra_header = ""
  ) -> optional< string >;
  
  /// @brief data が vector< T > の場合の syntax sugar
  /// @tparam T string または POD 型を内包する vector
  /// @note 厳密には .data() で先頭アドレス、 .size() で長さ [bytes]、 [ 0 ] で先頭オブジェクトを取得可能な型 T に対して動作する。
  template < typename T >
  static inline auto request_optional
  ( const string&  url
  , const string&  method
  , const T&       data
  , const string&  extra_header = ""
  ) -> optional< string >
  { try { return request( url, method, data, extra_header ); } catch ( ... ) { return { }; } }
  
#ifndef DISABLE_USAGI_HTTP_JSON
  /// @brief data が picojson::value の場合の syntax sugar
  USAGI_HTTP_STATIC inline auto request_optional
  ( const string&  url
  , const string&  method
  , const ::usagi::json::picojson::value_type& data
  , const string&  extra_header = ""
  ) -> optional< string >;
#endif
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "request.cxx"
#endif
