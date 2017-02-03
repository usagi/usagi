#pragma once

#include "type.hxx"

namespace usagi::json::picojson
{
  static inline auto to_bool( const boolean_type in ) { return in; }
  
  /// @note ECMA-262 NaN: Boolean( 0/0 ) -> false
  /// @note ECMA-262 +Inf: Boolean( +1/0 ) -> true
  /// @note ECMA-262 -Inf: Boolean( -1/0 ) -> true
  static inline auto to_bool( const number_type in )
  {
    return not std::isnan( in ) and in != 0;
  }
  
  /// @note ECMA-262 empty-string: Boolean( "" ) -> false
  static inline auto to_bool( const string_type& in )
  {
    return not in.empty();
  }
  
  /// @note: ECMA-262 array: Boolean( [] ) -> true
  static inline auto to_bool( const array_type& )
  { return true; }
  
  /// @note: ECMA-262 object: Boolean( {} ) -> true
  static inline auto to_bool( const object_type& )
  { return true; }
  
  /// @note: ECMA-262 null: Boolean( null ) -> false
  static inline auto to_bool( const null_type& = null_type() )
  { return false; }
  
  /// @brief ECMA-262 Boolean( in ) 互換変換
  static inline auto to_bool( const value_type& in )
  {
    if ( in.is< boolean_type >() )
      return to_bool( in.get< boolean_type >() );
    
    if ( in.is< number_type >() )
      return to_bool( in.get< number_type >() );
    
    if ( in.is< string_type >() )
      return to_bool( in.get< string_type >() );
    
    if ( in.is< array_type >() )
      return to_bool( in.get< array_type >() );
    
    if ( in.is< object_type >() )
      return to_bool( in.get< object_type >() );
    
    if ( in.is< null_type >() )
      return to_bool();
    
    return false;
  }
  
  /// @brief to_bool した結果を value_type で得る syntax sugar
  static inline auto to_bool_value( const value_type& in )
  { return value_type( to_bool( in ) ); }
}