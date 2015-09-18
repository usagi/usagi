/// @file
/// @brief define default_clock

#pragma once

#include "windows_query_performance_counter_clock.hxx"
#include <chrono>

namespace usagi
{
  namespace chrono
  {
    using default_clock =
#ifdef _WIN32
      windows_query_performance_counter_clock
#else
      std::chrono::steady_clock
#endif
      ;
  }
}
