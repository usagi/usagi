#pragma once

#include "constant.hxx"
#include "../../type.hxx"

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  /// @brief id を抽出する
  /// @return 抽出できた場合には id 、抽出できなかった場合には null
  static inline decltype( auto )
  get_id( const object_type& in )
    noexcept -> value_type
  {
    try
    { return in.at( key_id ); }
    catch ( ... )
    { return { }; }
  }
  
  static inline decltype( auto )
  get_id( const value_type& in )
    noexcept -> value_type
  {
    try
    { return get_id( in.get< object_type >() ); }
    catch ( ... )
    { return { }; }
  }
  
}