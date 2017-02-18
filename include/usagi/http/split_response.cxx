#pragma once

#include "structured_response_type.hxx"

#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string.hpp>

namespace usagi::http
{
  /// @brief HTTP RESPONSE 文字列を HEADER と BODY に分離する
  /// @param in RESPONSE 文字列
  /// @note 出力のメンバー headers の key は全て小文字に自動的に統一される
  auto split_response( const std::string& response )
    -> structured_response_type
  {
    structured_response_type out;
    
    using namespace boost::xpressive;
    
    mark_tag code(1), code_string(2), headers(2), body(4);
    
    sregex r
      =  bos
      >> as_xpr( "HTTP/" )
      >> +( boost::xpressive::set[ alnum | '.' ] )
      >> ( code = +alnum )
      >> as_xpr( ' ' )
      >> ( code_string = *_ )
      >> as_xpr( http_end_of_line )
      >> ( headers = *_ )
      >> as_xpr( http_separator )
      >> ( body = *_ )
      >> eos
      ;
    
    smatch m;
    
    if ( not regex_search( response, m, r ) )
      throw runtime_error( "cannot split the response: " + response );
    
    out.code = static_cast< decltype( out.code ) >( stoul( m[ code ] ) );
    out.code_string = m[ code_string ];
    out.body = m[ body ];
    
    {
      using boost::split;
      using boost::is_any_of;
      using boost::token_compress_on;
      using boost::trim;
      
      vector< string > buffer;
      buffer.reserve( 32 );
      split( buffer, m[ headers ], is_any_of( http_end_of_line ), token_compress_on );
      
      for ( const auto& line : buffer )
      {
        const auto p = line.find( ':' );
        
        if ( p == line.npos )
          throw runtime_error( "invalid line: " + line );
        
        auto key   = line.substr( 0, p );
        trim( key );
        
        auto value = line.substr( p + 1 );
        trim( value );
        
        out.headers.emplace( move( key ), move( value ) );
      }
    }
    
    return out;
  }
  
  auto split_response_optional( const string& in )
    -> optional< structured_response_type >
  { try { return split_response( in ); } catch ( ... ) { } return { }; }
  
  auto split_response_optional( const optional< string >& in )
    -> optional< structured_response_type >
  { try { if ( in ) return split_response( *in ); } catch ( ... ) { } return { }; }
}
