#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

namespace usagi::http
{
  struct http_headers_type
  {
    std::string method;
    std::string path;
    std::string version;
    
    std::unordered_map< std::string, std::string > headers;
    
    auto content_length() -> std::size_t;
    auto on_read_header( const std::string& line ) -> void;
    auto on_read_request_line( const std::string& line ) -> void;
  };
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "http_headers_type.cxx"
#endif