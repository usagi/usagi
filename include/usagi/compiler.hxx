#pragma once

namespace usagi
{
  namespace compiler
  {
    enum class type : unsigned
    { UNKNOWN
    , MSVC
    , CLANG
    , GCC
    , ICC
    };
    
    constexpr auto compile_time =
#if defined ( _MSC_VER )
      type::MSVC
#elif defined ( __clang__ )
      type::CLANG
#elif defined ( __GNUC__ )
      type::GCC
#elif defined ( __ICC )
      type::ICC
#else
      type::UNKOWN
#endif
    ;
  }
}
