#pragma once

#include "http_headers_type.hxx"
#include "path_to_handler_mapper_type.hxx"
#include "io_service.hxx"

#include <sstream>
#include <cstdint>
#include <memory>

namespace usagi::http
{
  class http_session_type
    : public std::enable_shared_from_this< http_session_type >
  {
    const bool only_localhost;
    
    boost::asio::streambuf buffer;
    http_headers_type headers;
    std::string body;
    
    shared_path_to_handler_mapper_type shared_path_to_handler_mapper;
    
    auto read_body() -> void;
    auto read_next_line() -> void;
    auto read_first_line() -> void;
    
    auto write_error( const std::uint16_t status_code ) -> void;
    auto write
    ( std::unordered_map< std::string, std::string >&& response_headers
    , std::string&& body
    ) -> void;
    
    auto execute_handler() -> void;
    
  public:
    
    using socket_type = boost::asio::ip::tcp::socket;
    
    socket_type socket;
    
    http_session_type
    ( boost::asio::io_service& io_service
    , const shared_path_to_handler_mapper_type& mapper
    , const bool _only_localhost
    );
    
    auto interact() -> void;
  };
  
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "http_session_type.cxx"
#endif