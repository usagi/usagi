/// @file
/// @brief windows

#pragma once

#ifndef USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_WINDOWS
#ifdef _WIN32
  #if !defined(NOMINMAX) && !defined(USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_NOMINMAX)
    #define NOMINMAX
  #endif
  #if !defined(WIN32_LEAN_AND_MEAN) && !defined(USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  #if defined(WIN32_LEAN_AND_MEAN) && defined(USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_WIN32_LEAN_AND_MEAN)
    #undef WIN32_LEAN_AND_MEAN
  #endif
  #if defined(NOMINMAX) && defined(USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_NOMINMAX)
    #define NOMINMAX
  #endif
#endif
#endif

