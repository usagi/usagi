#pragma once

#include "io_service.hxx"
#include "path_to_handler_mapper_type.hxx"

#include <atomic>

namespace usagi::http
{
  struct server_type;
  
  using unique_server_type = std::unique_ptr< server_type >;
  
  struct server_type
  {
    const std::uint16_t port;
    const bool          https;
    
    /// @brief localhost からのアクセス制限を行う場合は true のまま使用する
    std::atomic< bool > only_localhost;
    
    using handler_type = ::usagi::http::handler_type;
    
    auto add_handler
    ( const std::string& path
    , const handler_type& handler
    ) -> void;
    
    auto add_handler
    ( const char* path
    , const handler_type& handler
    )
    { add_handler( std::string( path ), handler ); }
    
    static auto make_unique
    ( const std::uint16_t port
    , const std::string& tls_certificate_chain_file
    , const std::string& tls_private_key_file
    , const std::string& tls_dhparam_file
    , const std::string& ip_address_string = ""
    ) -> unique_server_type;
    
    static auto make_unique
    ( const std::uint16_t port
    , const std::string& ip_address_string = ""
    ) -> unique_server_type;
    
    ~server_type();
    
    /// @brief HTTPS 版 ctor
    /// @note ip_address_string が空文字の場合は任意の ipv4 アドレスを使う
    server_type
    ( const std::uint16_t port_
    , const std::string& tls_certificate_chain_file
    , const std::string& tls_private_key_file
    , const std::string& tls_dhparam_file
    , const std::string& ip_address_string = ""
    );
    
    /// @brief HTTP 版 ctor
    server_type
    ( const std::uint16_t port_
    , const std::string& ip_address_string = ""
    );
    
    /// @brief セッションの待ち受けを開始
    auto operator()() -> void;
    
  private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor;
    std::unique_ptr< boost::asio::ssl::context > unique_ssl_context;
    shared_path_to_handler_mapper_type shared_path_to_handler_mapper;
    
    auto accept() -> void;
  };
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "server_type.cxx"
#endif
