#include "http_headers_type.hxx"
#include "https_session_type.hxx"
#include "exception.hxx"

#include <boost/asio.hpp>

#include <sstream>
#include <cstdint>
#include <memory>
#include <array>
#include <iostream>

namespace usagi::http
{
  auto https_session_type::execute_handler() -> void
  {
    const auto path_to_handler = shared_path_to_handler_mapper->find
    ( headers.path )
    ;
    
    if ( path_to_handler == shared_path_to_handler_mapper->end() )
    {
      constexpr auto http_status_code_not_found = static_cast< std::uint16_t >( 404 );
      write_error( http_status_code_not_found );
    }
    else
    {
      try
      {
        auto&& response_headers_and_body =
          path_to_handler->second
          ( headers.method
          , headers.path
          , headers.headers
          , body
          );
        
        write
        ( std::move( response_headers_and_body.first  )
        , std::move( response_headers_and_body.second )
        );
      }
      catch ( const http_error& e )
      {
        write_error( e.status );
      }
    }
  }
  
  auto https_session_type::write_error( const std::uint16_t status_code ) -> void
  {
    const auto first_line = "HTTP/1.1 " + std::to_string( status_code ) + " ERROR";
    const auto shared_string_buffer = std::make_shared< std::string >
    ( first_line + "\r\n"
      "content-type: text/plain\r\n"
      "content-length: " + std::to_string( first_line.size() ) + "\r\n"
      "\r\n"
    + first_line
    );
    
    boost::asio::async_write
    ( socket
    , boost::asio::buffer( shared_string_buffer->c_str(), shared_string_buffer->size() )
    , [ this_ = shared_from_this(), shared_string_buffer ]
      ( const boost::system::error_code&, std::size_t )
      {
        //if ( e )
        //  std::cerr << "error: " << e.message() << std::endl;
        //else
        //  std::cerr << "wrote. size = " << s << std::endl;
      }
    );
  }
  
  auto https_session_type::write
  ( std::unordered_map< std::string, std::string >&& response_headers
  , std::string&& response_body
  ) -> void
  {
    std::stringstream s;
    
    s << "HTTP/1.1 200 OK\r\n";
    
    response_headers[ "content-length" ] = std::to_string( response_body.size() );
    
    {
      bool has_content_type = false;
      
      for ( const auto& response_header_pair : response_headers )
      {
        s << response_header_pair.first << ": " << response_header_pair.second << "\r\n";
        has_content_type |= response_header_pair.first == "content-type";
      }
      
      // 予め count または find でチェックして入れておく事もできるが、どのみち for を回すので処理コストの都合この実装形態とする
      if ( not has_content_type )
        s << "content-type: application/octet-stream\r\n";
    }
    
    s << "\r\n";
    
    s << std::forward< std::string >( response_body );
    
    const auto shared_string_buffer = std::make_shared< std::string >( s.str() );
    
    boost::asio::async_write
    ( socket
    , boost::asio::buffer( shared_string_buffer->c_str(), shared_string_buffer->size() )
    , [ this_ = shared_from_this(), shared_string_buffer ]
      ( const boost::system::error_code&, std::size_t )
      { }
    );
  }
        
  auto https_session_type::read_body() -> void
  {
    const auto entity =
    [ this_ = shared_from_this() ]
    ( const boost::system::error_code& e, std::size_t )
    {
      if ( e and e != boost::asio::error::eof )
      {
        this_->write_error( 400 );
        return;
      }
      
      this_->body =
        std::string
        ( boost::asio::buffer_cast< const char* >( this_->buffer.data() )
        , this_->buffer.size()
        );
      
      this_->execute_handler();
    };
    
    if ( buffer.size() < headers.content_length() )
      boost::asio::async_read
      ( socket
      , buffer
      , boost::asio::transfer_all()
      , entity
      );
    else
      entity( { }, 0 );
  }
  
  auto https_session_type::read_next_line() -> void
  {
    boost::asio::async_read_until
    ( socket
    , buffer
    , '\r'
    , [ this_ = shared_from_this() ]
      ( const boost::system::error_code& e, std::size_t )
      {
        if ( e )
        {
          this_->write_error( 400 );
          return;
        }
        
        std::string line, ignore;
        std::istream s( &this_->buffer );
        s.exceptions( std::istream::failbit | std::istream::badbit );
        
        try
        {
          std::getline( s, line, '\r' );
          std::getline( s, ignore, '\n' );
        }
        catch ( const std::istream::failure& e )
        {
          this_->write_error( 400 );
          return;
        }
        
        this_->headers.on_read_header( line );
        
        // 最初の空行 ＝ header と body の境界なので、それまでは header を読み続ける
        if ( line.length() != 0 )
        {
          this_->read_next_line();
          return;
        }
        
        // 空行まで読んだのなら、続きは body なので content_length > 0 なら読む
        if ( this_->headers.content_length() > 0 )
        {
          this_->read_body();
          return;
        }
        
        // body は 0 byte なので読む必要が無いので response の書き出しへ
        this_->execute_handler();
      }
    );
  }
  
  auto https_session_type::read_first_line() -> void
  {
    boost::asio::async_read_until
    ( socket
    , buffer
    , '\r'
    , [ this_ = shared_from_this() ]
      ( const boost::system::error_code& e, std::size_t )
      {
        if ( e )
        {
          this_->write_error( 400 );
          return;
        }
        
        std::string line, ignore;
        std::istream s( &this_->buffer );
        s.exceptions( std::istream::failbit | std::istream::badbit );
        
        try
        {
          std::getline( s, line, '\r' );
          std::getline( s, ignore, '\n' );
        }
        catch ( const std::istream::failure& e )
        {
          this_->write_error( 400 );
          return;
        }
        
        this_->headers.on_read_request_line( line );
        this_->read_next_line();
      }
    );
  }
  
  auto https_session_type::handshake() -> void
  {
    socket.async_handshake
    ( boost::asio::ssl::stream_base::server
    , [ this_ = shared_from_this() ]
      ( const boost::system::error_code& e )
      {
        if ( e )
        {
          this_->write_error( 400 );
          return;
        }
        
        this_->read_first_line();
      }
    );
  }
  
  https_session_type::https_session_type
  ( boost::asio::io_service& io_service
  , boost::asio::ssl::context& ssl_context
  , const shared_path_to_handler_mapper_type& mapper
  , const bool _only_localhost
  )
    : only_localhost( _only_localhost )
    , shared_path_to_handler_mapper( mapper )
    , socket( io_service, ssl_context )
  { }
  
  auto https_session_type::interact() -> void
  {
    handshake();
  }

}