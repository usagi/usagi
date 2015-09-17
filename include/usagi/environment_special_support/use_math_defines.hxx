/// @file
/// @brief _USE_MATH_DEFINES
/// @note _WIN32の場合に _USE_MATH_DEFINES されていなければ <cmath> が定数を定義しない問題へ対応

#pragma once

#ifndef USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_USE_MATH_DEFINES
#ifdef _WIN32
  #define _USE_MATH_DEFINES
#endif
#endif

