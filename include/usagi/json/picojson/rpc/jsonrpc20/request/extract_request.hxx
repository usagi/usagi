#pragma once

#include "../../../type.hxx"
#include "../../../make_object.hxx"

#include "../error_code_type.hxx"
#include "../constant.hxx"
#include "../is_jsonrpc20.hxx"

#include "get_method.hxx"
#include "get_params.hxx"

#include <tuple>

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  /// @return tie( method:string_type, params:value_type )
  static inline auto extract_request
  ( const value_type& request
  )
  {
    const auto& o = request.get< object_type >();
    
    if ( not is_jsonrpc20( o ) )
      throw exception_type( error_code_type::invalid_request );
    
    return std::make_tuple
    ( get_method( o )
    , get_params( o )
    );
  }
  
}