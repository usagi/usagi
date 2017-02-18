#include "structured_response_type.hxx"

namespace usagi::http
{
  auto structured_response_type::is_informational() const noexcept
    -> bool
  { return code >= 100 and code < 200; }
  
  auto structured_response_type::is_success() const noexcept
    -> bool
  { return code >= 200 and code < 300; }
  
  auto structured_response_type::is_redirection() const noexcept
    -> bool
  { return code >= 300 and code < 400; }
  
  auto structured_response_type::is_client_error() const noexcept
    -> bool
  { return code >= 400 and code < 500; }
  
  auto structured_response_type::is_server_error() const noexcept
    -> bool
  { return code >= 500 and code < 600; }
  
  auto structured_response_type::at( const string& header_key ) const noexcept
    -> const string&
  { return headers.at( header_key ); }
  
  auto structured_response_type::at_optional( const string& header_key )
    const noexcept
    -> optional< string >
  { try { return headers.at( header_key ); } catch ( ... ) { return { }; } }
  
  auto structured_response_type::get_location_if_redirection() const noexcept
    -> optional< string >
  {
    if ( is_redirection() )
      return at_optional( "location" );
    return { };
  }
  
  decltype( auto ) structured_response_type::operator*() const noexcept
  { return body; }
  
  structured_response_type::operator bool() const noexcept
  { return is_success(); }
  
  decltype( auto ) operator<<( ostream& out, const structured_response_type& in )
  { return out << *in; }
}
