/// @file
/// @brief time zone difference

#pragma once

#include <chrono>
#include <ctime>

namespace usagi
{
  namespace chrono
  {
    template < typename T = std::chrono::minutes >
    static auto time_zone_difference()
    {
      const auto current_time = time ( 0 );
      auto local_tm = *gmtime ( &current_time );
      const auto utc_time = mktime ( &local_tm );
      return std::chrono::duration_cast< T >
        ( std::chrono::duration< double > ( difftime ( current_time, utc_time ) )
        );
    }
  }
}
