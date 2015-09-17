/// @file
/// @brief time_point to time_t converter

#pragma once

#include <chrono>

namespace usagi
{
  namespace chrono
  {
    template < typename T >
    inline static auto to_time_t( const T& t )
    {
      using std::chrono::system_clock;
      using std::chrono::duration_cast;
      const auto& dt = t - T::clock::now();
      const auto& et = system_clock::now() + duration_cast< system_clock::duration >( dt );
      return std::time_t( system_clock::to_time_t( et ) );
    }
  }
}
