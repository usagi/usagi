/// @file
/// @brief Windows 専用 QueryPerformanceCounter 版 std::chrono 互換 clock

#pragma once

#include "../environment_special_support.hxx"
#include "to_time_t.hxx"
#include <chrono>

#ifdef _WIN32
namespace usagi
{
  namespace chrono
  {
    template < typename REP = long double >
    struct windows_query_performance_counter_clock
    {
      using rep = REP;
      using period = std::pico;
      using duration = std::chrono::duration< rep, period >;
      using time_point = std::chrono::time_point< windows_query_performance_counter_clock< REP > >;
      static const bool is_steady = true;
      static time_point now()
      {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency ( &frequency );
        LARGE_INTEGER count;
        QueryPerformanceCounter ( &count );
        return time_point ( duration ( count.QuadPart * static_cast< rep > ( period::den ) /
                                       frequency.QuadPart ) );
      }
      static auto to_time_t( const time_point& tp ) noexcept -> std::time_t
      {
        return usagi::chrono::to_time_t( tp );
      }
    };
  }
}
#endif
