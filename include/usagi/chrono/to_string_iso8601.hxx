/// @file
/// @brief time_point to iso8601 string converter

#pragma once

#include "default_clock.hxx"
#ifdef _WIN32
#include "time_zone_difference.hxx"
#endif
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>

namespace usagi
{
  namespace chrono
  {
#ifdef _WIN32
    // TDM-GCC-5.1.0 is not support %F and %T
    //   note: mingw is supported. but we cannot predicate TDM or not.
    constexpr auto format_date_time = "%Y-%m-%dT%H:%M:%S";
#else
    constexpr auto format_date_time = "%FT%T";
#endif
    constexpr auto format_time_zone = "%z";
    
    namespace detail
    {
      template
      < typename T = usagi::chrono::default_clock::time_point
      , typename U = std::chrono::seconds
      >
      auto get_sub_seccond_string( const T& time_point )
        -> std::string
      {
        constexpr auto ratio_in_real = static_cast< long double >( U::period::num ) / U::period::den;
        constexpr auto sub_second_digits = -std::log10( ratio_in_real );
        
        if ( sub_second_digits < 1 )
          return "";
        
        const auto full = std::chrono::duration_cast< std::chrono::duration< long double > >( time_point.time_since_epoch() );
        const auto sec  = std::chrono::duration_cast< std::chrono::seconds >( time_point.time_since_epoch() );
        
        const auto sub_second_in_real = ( full - sec ).count();
        
        std::stringstream s;
        s << std::fixed << std::setprecision( sub_second_digits ) << sub_second_in_real;
        constexpr auto in_dot = 1;
        return s.str().substr( in_dot );
      }
    }
    
    /// @brief time_point to iso8601 string
    /// @tparam T clock type
    /// @param t time_point
    /// @return iso8601 string
    template
    < typename TIME_POINT = default_clock::time_point
    , typename SECOND_UNIT = std::chrono::seconds
    >
    auto to_string_iso8601_gmt
    ( const TIME_POINT& t = TIME_POINT::clock::now()
    )
    -> std::string
    {
      using namespace std::chrono;
      const auto& ct = TIME_POINT::clock::to_time_t ( t );
      const auto gt = std::gmtime( &ct );
      std::stringstream r;
      r << std::put_time( gt, format_date_time )
        << detail::get_sub_seccond_string< TIME_POINT, SECOND_UNIT >( t )
        << "Z"
        ;
      return r.str();
    }
    
    template
    < typename TIME_POINT = default_clock::time_point
    , typename SECOND_UNIT = std::chrono::seconds
    >
    auto to_string_iso8601_jst
    ( const TIME_POINT& t = TIME_POINT::clock::now()
    )
    {
      using namespace std::chrono;
      const auto& ct = TIME_POINT::clock::to_time_t ( t );
      auto lt = *std::localtime( &ct );
#ifdef _WIN32
      // MSVC++ ( and mingw ) put an invalid `%z` time zone string, then
      // write convertible code and it requred a time zone difference.
      const auto z = time_zone_difference();
#endif
      std::stringstream r;
      r << std::put_time( &lt, format_date_time )
        << detail::get_sub_seccond_string< TIME_POINT, SECOND_UNIT >( t )
#ifdef _WIN32
        << ( std::signbit( z.count() ) ? "-" : "+" )
        << std::setw( 2 ) << std::setfill( '0' )
        << std::to_string( std::abs( duration_cast< hours >( z ).count() ) )
        << ":"
        << std::setw( 2 ) << std::setfill( '0' )
        << std::to_string( std::abs( duration_cast< minutes >( z ).count() ) % minutes::period::den )
#else
        << std::put_time( &lt, format_time_zone )
#endif
        ;
      
      return r.str();
    }
    
    template
    < typename TIME_POINT = default_clock::time_point
    , typename SECOND_UNIT = std::chrono::seconds
    >
    [[deprecated ( "to use: to_string_iso8601_gmt" )]]
    auto to_string_iso8601
    ( const TIME_POINT& t = TIME_POINT::clock::now()
    )
    { return to_string_iso8601_gmt< TIME_POINT, SECOND_UNIT > ( t ); }
  }
}
