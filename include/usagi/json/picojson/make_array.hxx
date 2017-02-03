#pragma once

#include "make_value.hxx"

namespace usagi::json::picojson
{
  namespace detail
  {
    template < typename T >
    static inline decltype( auto ) emplace_back
    ( array_type& a, const T& v )
    {
      a.emplace_back( make_value( v ) );
      return a;
    }
    
    static inline decltype( auto ) make_array_internal( array_type& a )
    {
      return a;
    }
    
    template < typename HEAD, typename ... TAIL >
    static inline decltype( auto ) make_array_internal
    ( array_type& a, const HEAD& v, const TAIL& ... vs )
    {
      return make_array_internal( emplace_back( a, v ), vs ... );
    }
  }
  
  template < typename ... TS >
  static inline auto make_array
  ( const TS& ... vs )
  {
    array_type a;
    return detail::make_array_internal( a, vs ... );
  }
  
  template < typename ... TS >
  static inline auto make_array_value
  ( const TS& ... vs )
  {
    return value_type( make_array( vs ... ) );
  }
}