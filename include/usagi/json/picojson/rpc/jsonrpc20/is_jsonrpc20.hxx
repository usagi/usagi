#pragma once

#include "constant.hxx"

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  static inline auto is_jsonrpc20
  ( const object_type& request
  ) noexcept -> bool
  {
    try
    { return request.at( key_jsonrpc ).get< std::string >() == value_jsonrpc; }
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