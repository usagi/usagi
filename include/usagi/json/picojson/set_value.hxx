#pragma once

#include "type.hxx"

#include <picojson.h>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include <memory>

namespace usagi::json::picojson
{
  namespace detail
  {
    /// @brief object_type に対しドット区切りのパスで object_type の階層を必要なら作成しながら辿り末梢の要素の参照を返す
    static inline decltype( auto ) make_object_path
    ( object_type& root_object
    , const std::string& dot_separated_path
    )
    {
      std::vector< std::string > path;
      boost::split( path, dot_separated_path, boost::is_any_of( "." ) );
      
      auto peripheral = &root_object;
      
      const auto last_path = std::move( path.back() );
      path.pop_back();
      
      for ( const auto& path_part : path )
      {
        const auto i = peripheral->find( path_part );
        if ( i == peripheral->cend() or not i->second.is< object_type >() )
          (*peripheral)[ path_part ] = value_type( object_type() );
        peripheral = &(*peripheral)[ path_part ].get< object_type >();
      }
      
      return (*peripheral)[ last_path ];
    }
  }
  
  /// @brief element_value が rvalue の場合向けの set_value
  static inline auto set_value
  ( object_type& root_object
  , const std::string& dot_separated_path
  , value_type&& element_value
  )
  { detail::make_object_path( root_object, dot_separated_path ) = std::move( element_value ); }
  
  /// @brief object_type に対しドット区切りのパスで object_type の階層を辿り value_type を放り込む
  static inline auto set_value
  ( object_type& root_object
  , const std::string& dot_separated_path
  , const value_type& element_value
  )
  { detail::make_object_path( root_object, dot_separated_path ) = element_value; }
  
  /// @brief value_type へ可換な T 型の値に対応する set_value の syntax sugar ラッパー
  template < typename T = null_type >
  static inline auto set_value( object_type& root_object, const std::string& dot_separated_path, const T& element_value = T() )
  { set_value( root_object, dot_separated_path, value_type( static_cast< double >( element_value ) ) ); }
  
  template < >
  inline auto set_value< null_type >( object_type& root_object, const std::string& dot_separated_path, const null_type& )
  { set_value( root_object, dot_separated_path, value_type() ); }
  
  template < >
  inline auto set_value< array_type >( object_type& root_object, const std::string& dot_separated_path, const array_type& element_value )
  { set_value( root_object, dot_separated_path, value_type( element_value ) ); }
  
  template < >
  inline auto set_value< object_type >( object_type& root_object, const std::string& dot_separated_path, const object_type& element_value )
  { set_value( root_object, dot_separated_path, value_type( element_value ) ); }
  
  template < >
  inline auto set_value< value_type >( object_type& root_object, const std::string& dot_separated_path, const value_type& element_value )
  { set_value( root_object, dot_separated_path, element_value ); }
  
  template < >
  inline auto set_value< std::string >( object_type& root_object, const std::string& dot_separated_path, const std::string& element_value )
  { set_value( root_object, dot_separated_path, value_type( element_value ) ); }
  
  static inline auto set_value( object_type& root_object, const std::string& dot_separated_path, const char* element_value )
  { set_value( root_object, dot_separated_path, value_type( element_value ) ); }
  
  static inline auto set_value( object_type& root_object, const std::string& dot_separated_path, const bool element_value )
  { set_value( root_object, dot_separated_path, value_type( element_value ) ); }
  
  /// root_value が object_type を内包する value_type の場合に対応する set_value の syntax sugar ラッパー
  template < typename T = null_type >
  static inline auto set_value( value_type& root_value, const std::string& dot_separated_path, T&& element_value = T() )
  { set_value( root_value.get< object_type >(), dot_separated_path, std::move( element_value ) ); }
  
}