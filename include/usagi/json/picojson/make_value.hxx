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
  
  /// @note: nan, inf を入力した場合も例外を起こさず nan, inf を保持した value_type を生成する
  template < typename T >
  static inline auto make_value
  ( const T v
  ) -> value_type
  {
    // nan, inf の場合に exception を起こさないトリック
    value_type r( number_type( 0 ) );
    r.get< number_type >() = static_cast< double >( v );
    return r;
  }
}