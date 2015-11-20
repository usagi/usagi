/// @file
/// @brief useful and easy for use flag management wrapper class
/// 
/// // 1. Declaration
/// 
/// // case 1: T = enum class as a modern C++-style flag
/// enum class x { a, b, c };
/// usagi::flag::wrapper<x> o;
/// 
/// // case 2: T = enum as a C-style flag
/// enum y { d, e, f };
///
/// usagi::flag::wrapper<y> p;
///
/// // case 3: T = int or etc. as a legacy #define style flag
///
/// #define Z_G 1
/// #define Z_H ( Z_G << 1 )
/// #define Z_I ( Z_G << 2 )
///
/// usagi::flag::wrapper<decltype(Z_G)> q;
///
/// // 2. Set or reset flags
/// 
/// o + x::a + x::b - x::c; // Eq.: x j; j = ( x::a | x::b ) & ~ x::c;
/// p +    d +    e -    f; // Eq.: y k; k = (    d |    e ) & ~    f;
/// q +  Z_G +  Z_H -  Z_I; // Eq.: x l; l = (  Z_G |  Z_H ) & ~  Z_I;
///
/// // 3. Get a raw flag value from a usagi::flag::wrapper object
///
/// const auto raw_o = o.value;
/// const auto raw_p = p.value;
/// const auto raw_q = q.value;

#pragma once

#include <iostream>
#include <cstdint>
#include <type_traits>
#include <bitset>
#include <sstream>

namespace usagi
{
  namespace flag
  {
    namespace detail
    {
      template < typename T > struct _underlying_type{ using type = typename std::underlying_type<T>::type; };
      template < > struct _underlying_type< std::uint8_t  >{ using type = std::uint8_t ; };
      template < > struct _underlying_type< std::uint16_t >{ using type = std::uint16_t; };
      template < > struct _underlying_type< std::uint32_t >{ using type = std::uint32_t; };
      template < > struct _underlying_type< std::uint64_t >{ using type = std::uint64_t; };
      template < > struct _underlying_type< std::int8_t   >{ using type = std::int8_t  ; };
      template < > struct _underlying_type< std::int16_t  >{ using type = std::int16_t ; };
      template < > struct _underlying_type< std::int32_t  >{ using type = std::int32_t ; };
      template < > struct _underlying_type< std::int64_t  >{ using type = std::int64_t ; };
    }
    
    /// @brief  useful and easy for use flag management wrapper class
    /// @tparam T enum or integral flag type( `enum class`, `enum` `int` or etc. ).
    template < typename T >
    struct wrapper
    {
      T value = static_cast< T >( 0 );
      
      using underlying_type = typename _underlying_type< T >::type;
      
      static constexpr auto size = sizeof( T );
      static constexpr auto bits_of_byte = 8u;
      
      static inline auto _underlying_value( const T v ) { return static_cast< underlying_type >( v ); }
      
      flag_object( const underlying_type v = 0 ) : value( static_cast< T >( v ) ) {  }
      
      auto underlying_value() const { return _underlying_value( value ); }
      
      decltype(auto) operator+( const T f ){ value = static_cast< T >( _underlying_value( value ) |  _underlying_value( f ) ); return *this; }
      decltype(auto) operator-( const T f ){ value = static_cast< T >( _underlying_value( value ) & ~_underlying_value( f ) ); return *this; }
      
      auto reset() { return value = static_cast< T >(  0 ); }
      auto all()   { return value = static_cast< T >( -1 ); }
      
      auto to_string() const { std::stringstream s; s << static_cast< std::bitset< size * bits_of_byte > >( underlying_value() ); return s.str(); }
    };
  }
}
