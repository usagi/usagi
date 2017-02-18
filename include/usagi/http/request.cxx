#pragma once

#include "macro.hxx"
#include "io_service.hxx"
#include "constant.hxx"

#ifndef DISABLE_USAGI_HTTP_JSON
  #include <usagi/json/picojson/type.hxx>
#endif

#include <stdexcept>

#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>


namespace usagi::http
{
  auto request
  ( const string&  url
  , const string&  method
  , const void*    data
  , const size_t   data_size
  , const string&  extra_header
  ) -> string
  {
    using namespace boost::xpressive;
    
    ip::tcp::resolver resolver( io_service );
    
    mark_tag scheme(1), host(2), port(3), tail(4);
    
    sregex r
      =  bos
      >> ( scheme = *_ )
      >> as_xpr( ':' ) >> +as_xpr( '/' )
      >> ( host = +( boost::xpressive::set[ alnum | '.' | '-' ] ) )
      >> ! ( as_xpr( ':' ) >> ( port = +alnum ) )
      >> ( tail = *_ )
      >> eos
      ;
    
    smatch m;
    
    if ( not regex_search( url, m, r ) )
      throw runtime_error( "cannot split the url: " + url );
    
    const auto& url_scheme  = m[ scheme ];
    const auto& url_host    = m[ host   ];
    
    auto url_port    = string( m[ port ] );
    if ( url_port.empty() )
      if ( url_scheme == "http" )
        url_port = "80";
      else if ( url_scheme == "https" )
        url_port = "443";
      else
        throw runtime_error( "cannot set default port with unknown scheme:" + url_scheme );
    
    auto url_tail = string( m[ tail ] );
    if ( url_tail.empty() )
      url_tail = "/";
    
    ip::tcp::resolver::query
      query
      ( url_host
      , url_port
      );
    
    ip::tcp::endpoint endpoint( *resolver.resolve( query ) );
    
    const auto request_writer =
      [&]
      ( auto& target_stream )
      {
        boost::asio::streambuf buffer;
        ostream out( &buffer );
        
        out
          << method << ' ' << url_tail << " HTTP/1.0" << http_end_of_line
          << "Host: " << url_host << http_end_of_line
          << "Connection: Close" << http_end_of_line
          << extra_header
          ;
        
        if ( data and data_size )
        {
          out << "Content-Length: " << data_size << http_separator;
          out.write( reinterpret_cast< const char*>( data ), data_size );
        }
        else
          out << http_end_of_line;
        
        write( target_stream, buffer );
      };
    
    const auto response_reader =
      [&]
      ( auto& target_stream )
      {
        stringstream result;
        
        boost::asio::streambuf buffer;
        boost::system::error_code e;
        
        while( read( target_stream, buffer, transfer_all(), e ) )
          result << string( buffer_cast< const char* >( buffer.data() ), buffer.size() );
        
        if ( e and e != error::eof )
        {
          switch ( e.value() )
          {
            case 335544539:
              // SSL SHORT READ は無視する
              break;
            default:
              throw runtime_error( to_string( e.value() ) + " : " + e.message() );
          }
        }
        
        return result.str();
      };
    
    if ( url_scheme == "https" )
    {
      ssl::context ssl_context( ssl::context::tlsv1_client );
      ssl::stream< ip::tcp::socket > ssl_stream( io_service, ssl_context );
      
      ssl_stream.lowest_layer().connect( endpoint );
      ssl_stream.handshake( ssl::stream_base::client );
      
      request_writer( ssl_stream );
      return response_reader( ssl_stream );
    }
    else
    {
      ip::tcp::socket socket( io_service );
      
      socket.connect( endpoint );
      
      request_writer( socket );
      return response_reader( socket );
    }
  }
  
  auto request
  ( const string&  url
  , const string&  method
  ) -> string
  { return request( url, method, nullptr, 0, "" ); }
  
#ifndef DISABLE_USAGI_HTTP_JSON
  auto request
  ( const string&  url
  , const string&  method
  , const ::usagi::json::picojson::value_type& data
  , const string&  extra_header
  ) -> string
  { return request( url, method, data.serialize(), "Content-Type: application/json\r\n" + extra_header ); }
#endif
  
  auto request_optional
  ( const string&  url
  , const string&  method
  , const void*    data         = nullptr
  , const size_t   data_size    = 0
  , const string&  extra_header
  ) -> optional< string >
  { try { return request( url, method, data, data_size, extra_header ); } catch ( ... ) { return { }; } }
  
  auto request_optional
  ( const string&  url
  , const string&  method
  , const string&  data
  , const string&  extra_header
  ) -> optional< string >
  { try { return request( url, method, data, extra_header ); } catch ( ... ) { return { }; } }
  
#ifndef DISABLE_USAGI_HTTP_JSON
  auto request_optional
  ( const string&  url
  , const string&  method
  , const ::usagi::json::picojson::value_type& data
  , const string&  extra_header
  ) -> optional< string >
  { try { return request( url, method, data, extra_header ); } catch ( ... ) { return { }; } }
#endif
}
