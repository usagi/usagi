#pragma once

#include <minizip/unzip.h>
#include <boost/filesystem.hpp>
#define BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS
#include <boost/scope_exit.hpp>
#include <array>

namespace usagi
{
  namespace zip
  {
    static inline auto unzip( const std::string& in_path )
      -> bool
    {
      auto file = unzOpen( in_path.c_str() );
      
      if ( not file )
        return false;
      
      BOOST_SCOPE_EXIT( &file ){ unzClose( file ); };
      
      unz_global_info global_info;
      
      if ( unzGetGlobalInfo( file, &global_info ) != UNZ_OK )
        return false;
      
      if ( unzGoToFirstFile( file ) != UNZ_OK )
        return false;
      
      for ( auto n = 0; n < global_info.number_entry; ++n )
      {
        std::string filename;
        filename.resize( 255, '\0' );
        
        if ( unzGetCurrentFileInfo64( file, nullptr, &filename[ 0 ], filename.size(), nullptr, 0, nullptr, 0 ) != UNZ_OK )
          return false;
        
        auto p = ( boost::filesystem::path( in_path ).parent_path() += "/" ) / filename;
        
        if ( unzOpenCurrentFile( file ) != UNZ_OK )
          return false;
        
        {
          BOOST_SCOPE_EXIT( &file ) { unzCloseCurrentFile( file ); };
          
          std::ofstream o( p.string(), std::ios::binary );
          
          if ( not o.is_open() )
            return false;
          
          constexpr auto buffer_size = 32768;
          std::array< std::uint8_t, buffer_size > buffer;
          
          int read_size, total_size = 0;
          while ( ( read_size = unzReadCurrentFile( file, &buffer[ 0 ], buffer_size ) ) > 0 )
          {
            total_size += read_size;
            o.write( (const char*)buffer.data(), read_size );
          }
        }
        
        unzGoToNextFile( file );
      }
      
      return true;
    }
  }
}