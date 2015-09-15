/// @file
/// @brief operator[](0)により先頭要素を取得可能なある型の連続領域により構成される型に含まれるある型の要素数を取得

#pragma once

#include <cstdint>

#ifndef OFFSETOF
  /// @def ARRAY_ELEMENTSOF( TYPE )
  /// @brief operator[](0)により先頭要素を取得可能なある型の連続領域により構成される型に含まれるある型の要素数を取得
  /// @param TYPE 要素型の連続領域からなるoperator[](0)により先頭要素を取得可能な型
  /// @return 要素数
  #define ARRAY_ELEMENTSOF( TYPE ) ( sizeof( TYPE ) / sizeof( TYPE[0] ) )
#endif
