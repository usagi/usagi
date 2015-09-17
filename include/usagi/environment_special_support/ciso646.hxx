/// @file
/// @brief ciso646
/// @note C++言語規格として本来<ciso646.h>のインクルード無しに使用可能だがMSVC++では明示的なインクルードが必要となる

#pragma once

#ifndef USAGI_ENVIRONMENT_SPECIAL_SUPPORT_NO_CISO646
#ifdef _MSC_VER
  #include <ciso646.h>
#endif
#endif
