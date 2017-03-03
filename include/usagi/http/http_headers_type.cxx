#include "http_headers_type.hxx"

#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

namespace usagi::http
{
  
  auto http_headers_type::content_length() -> std::size_t
  {
    const auto request = headers.find( "content-length" );
    
    if ( request == headers.end() )
      return 0;
    
    std::stringstream s( request->second );
    std::size_t value;
    s >> value;
    
    return value;
  }
  
  auto http_headers_type::on_read_header( const std::string& line ) -> void
  {
    std::stringstream s( line );
    
    std::string key;
    std::getline( s, key, ':' );
    
    std::transform( key.begin(), key.end(), key.begin(), ::tolower );
    
    std::string value;
    std::getline( s, value );
    
    headers.emplace( std::move( key ), std::move( value ) );
  }
  
  auto http_headers_type::on_read_request_line( const std::string& line ) -> void
  {
    std::stringstream s( line );
    s >> method;
    s >> path;
    s >> version;
  }
  
}