#pragma once

#include "http_headers_type.hxx"

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

namespace usagi::http
{
  using handler_header_key_type   = std::string;
  using handler_header_value_type = std::string;
  
  using handler_headers_type = 
    std::unordered_map
    < handler_header_key_type
    , handler_header_value_type
    >;
  
  using handler_body_type = std::string;
  
  using handler_return_type =
    std::pair
    < handler_headers_type
    , handler_body_type
    >;
  
  using handler_signature_type =
    auto
    ( const std::string& method
    , const std::string& path
    , const handler_headers_type& headers
    , const std::string& body
    ) -> handler_return_type
    ;
  
  using handler_type = std::function< handler_signature_type >;
  
  using path_to_handler_mapper_type =
    std::unordered_map
    < std::string
    , handler_type
    >
    ;
  
  using shared_path_to_handler_mapper_type =
    std::shared_ptr
    < path_to_handler_mapper_type
    >
    ;
}
