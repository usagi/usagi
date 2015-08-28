/// @file
/// @brief time_point to iso8601 string converter

#include "time_zone_difference.hxx"
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace usagi
{
  namespace chrono
  {
    using default_clock = std::chrono::system_clock;

#ifdef _WIN32
    // TDM-GCC-5.1.0 is not support %F and %T
    //   note: mingw is supported. but we cannot predicate TDM or not.
    constexpr auto format_date_time = "%Y-%m-%dT%H:%M:%S";
#else
    constexpr auto format_date_time = "%FT%T";
#endif
    constexpr auto format_time_zone = "%H:%M";
    
    /// @brief time_point to iso8601 string
    /// @tparam T clock type
    /// @param t time_point
    /// @return iso8601 string
    template < typename TIME_POINT = default_clock::time_point >
    auto to_string_iso8601_gmt ( const TIME_POINT& t = TIME_POINT::clock::now() )
    -> std::string
    {
      using namespace std::chrono;
      
      const time_t ct = duration_cast< seconds >( t.time_since_epoch() ).count();
      
      std::string r = "0000-00-00T00:00:00.Z";
      std::strftime ( const_cast< char* > ( r.data( ) ), r.size( ), format_date_time, std::gmtime ( &ct ) );
      
      return r;
    }
    
    template < typename TIME_POINT = default_clock::time_point >
    auto to_string_iso8601_jst ( const TIME_POINT& t = TIME_POINT::clock::now() )
    {
      using namespace std::chrono;
      
      const time_t ct = duration_cast< seconds >( t.time_since_epoch() ).count();
      
      std::string r = "0000-00-00T00:00:00.";
      std::strftime ( const_cast< char* > ( r.data( ) ), r.size( ), format_date_time, std::localtime ( &ct ) );
      
      const auto z = time_zone_difference();
      
      std::stringstream rz;
      rz
        << ( std::signbit( z.count() ) ? "-" : "+" )
        << std::setw( 2 ) << std::setfill( '0' )
        << std::to_string( std::abs( duration_cast<hours>(z).count() ) )
        << ":"
        << std::setw( 2 ) << std::setfill( '0' )
        << std::to_string( std::abs( duration_cast<minutes>(z).count() ) % 60 )
        ;
      
      return r + rz.str();
    }
    
    template < typename TIME_POINT = default_clock::time_point >
    [[deprecated("to use: to_string_iso8601_gmt")]] auto to_string_iso8601 ( const TIME_POINT& t = TIME_POINT::clock::now() )
    { return to_string_iso8601_gmt< TIME_POINT >( t ); }
  }
}
