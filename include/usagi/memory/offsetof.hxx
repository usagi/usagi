/// @file
/// @brief POD型のエレメントのアドレスオフセットを得る OFFSETOF マクロを定義する

#pragma once

#include <cstdint>

#ifndef OFFSETOF
  /// @def OFFSETOF( TYPE, ELEMENT )
  /// @brief POD型のELEMENT要素のアドレスオフセットを std::size_t 型で取得
  /// @param TYPE 要素のアドレスオフセットを得るPOD型
  /// @param ELEMENT アドレスオフセットを得るTYPE型の要素
  /// @return アドレスオフセット
  #define OFFSETOF( TYPE, ELEMENT ) reinterpret_cast< std::size_t >( &reinterpret_cast< TYPE * >( 0 ) -> ELEMENT )
#endif
