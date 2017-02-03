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
  
  template < typename T >
  static inline auto make_value
  ( const T v
  ) -> value_type
  { return value_type( static_cast< double >( v ) ); }
}