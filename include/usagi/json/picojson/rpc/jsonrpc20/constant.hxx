#pragma once

namespace usagi::json::picojson::rpc::jsonrpc20
{
  static constexpr auto key_jsonrpc   = "jsonrpc";
  static constexpr auto value_jsonrpc = "2.0";
  static constexpr auto key_method    = "method";
  static constexpr auto key_params    = "params";
  static constexpr auto key_id        = "id";
  static constexpr auto key_error     = "error";
  static constexpr auto key_code      = "code";
  static constexpr auto key_message   = "message";
  static constexpr auto key_data      = "data";
}