#pragma once

#include <picojson.h>
#include <string>

namespace usagi::json::picojson
{
  using object_type = ::picojson::object;
  using array_type  = ::picojson::array;
  using value_type  = ::picojson::value;
  using null_type   = ::picojson::null;
  using number_type = double;
  using string_type = std::string;
}
