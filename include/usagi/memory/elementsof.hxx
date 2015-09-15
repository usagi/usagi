/// @file
/// @brief ある型の連続領域により構成される型に含まれるある型の要素数を取得

#pragma once

#include <cstdint>

#ifndef OFFSETOF
  /// @def ELEMENTSOF( TYPE, ELEMENT )
  /// @brief ある型の連続領域により構成される型に含まれるある型の要素数を取得
  /// @param TYPE ある型の連続領域により構成される型
  /// @param ELEMENT_TYPE TYPEを構成する要素型
  /// @return 要素数
  #define ELEMENTSOF( TYPE, ELEMENT_TYPE ) ( sizeof( TYPE ) / sizeof( ELEMENT_TYPE ) )
#endif
