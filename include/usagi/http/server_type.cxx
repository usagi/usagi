#pragma once

#include "server_type.hxx"

#include "exception.hxx"
#include "http_session_type.hxx"
#include "https_session_type.hxx"
#include <iostream>

namespace usagi::http
{
  auto server_type::make_unique
  ( const std::uint16_t port_
  , const std::string& tls_certificate_chain_file
  , const std::string& tls_private_key_file
  , const std::string& tls_dhparam_file
  , const std::string& ip_address_string
  ) -> unique_server_type
  {
    return std::make_unique< server_type >
      ( port_
      , tls_certificate_chain_file
      , tls_private_key_file
      , tls_dhparam_file
      , ip_address_string
      );
  }
  
  auto server_type::make_unique
  ( const std::uint16_t port_
  , const std::string& ip_address_string
  ) -> unique_server_type
  {
    return std::make_unique< server_type >
      ( port_
      , ip_address_string
      );
  }
  
  server_type::~server_type()
  {
    io_service.stop();
    io_service.reset();
  }
  
  server_type::server_type
  ( const std::uint16_t port_
  , const std::string& tls_certificate_chain_file
  , const std::string& tls_private_key_file
  , const std::string& tls_dhparam_file
  , const std::string& ip_address_string
  )
    : port( port_ )
    , https( true )
    , only_localhost( true )
    , acceptor
      ( io_service
      , ip_address_string.empty()
          ? boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port_ )
          : boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string( ip_address_string ), port_ )
      )
    , unique_ssl_context( std::make_unique< boost::asio::ssl::context >( io_service, boost::asio::ssl::context::sslv23 ) )
    , shared_path_to_handler_mapper( std::make_shared< path_to_handler_mapper_type >() )
  {
    unique_ssl_context->set_options
    ( boost::asio::ssl::context::default_workarounds
    | boost::asio::ssl::context::no_sslv2
    | boost::asio::ssl::context::single_dh_use
    );
    unique_ssl_context->use_certificate_chain_file( tls_certificate_chain_file );
    unique_ssl_context->use_private_key_file( tls_private_key_file, boost::asio::ssl::context::pem );
    unique_ssl_context->use_tmp_dh_file( tls_dhparam_file );
  }
  
  server_type::server_type
  ( const std::uint16_t port_
  , const std::string& ip_address_string
  )
    : port( port_ )
    , https( false )
    , only_localhost( true )
    , acceptor
      ( io_service
      , ip_address_string.empty()
          ? boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port_ )
          : boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string( ip_address_string ), port_ )
      )
    , shared_path_to_handler_mapper( std::make_shared< path_to_handler_mapper_type >() )
  { }
  
  auto server_type::add_handler
  ( const std::string& path
  , const handler_type& handler
  ) -> void
  {
    shared_path_to_handler_mapper->emplace( path, handler );
  }
  
  auto server_type::accept() -> void
  {
    if ( https )
    {
      auto shared_session = 
        std::make_shared< https_session_type >
        ( io_service
        , *unique_ssl_context
        , shared_path_to_handler_mapper
        , only_localhost
        );
      
      acceptor.async_accept
      ( shared_session->socket.lowest_layer()
      , [ this
        , shared_session
        ]
        ( const boost::system::error_code& accept_error )
        {
          accept();
          
          if ( not accept_error )
            shared_session->interact();
        }
      );
    }
    else
    {
      auto shared_session = 
        std::make_shared< http_session_type >
        ( io_service
        , shared_path_to_handler_mapper
        , only_localhost
        );
      
      acceptor.async_accept
      ( shared_session->socket
      , [ this
        , shared_session
        ]
        ( const boost::system::error_code& accept_error )
        {
          accept();
          
          if ( not accept_error )
            shared_session->interact();
        }
      );
    }
  }
  
  auto server_type::operator()() -> void
  {
    acceptor.listen();
    accept();
    io_service.run();
  }
}
