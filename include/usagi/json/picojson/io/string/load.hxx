#pragma once

#include "../../type.hxx"

inline namespace usagi::json::picojson::io::string
{
  static inline auto load( const std::string& json_string )
    -> value_type
  {
    value_type v;
    const auto& e = ::picojson::parse( v, json_string );
    
    if ( not e.empty() )
      throw std::runtime_error( e );
    
    return v;
  }
  
  static inline auto load( const char* json_const_char_pointer )
  { return load( std::string( json_const_char_pointer ) ); }
  
  static inline auto load_optional( const std::string& in )
    -> boost::optional< value_type >
  { 
    try
    { return load( in ); }
    catch ( ... )
    { return { }; }
  }
  
  static inline auto load_optional( const char* in )
    -> boost::optional< value_type >
  { 
    try
    { return load( in ); }
    catch ( ... )
    { return { }; }
  }
}