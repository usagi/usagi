#pragma once

#include <picojson.h>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <sstream>

namespace usagi::json::picojson
{
  using object_type = picojson::object;
  using array_type  = picojson::array;
  using value_type  = picojson::value;
  using null_type   = picojson::null;
  
  static inline auto emplace( object_type& o, const std::string& k, const null_type ) { o.emplace( k, value_type() ); }
  static inline auto emplace( object_type& o, const std::string& k, const value_type& v ) { o.emplace( k, v ); }
  static inline auto emplace( object_type& o, const std::string& k, const array_type& v ) { o.emplace( k, value_type( v ) ); }
  static inline auto emplace( object_type& o, const std::string& k, const object_type& v ) { o.emplace( k, value_type( v ) ); }
  static inline auto emplace( object_type& o, const std::string& k, const char* v ) { o.emplace( k, value_type( v ) ); }
  static inline auto emplace( object_type& o, const std::string& k, const std::string& v ) { o.emplace( k, value_type( v ) ); }

  template < typename T >
  static inline auto emplace( object_type& o, const std::string& k, const T& v )
  { o.emplace( k, static_cast< double >( v ) ); }

  static inline auto make_object_internal( object_type& )
  { }

  template < typename HEAD, typename ... TAIL >
  static inline auto make_object_internal( object_type& o, const std::string& k, const HEAD& v, const TAIL& ... vs )
  {
    emplace( o, k, v );
    make_object_internal( o, vs ... );
  }

  template < typename ... TS >
  static inline auto make_object( const TS& ... vs )
  {
    object_type o;
    make_object_internal( o, vs ... );
    return o;
  }
  
  /// @brief picojson::value に対しドット区切りのパスで picojson::object の階層を辿り picojson::value を引っ張り出す
  static inline auto get_value( const picojson::value& source, const std::string& dot_separated_path )
    -> picojson::value&
  {
    std::vector< std::string > path;
    boost::split( path, dot_separated_path, boost::is_any_of( "." ) );
    
    auto out = const_cast< const picojson::value* >( &source );
    for ( const auto& path_part : path )
      out = &out->get< picojson::object >().at( path_part );
    
    return const_cast< picojson::value& >( *out );
  }
  
  /// @brief get_value が out_of_range や runtime_error など例外で失敗する場合に optional で例外の送出をカバーする版
  static inline auto get_value_optional( const picojson::value& source, const std::string& dot_separated_path )
    noexcept
    -> boost::optional< picojson::value& >
  {
    try
    { return get_value( source, dot_separated_path ); }
    catch ( ... )
    { return { }; }
  }
  
  /// @brief get_value + picojson::get + 可能な限りの自動的な型変換（ double や string を float で取り出したり、 null を string で取り出したりもできる ）
  /// @param type_conversion true の場合には可能な限りの自動的な型変換を試みる。 false の場合には picojson::value::get のみ。
  template < typename T >
  static inline auto get_value_as
  ( const picojson::value& source
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
    
    throw std::runtime_error( "cannot convert to a number type from picojson::value type." );
  }
  
  template < >
  inline auto get_value_as< double >
  ( const picojson::value& source
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
  ( const picojson::value& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> std::string
  {
    const auto& v = get_value( source, dot_separated_path );
    
    if ( v.is< std::string >() )
      return v.get< std::string >();
    
    if ( type_conversion )
    {
      // picojson::value の operator<< で string にして返す
      std::stringstream s;
      s << v;
      return s.str();
    }
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< std::string >();
  }
  
  template < >
  inline auto get_value_as< picojson::object >
  ( const picojson::value& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> picojson::object
  {
    const auto& v = get_value( source, dot_separated_path );
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< picojson::object >();
  }
  
  template < >
  inline auto get_value_as< picojson::array >
  ( const picojson::value& source
  , const std::string& dot_separated_path
  , const bool type_conversion
  ) -> picojson::array
  {
    const auto& v = get_value( source, dot_separated_path );
    
    // note: picojson による cast 失敗で適当な std::runtime_error が発行される
    return v.get< picojson::array >();
  }
  
  /// @brief get_value_as が out_of_range や runtime_error など例外で失敗する場合に optional で例外の送出をカバーする版
  template < typename T >
  static inline auto get_value_as_optional
  ( const picojson::value& source
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
