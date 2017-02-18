/// @file
/// @description HTTP features
/// @note
/// 
/// このライブラリーを使用するためには以下の依存ライブラリーのヘッダー及びリンクが必要です。
///   1. Boost.Asio: libboost_system
///   2. OpenSSL または LibreSSL: libcrypto, libssl, libtls
/// 
/// DISABLE_USAGI_HTTP_IMPLEMENTATION マクロが定義されている場合は実装を取り込まない。
///   ユースケース 1: 個別にコンパイル後にリンクする複数のソースから使用したい場合
///   ユースケース 2: binary library を生成して用いたい場合
///   定義しない    : 単一のソースからのみ使用し、ヘッダーオンリーライブラリーとして単純に用いたい場合
/// 
/// DISABLE_USAGI_HTTP_JSON マクロが定義されている場合は usagi::json を用いる syntax sugar は定義されない。

#include "http/get.hxx"
#include "http/post.hxx"
#include "http/put.hxx"
#include "http/delete.hxx"
#include "http/head.hxx"
//#include "http/server.hxx"
