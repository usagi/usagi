#pragma once

#include <string>
#include <sstream>
#include <iomanip>

namespace usagi
{
  namespace url_encode
  {
    /// @reference http://stackoverflow.com/a/17708801/1211392
    template < typename T = std::string >
    static inline auto encode( const T& in )
      -> T
    {
      std::basic_ostringstream< T::value_type > escaped;
      escaped.fill( T::value_type( '0' ) );
      escaped << std::hex;
      
      for ( const auto c : in )
      {
        if
        (    std::isalnum( c )
          or c == T::value_type( '-' )
          or c == T::value_type( '_' )
          or c == T::value_type( '.' )
          or c == T::value_type( '~' )
        )
        {
          escaped << c;
          continue;
        }
        
        escaped
          << std::uppercase
          << T::value_type( '%' )
          << std::setw( 2 )
          << static_cast< int >( *reinterpret_cast< const unsigned char* >( &c ) )
          << std::nouppercase
          ;
      }
      
      return escaped.str();
    }
    
  }
}