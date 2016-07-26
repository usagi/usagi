#pragma once

namespace glm
{
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tvec1< T >& v ) { a( cereal::make_nvp( "x", v.x ) ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tvec2< T >& v ) { a( cereal::make_nvp( "x", v.x ), cereal::make_nvp( "y", v.y ) ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tvec3< T >& v ) { a( cereal::make_nvp( "x", v.x ), cereal::make_nvp( "y", v.y ), cereal::make_nvp( "z", v.z ) ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tvec4< T >& v ) { a( cereal::make_nvp( "x", v.x ), cereal::make_nvp( "y", v.y ), cereal::make_nvp( "z", v.z ), cereal::make_nvp( "w", v.w ) ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tquat< T >& v ) { a( cereal::make_nvp( "x", v.x ), cereal::make_nvp( "y", v.y ), cereal::make_nvp( "z", v.z ), cereal::make_nvp( "w", v.w ) ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat2x2< T >& v ) { a( v[0], v[1] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat3x2< T >& v ) { a( v[0], v[1] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat4x2< T >& v ) { a( v[0], v[1] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat2x3< T >& v ) { a( v[0], v[1], v[2] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat3x3< T >& v ) { a( v[0], v[1], v[2] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat4x3< T >& v ) { a( v[0], v[1], v[2] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat2x4< T >& v ) { a( v[0], v[1], v[2], v[3] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat3x4< T >& v ) { a( v[0], v[1], v[2], v[3] ); }
  template < typename ARCHIVER, typename T > auto serialize( ARCHIVER& a, tmat4x4< T >& v ) { a( v[0], v[1], v[2], v[3] ); }
}

