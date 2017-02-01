#pragma once

#include "../../../type.hxx"
#include "../../../make_object.hxx"

#include "../error_code_type.hxx"
#include "../exception_type.hxx"
#include "../constant.hxx"
#include "../get_id.hxx"

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  static inline auto make_error
  ( const value_type& error = value_type()
  , const value_type& id    = value_type()
  ) -> value_type
  {
    return
      value_type
      ( make_object
        ( key_jsonrpc , value_jsonrpc
        , key_error   , error
        , key_id      , id
        )
      );
  }
  
  static inline auto make_error_from_request
  ( const value_type& request
  , const value_type& error   = value_type()
  ) -> value_type
  {
    return
      make_error
      ( error
      , get_id( request )
      );
  }
  
  static inline auto make_error
  ( const error_code_type&  error_code
  , const std::string&      error_message = ""
  , const value_type&       data          = value_type()
  , const value_type&       id            = value_type()
  ) -> value_type
  {
    return
      make_error
      ( value_type
        ( make_object
          ( key_code    , to_number( error_code )
          , key_message , error_message.empty()
                            ? to_string( error_code )
                            : error_message
          , key_data    , data
          )
        )
      , id
      );
  }
  
  static inline auto make_error_from_request
  ( const value_type& request
  , const error_code_type&  error_code
  , const std::string&      error_message = ""
  , const value_type&       data          = value_type()
  ) -> value_type
  {
    return
      make_error
      ( error_code
      , error_message
      , data
      , get_id( request )
      );
  }
  
  static inline auto make_error
  ( const exception_type& e
  , const value_type&     id = value_type()
  ) -> value_type
  {
    return
      make_error
      ( e.error_code
      , e.what()
      , value_type()
      , id
      );
  }
  
  static inline auto make_error_from_request
  ( const value_type& request
  , const exception_type& e
  ) -> value_type
  {
    return
      make_error
      ( e
      , get_id( request )
      );
  }
  
}