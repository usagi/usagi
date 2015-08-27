/// @file
/// @brief iso8601 string to time_point converter

#include "time_zone_difference.hxx"
#include <chrono>
#include <iomanip>
#include <string>
#include <stdexcept>

namespace usagi
{
  namespace chrono
  {
    template < typename T = std::chrono::system_clock >
    static auto from_iso8601( const std::string& iso8601_string )
    {
      bool is_jst = false;
      bool is_negative_jst = false;
      std::size_t sign_position = iso8601_string.find('Z');

      if ( sign_position == std::string::npos )
      {
        sign_position = iso8601_string.find('+');
        
        if ( sign_position == std::string::npos )
        {
          sign_position = iso8601_string.find('-');
          is_negative_jst = true;
        }
        
        if ( sign_position == std::string::npos or sign_position + sizeof("00:00") != iso8601_string.size() )
          throw std::runtime_error( "cannot parse iso8601 time zone" );
        
        is_jst = true;
      }
      
      std::istringstream s( std::string( iso8601_string.cbegin(), iso8601_string.cbegin() + sign_position ) );
      
      std::tm t;
      constexpr auto format_string = "%Y-%m-%dT%H:%M:%S";
      s >> std::get_time( &t, format_string );
      
      if ( s.fail() )
        throw std::runtime_error("cannot parse date-time part");
      
      const auto p = std::mktime( &t );
      auto r = T::from_time_t( p ) + time_zone_difference();
      
      if ( is_jst )
      {
        std::istringstream s_jst( std::string( iso8601_string.cbegin() + sign_position + 1, iso8601_string.cend() ) );
        
        std::tm t_jst;
        constexpr auto format_string_jst = "%H:%M";
        s_jst >> std::get_time( &t_jst, format_string_jst );
        
        if ( s.fail() )
          throw std::runtime_error("cannot parse JST part");
        
        r -= ( is_negative_jst ? -1 : 1 ) * std::chrono::hours( t_jst.tm_hour ) + std::chrono::minutes( t_jst.tm_min );
      }
      
      return r;
    }
  }
}
