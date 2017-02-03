#pragma once

#include "make_value.hxx"

namespace usagi::json::picojson
{
  namespace detail
  {
    template < typename T >
    static inline decltype( auto ) emplace
    ( object_type& o, const std::string& k, const T& v )
    {
      o.emplace( k, make_value( v ) );
      return o;
    }
    
    static inline decltype( auto ) make_object_internal( object_type& o )
    {
      return o;
    }
    
    template < typename HEAD, typename ... TAIL >
    static inline decltype( auto ) make_object_internal
    ( object_type& o, const std::string& k, const HEAD& v, const TAIL& ... vs )
    {
      return make_object_internal( emplace( o, k, v ), vs ... );
    }
  }
  
  template < typename ... TS >
  static inline auto make_object( const TS& ... vs )
  {
    object_type o;
    return detail::make_object_internal( o, vs ... );
  }
  
  template < typename ... TS >
  static inline auto make_object_value
  ( const TS& ... vs )
  {
    return value_type( make_object( vs ... ) );
  }
}