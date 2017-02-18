#pragma once

#include "macro.hxx"

/// @brief LibreSSL と Boost.ASIO.SSL の OpenSSL-1.1 API 対応判定の偽装処理
/// @ref 
/// @{
#if LIBRESSL_VERSION_NUMBER <= 0x2050100fL
  #include <openssl/opensslv.h>
  #undef OPENSSL_VERSION_NUMBER
  #define OPENSSL_VERSION_NUMBER 0x10001fffL
#endif
/// @}

#include <boost/asio/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

namespace usagi::http
{
  using namespace std;
  using namespace boost::asio;
}

#ifndef DISABLE_USGI_HTTP_IMPLEMENTATION
  #include "io_service.cxx"
#endif