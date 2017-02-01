#pragma once

#include "error_code_type.hxx"
#include <stdexcept>

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  struct exception_type
    : public std::runtime_error
  {
    const error_code_type error_code;
    exception_type( const error_code_type e, const std::string& m = "" )
      : std::runtime_error( m.empty() ? to_string( e ) : to_string( e ) + " ( " + m + " ) " )
      , error_code( e )
    { }
  };
  
}