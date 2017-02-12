/// @file
/// @brief すぐ使える実用的な簡易ロガー
/// @description LOGI, LOGD, LOGW, LOGE の4つのマクロが定義され、それぞれをログ出力ストリームと見做して LOGI << "hoge"; のように使う。
/// LOGI ... info  レベルのログメッセージ出力（白色）
/// LOGD ... debug レベルのログメッセージ出力（緑色）
/// LOGW ... warn  レベルのログメッセージ出力（黄色）
/// LOGE ... error レベルのログメッセージ出力（赤色）
/// @note
/// (a): USE_USAGI_LOG_EASY_LOGGER_BOOST_CPU_TIMER が定義されている場合、時間計測に boost::timer::cpu_timer を使用する（ boost.timer 関連のライブラリーのリンクが必要 ）
/// (b): USE_USAGI_LOG_EASY_LOGGER_BOOST_CHRONO が定義されている場合、時間計測に boost::chrono::steady_clock を使用する ( boost.chrono 関連のライブラリーのリンクが必要)
/// (c): USE_USAGI_LOG_EASY_LOGGER_STD_CHRONO が定義されている場合、時間計測に std::chrono::steady_clock を使用する（ 外部ライブラリーのリンクは不要だが、処理系によっては分解能が不足する ）
/// (d): (a), (b), (c) の何れも定義されていない場合、時間計測に usagi::chrono::default_clock を使用する（ 外部ライブラリーは不要、windows処理系でもQueryPerformanceCounterを内部的に使用する ）
/// (f): DISABLE_USAGI_LOG_EASY_LOGGER が定義されている場合、全てのログ出力は事実上無効になる。

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

#ifdef USE_USAGI_LOG_EASY_LOGGER_BOOST_CPU_TIMER
  #include <boost/timer/timer.hpp>
#else
  #ifndef USAGI_LOG_EASY_LOGGER_CLOCK_TYPE
    #ifdef USE_USAGI_LOG_EASY_LOGGER_BOOST_CHRONO
      #include <boost/chrono.hpp>
      #define USAGI_LOG_EASY_LOGGER_CLOCK_TYPE boost::chrono::steady_clock
    #elif defined( USE_USAGI_LOG_EASY_LOGGER_STD_CHRONO )
      #include <chrono>
      #define USAGI_LOG_EASY_LOGGER_CLOCK_TYPE std::chrono::steady_clock
    #else
      #include "../chrono/default_clock.hxx"
      #define USAGI_LOG_EASY_LOGGER_CLOCK_TYPE usagi::chrono::default_clock
    #endif
  #endif
#endif

#include <string>
#include <iostream>
#include <iomanip>

namespace usagi::log::easy_logger
{
  using namespace std;

#ifndef USE_USAGI_LOG_EASY_LOGGER_BOOST_CPU_TIMER
  #ifdef USE_USAGI_LOG_EASY_LOGGER_BOOST_CHRONO
    using namespace boost::chrono;
  #else
    using namespace std::chrono;
  #endif
#endif
  
#ifdef DISABLE_USAGI_LOG_EASY_LOGGER
  
  struct log_null
  {
    template < typename T >
    decltype( auto ) operator<<( const T& ) { return *this; }
  };
  
#else
  
#ifdef USE_USAGI_LOG_EASY_LOGGER_BOOST_CPU_TIMER
  boost::timer::cpu_timer global_timer;
#else
  const auto global_start_time = USAGI_LOG_EASY_LOGGER_CLOCK_TYPE::now();
#endif
  
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
    
    static constexpr auto log_time_width = 16;
    
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
          << "[ " << setw( log_time_width )
          << setprecision( 9 )
          << fixed
#ifdef USE_USAGI_LOG_EASY_LOGGER_BOOST_CPU_TIMER
          << static_cast< long double >( global_timer.elapsed().wall ) * 1.0e-9
#else
          << duration_cast< duration< long double > >( USAGI_LOG_EASY_LOGGER_CLOCK_TYPE::now() - global_start_time ).count()
#endif
          << " ]"
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

#endif

}
