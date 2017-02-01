#pragma once

#include "../../../type.hxx"
#include "../../../make_object.hxx"

#include "../constant.hxx"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  static inline auto make_request
  ( const std::string& method
  , const value_type& params = value_type()
  , const value_type& id = value_type()
  ) -> value_type
  {
    return
    value_type
    ( make_object
      ( key_jsonrpc , value_jsonrpc
      , key_method  , method
      , key_params  , params
      , key_id      , id
      )
    );
  }
  
  static inline auto make_request_with_uuidv4
  ( const std::string& method
  , const value_type& params = value_type()
  )
  {
    return make_request
      ( method
      , params
      , value_type( boost::uuids::to_string( boost::uuids::random_generator()() ) )
      );
  }
}