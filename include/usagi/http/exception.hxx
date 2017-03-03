#pragma once

#include <stdexcept>
#include <string>

namespace usagi::http
{
  struct http_error
    : std::runtime_error
  {
    static constexpr unsigned redirection_count_over_value          = 996u;
    static constexpr unsigned response_has_no_location_header_value = 997u;
    static constexpr unsigned response_has_no_status_code_value     = 998u;
    static constexpr unsigned response_has_no_separator_value       = 999u;
    
    const unsigned status = 0u;
    
    explicit http_error( const unsigned http_status_code )
      : std::runtime_error( "http error status: " + std::to_string( http_status_code ) )
      , status( http_status_code )
    { }
    
    auto client_error()               const { return status >= 400u and status < 500u; }
    auto bad_request()                const { return status == 400u; }
    auto unauthorized()               const { return status == 401u; }
    auto forbidden()                  const { return status == 403u; }
    auto not_found()                  const { return status == 404u; }
    auto request_timeout()            const { return status == 408u; }
    
    auto server_error()               const { return status >= 500u and status < 600u; }
    auto internal_server_error()      const { return status == 500u; }
    auto not_implemented()            const { return status == 501u; }
    auto bad_gateway()                const { return status == 502u; }
    auto service_unavailable()        const { return status == 503u; }
    auto gateway_timeout()            const { return status == 504u; }
    auto http_version_not_supported() const { return status == 505u; }
    
    /// @brief 独自拡張コード: レスポンスが壊れていてヘッダーとボディーのスプリッターが無い場合に発行
    auto response_has_no_separator() const 
    { return status == response_has_no_separator_value; }
    
    /// @brief 独自拡張コード: レスポンスが壊れていてステータスコードが無い場合に発行
    auto response_has_no_status_code() const 
    { return status == response_has_no_status_code_value; }
    
    /// @brief 独自拡張コード: レスポンスが壊れていてリダイレクションにLocationヘッダーが無い場合に発行
    auto response_has_no_location_header() const 
    { return status == response_has_no_location_header_value; }
    
    /// @brief 独自拡張コード: リダイレクション回数オーバーした場合に発行
    auto redirection_count_over() const 
    { return status == redirection_count_over_value; }
    
    /// @brief リトライによって問題が発生しない可能性が高いエラーかどうか
    auto could_retry() const 
    {
      return request_timeout()
        or internal_server_error()
        or service_unavailable()
        or gateway_timeout()
        ;
    }
  };
}