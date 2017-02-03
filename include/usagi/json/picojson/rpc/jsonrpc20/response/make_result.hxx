#pragma once


#include "../../../type.hxx"
#include "../../../make_object.hxx"

#include "../constant.hxx"
#include "../get_id.hxx"

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  static inline auto make_result
  ( const value_type& result  = value_type()
  , const value_type& id      = value_type()
  ) -> value_type
  {
    return
      make_object_value
      ( key_jsonrpc , value_jsonrpc
      , key_result  , result
      , key_id      , id
      );
  }
  
  static inline auto make_result_from_request
  ( const value_type& request
  , const value_type& result  = value_type()
  ) -> value_type
  {
    return
      make_result
      ( result
      , get_id( request )
      );
  }
  
}