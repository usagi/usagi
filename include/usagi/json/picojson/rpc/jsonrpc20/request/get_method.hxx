#pragma once

#include "../../../type.hxx"

#include "../error_code_type.hxx"
#include "../constant.hxx"

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  static inline decltype( auto ) get_method
  ( const object_type& request
  )
  {
    try
    {
      return
        request
        .at( key_method )
        .get< std::string >();
    }
    catch ( const std::out_of_range& e )
    { throw exception_type( error_code_type::method_not_found, e.what() ); }
    catch ( const std::runtime_error& e )
    { throw exception_type( error_code_type::invalid_request, e.what() ); }
    catch ( ... )
    { throw exception_type( error_code_type::invalid_request ); }
  }
  
  static inline decltype( auto ) get_method
  ( const value_type& request
  )
  {
    try
    { return get_method( request.get< object_type >() ); }
    catch ( const std::runtime_error& e )
    { throw exception_type( error_code_type::invalid_request, e.what() ); }
    catch ( ... )
    { throw exception_type( error_code_type::invalid_request ); }
  }
  
}