/// @file
/// @brief すぐ使える実用的な簡易ロガー
/// @description LOGI, LOGD, LOGW, LOGE の4つのマクロが定義され、それぞれをログ出力ストリームと見做して LOGI << "hoge"; のように使う。
/// LOGI ... info  レベルのログメッセージ出力（白色）
/// LOGD ... debug レベルのログメッセージ出力（緑色）
/// LOGW ... warn  レベルのログメッセージ出力（黄色）
/// LOGE ... error レベルのログメッセージ出力（赤色）
/// @note DISABLE_USAGI_LOG_EASY_LOGGER が定義されている場合、全てのログ出力は事実上無効になる。

#pragma once

#ifdef DISABLE_USAGI_LOG_EASY_LOGGER

#define LOGI ::usagi::log::easy_logger::log_null()
#define LOGD ::usagi::log::easy_logger::log_null()
#define LOGW ::usagi::log::easy_logger::log_null()
#define LOGE ::usagi::log::easy_logger::log_null()

#else

#define LOGI ::usagi::log::easy_logger::log_intermediate::make_info ( __FILE__, __LINE__, __PRETTY_FUNCTION__ ) << '\t'
#define LOGD ::usagi::log::easy_logger::log_intermediate::make_debug( __FILE__, __LINE__, __PRETTY_FUNCTION__ ) << '\t'
#define LOGW ::usagi::log::easy_logger::log_intermediate::make_warn ( __FILE__, __LINE__, __PRETTY_FUNCTION__ ) << '\t'
#define LOGE ::usagi::log::easy_logger::log_intermediate::make_error( __FILE__, __LINE__, __PRETTY_FUNCTION__ ) << '\t'

#endif

#include <boost/timer/timer.hpp>
#include <string>
#include <iostream>
#include <iomanip>

namespace usagi::log::easy_logger
{
  using namespace std;
  
  boost::timer::cpu_timer global_timer;
  
  struct log_null
  {
    template < typename T >
    decltype( auto ) operator<<( const T& ) { return *this; }
  };
  
  struct log_intermediate
  {
    stringstream buffer;
    
    const char* prefix;
    const char* suffix;
    
    const char* source;
    const size_t line;
    const char* function;
    
    static constexpr auto prefix_i = "";
    static constexpr auto prefix_d = "\x1b[32m";
    static constexpr auto prefix_w = "\x1b[33m";
    static constexpr auto prefix_e = "\x1b[31m";
    
    static constexpr auto suffix_i = "";
    static constexpr auto suffix_d = "\x1b[0m";
    static constexpr auto suffix_w = "\x1b[0m";
    static constexpr auto suffix_e = "\x1b[0m";
    
    static constexpr auto log_time_width = 13;
    
    log_intermediate( log_intermediate&& a )
      : buffer( move( a.buffer ) )
      , prefix( a.prefix )
      , suffix( a.suffix )
      , source( a.source )
      , line( a.line )
      , function( a.function )
    { }
    
    log_intermediate( const char* p, const char* s, const char* source_, const size_t line_, const char* function_ )
      : prefix( p )
      , suffix( s )
      , source( source_ )
      , line( line_ )
      , function( function_ )
    { }
    
    static auto make_info ( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_i, suffix_i, s, l, f ); }
    static auto make_debug( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_d, suffix_d, s, l, f ); }
    static auto make_warn ( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_w, suffix_w, s, l, f ); }
    static auto make_error( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_e, suffix_e, s, l, f ); }
    
    template < typename T >
    decltype( auto ) operator<<( const T& in ) noexcept
    {
      try
      { return buffer << in; }
      catch ( const exception& e )
      { return buffer << "<<<<<exception on " << __PRETTY_FUNCTION__ << " what=" << e.what() << ">>>>>"; }
      catch ( ... )
      { return buffer << "<<<<<exception on " << __PRETTY_FUNCTION__ << " uknown>>>>>"; }
    }
    
    ~log_intermediate() noexcept
    {
      try
      {
        cout
          << prefix
          << "[ " << setw( log_time_width ) << global_timer.elapsed().wall << " ]"
             "\t"
          << buffer.str()
          << "\t"
          << source << '\t' << line << '\t' << function
          << suffix
          << endl
          ;
      }
      catch ( const exception& e )
      { cerr << "\n\n<<<<<\nexception on " << __PRETTY_FUNCTION__ << "\nwhat=" << e.what() << "\n>>>>>\n\n"; }
      catch ( ... )
      { cerr << "\n\n<<<<<\nexception on " << __PRETTY_FUNCTION__ << "\nunknown\n>>>>>\n\n"; }
    }
  };
}
