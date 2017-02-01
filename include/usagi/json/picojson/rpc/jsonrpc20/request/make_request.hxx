#pragma once

#include "../../type.hxx";
#include "../../make_object.hxx";

#include "../error.hxx";
#include "../constant.hxx";
#include "../get_id.hxx";

#include "get_method.hxx";
#include "get_params.hxx";

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
    return make_object
    ( key_jsonrpc , value_jsonrpc
    , key_method  , method
    , key_params  , params
    , key_id      , id
    );
  }
  
  static inline auto make_request_with_uuid
  ( const std::string& method
  , const value_type& params = value_type()
  )
  {
    return make_request
      ( method
      , params
      , boost::uuids::to_string( boost::uuids::random_generator()() )
      );
  }
}