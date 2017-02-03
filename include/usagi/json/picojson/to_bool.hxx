/// @file
/// @brief 任意の何かから value_type を"生成"する make_value さん群

#pragma once

#include "type.hxx"

namespace usagi::json::picojson
{
  /// @brief ECMA-262 Boolean( in ) 互換変換
  static inline auto to_bool( const value_type& in )
  {
    if ( in.is< boolean_type >() )
      return in.get< boolean_type >();
    
    if ( in.is< number_type >() )
    {
      const auto v = in.get< number_type >();
      // ECMA-262 NaN: Boolean( 0/0 ) -> false
      // ECMA-262 +Inf: Boolean( +1/0 ) -> true
      // ECMA-262 -Inf: Boolean( -1/0 ) -> true
      return not std::isnan( v ) and v != 0;
    }
    
    if ( in.is< string_type >() )
    {
      const auto& v = in.get< string_type >();
      // ECMA-262 empty-string: Boolean( "" ) -> false
      return not v.empty();
    }
    
    // ECMA-262 object: Boolean( {} ) -> true
    // ECMA-262 array: Boolean( [] ) -> true
    // ECMA-262 null: Boolean( null ) -> false
    return in.is< array_type >() or in.is< object_type >();
  }
  
  /// @brief to_bool した結果を value_type で得る syntax sugar
  static inline auto to_bool_value( const value_type& in )
  { return value_type( to_bool( in ) ); }
}