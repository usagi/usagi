#pragma once

#include <picojson.h>

namespace usagi::json::picojson
{
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
}