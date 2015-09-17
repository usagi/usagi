/// @file
/// @brief winscok

#pragma once

#ifndef USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_WINSOCK
#ifdef _WIN32
  #if !defined(NOMINMAX) && !defined(USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_NOMINMAX)
    #define NOMINMAX
  #endif
  #if !defined(WIN32_LEAN_AND_MEAN) && !defined(USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif
  // windows.h より先にインクルードする必要がある
  #include <WinSock2.h>
  // winsock2.h の内部でインクルードされるwindows.hへの環境特別対応
  #include "windows.hxx"
#endif
#endif
