#pragma once

namespace usagi
{
  namespace environment
  {
    enum class type: unsigned
    { UNKNOWN
    , APPLE_IPHONE
    , APPLE_OSX
    , WINDOWS
    , LINUX
    , UNIX
    };
    
    constexpr auto compile_time =
#if defined ( __APPLE__ )
  #if defined ( TARGET_OS_IPHONE )
      type::APPLE_IPHONE
  #elif TARGET_OS_MAC
      type::APPLE_OSX
  #endif
#elif defined ( _WIN32_ )
      type::WINDOWS
#elif defined ( __linux__ )
      type::LINUX
#elif defined ( __unix__ )
      type::UNIX
#else
      type::UNKNOWN
#endif
    ;
  }
}
