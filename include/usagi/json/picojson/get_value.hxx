#pragma once

#include "type.hxx"

#include <picojson.h>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <sstream>

namespace usagi::json::picojson
{
  /// @brief value_type に対しドット区切りのパスで object_type の階層を辿り value_type を引っ張り出す
  static inline auto get_value( const value_type& source, const std::string& dot_separated_path )
    -> value_type&
  {
    std::vector< std::string > path;
    boost::split( path, dot_separated_path, boost::is_any_of( "." ) );
    
    auto out = const_cast< const value_type* >( &source );
    for ( const auto& path_part : path )
      out = &out->get< object_type >().at( path_part );
    
    return const_cast< value_type& >( *out );
  }
  
  /// @brief get_value が out_of_range や runtime_error など例外で失敗する場合に optional で例外の送出をカバーする版
  static inline auto get_value_optional( const value_type& source, const std::string& dot_separated_path )
    noexcept
    -> boost::optional< value_type& >
  {
    try
    { return get_value( source, dot_separated_path ); }
    catch ( ... )
    { return { }; }
  }
  
  /// @brief get_value + picojson::get + 可能な限りの自動的な型変換（ double や string を float で取り出したり、 null を string で取り出したりもできる ）
  /// @param type_conversion true の場合には可能な限りの自動的な型変換を試みる。 false の場合には value_type::get のみ。
  template < typename T >
  static inline auto get_value_as
  ( const value_type& source
  , const std::string& dot_separated_path
  , const bool type_conversion = true
  ) -> T
  {
    const auto& v = get_value( source, dot_separated_path );
    
    if ( type_conversion )
    {
      // T が数値型の場合の変換込みの処理
      if ( std::is_integral< T >::value or std::is_floating_point< T >::value )
      {
        // double -> T
        if ( v.is< double >() )
          return static_cast< T >( v.get< double >() );
        
        // string -> T
        if ( v.is< std::string >() )
        {
          std::stringstream s;
          s << v.get< std::string >();
          T out;
          s >> out;
          
          if ( s.fail() )
            throw std::runtime_error( "cannot convert to a number type from std::string type." );
          
          return out;
        }
      }
    }
    
    throw std::runtime_error( "cannot convert to a number type from value_type type." );
  }
  
  template < >
  inline auto get_value_as< double >
  ( const value_type& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> double
  {
    const auto& v = get_value( source, dot_separated_path );
    
    if ( v.is< double >() )
      return v.get< double >();
    
    if ( type_conversion )
    {
      // string -> double
      if ( v.is< std::string >() )
      {
        std::stringstream s;
        s << v.get< std::string >();
        double out;
        s >> out;
        
        if ( s.fail() )
          throw std::runtime_error( "cannot convert to a number type from std::string type." );
        
        return out;
      }
    }
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< double >();
  }
  
  template < >
  inline auto get_value_as< std::string >
  ( const value_type& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> std::string
  {
    const auto& v = get_value( source, dot_separated_path );
    
    if ( v.is< std::string >() )
      return v.get< std::string >();
    
    if ( type_conversion )
    {
      // value_type の operator<< で string にして返す
      std::stringstream s;
      s << v;
      return s.str();
    }
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< std::string >();
  }
  
  template < >
  inline auto get_value_as< object_type >
  ( const value_type& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> object_type
  {
    const auto& v = get_value( source, dot_separated_path );
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< object_type >();
  }
  
  template < >
  inline auto get_value_as< array_type >
  ( const value_type& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> array_type
  {
    const auto& v = get_value( source, dot_separated_path );
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< array_type >();
  }
  
  /// @brief get_value_as が out_of_range や runtime_error など例外で失敗する場合に optional で例外の送出をカバーする版
  template < typename T >
  static inline auto get_value_as_optional
  ( const value_type& source
  , const std::string& dot_separated_path
  , const bool type_conversion = true
  ) noexcept -> boost::optional< T >
  {
    try
    { return get_value_as< T >( source, dot_separated_path, type_conversion ); }
    catch ( ... )
    { return { }; }
  }
  
}
