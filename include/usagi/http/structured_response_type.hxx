#pragma once

#include "macro.hxx"

#include <boost/optional.hpp>

#include <string>
#include <unordered_map>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace usagi::http
{
  using namespace std;
  using boost::optional;
  
  struct structured_response_type
  {
    uint16_t                        code;
    string                          code_string;
    unordered_map< string, string > headers;
    string                          body;
    
    auto is_informational () const noexcept -> bool;
    auto is_success       () const noexcept -> bool;
    auto is_redirection   () const noexcept -> bool;
    auto is_client_error  () const noexcept -> bool;
    auto is_server_error  () const noexcept -> bool;
    
    auto at( const string& header_key ) const noexcept -> const string&;
    auto at_optional( const string& header_key ) const noexcept -> optional< string >;
    
    auto get_location_if_redirection() const noexcept -> optional< string >;
    
    decltype( auto ) operator*() const noexcept;
    
    operator bool() const noexcept;
  };
  
  USAGI_HTTP_STATIC inline
  decltype( auto ) operator<<( ostream& out, const structured_response_type& in );
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "structured_response_type.cxx"
#endif