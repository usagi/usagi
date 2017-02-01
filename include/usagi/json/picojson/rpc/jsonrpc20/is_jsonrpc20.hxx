#pragma once

#include "../../type.hxx";
#include "../../make_object.hxx";

#include "../error.hxx";
#include "../constant.hxx";
#include "../get_id.hxx";

#include "get_method.hxx";
#include "get_params.hxx";

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  static inline auto is_jsonrpc20
  ( const object_type& request
  ) noexcept -> bool
  {
    try
    { return o.at( key_jsonrpc ) = value_jsonrpc ); }
    catch ( ... )
    { return false; }
  }
  
  static inline auto is_jsonrpc20
  ( const value_type& request
  ) noexcept -> bool
  {
    try
    { return is_jsonrpc20( request.get< object_type >() ); }
    catch ( ... )
    { return false; }
  }
  
}