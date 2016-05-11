#pragma once

#include "get.hxx"

namespace usagi
{
  namespace gl
  {
    
    static inline auto get_current_program()
      -> GLuint
    {
      return static_cast< GLuint >( get< GLint >( GL_CURRENT_PROGRAM ) );
    }
    
  }
}
