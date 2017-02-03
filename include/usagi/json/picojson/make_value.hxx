/// @file
/// @brief 任意の何かから value_type を"生成"する make_value さん群

#pragma once

#include "type.hxx"

namespace usagi::json::picojson
{
  static inline auto make_value(                       ) -> value_type { return value_type(); }
  static inline auto make_value( const null_type       ) -> value_type { return make_value(); }
  static inline auto make_value( const value_type&  in ) -> value_type { return value_type( in ); }
  static inline auto make_value( value_type&&       in ) -> value_type { return std::move( in ); }
  static inline auto make_value( const array_type&  in ) -> value_type { return value_type( in ); }
  static inline auto make_value( array_type&        in ) -> value_type { return value_type( std::move( in ) ); }
  static inline auto make_value( const object_type& in ) -> value_type { return value_type( in ); }
  static inline auto make_value( object_type&&      in ) -> value_type { return value_type( std::move( in ) ); }
  static inline auto make_value( const char*        in ) -> value_type { return value_type( in ); }
  static inline auto make_value( const std::string& in ) -> value_type { return value_type( in ); }
  static inline auto make_value( std::string&&      in ) -> value_type { return value_type( std::move( in ) ); }
  static inline auto make_value( const bool         in ) -> value_type { return value_type( in ); }
  
  /// @exception std::overflow_error nan, inf を与えた場合、 picojson::value の ctor から例外が送出される可能性がある
  template < typename T >
  static inline auto make_value
  ( const T v
  ) noexcept -> value_type
  {
    return value_type( static_cast< double >( v ) );
  }
  
  /// @brief picojson::value へ double を与えて構築する場合の nan, inf による std::overflow_error 例外を抑止する版
  /// @note nan, inf を入力した場合は null になる
  template < typename T >
  static inline auto make_value_noexcept
  ( const T v
  ) noexcept -> value_type
  {
    try { return make_value( v ); }
    catch ( const std::overflow_error& ) { return value_type(); }
  }
  
  /// @brief nan, inf を与えても picojson::value の ctor の isnan, isinf からの std::overflow_error を回避して無理矢理 nan, inf を持つ picojson::value を生成する版
  /// @warning ECMA-404 としては不正、シリアライズで nan, inf が発生した JSON もどき文字列を picojson::parse してもエラーとなる。永遠の decrecated
  [[deprecated]]
  template < typename T >
  static inline auto make_value_nanable
  ( const T v
  ) -> value_type
  {
    // nan, inf の場合に exception を起こさないトリック
    value_type r( number_type( 0 ) );
    r.get< number_type >() = static_cast< double >( v );
    return r;
  }
}