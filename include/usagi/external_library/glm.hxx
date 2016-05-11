#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif
