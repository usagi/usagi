#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>

namespace usagi
{
  namespace brotli
  {
    namespace c
    {
#include <brotli/dec/decode.h>
#include <brotli/enc/encode.h>
    }
    
    enum class mode_type
      : std::uint32_t
    { generic = c::BROTLI_MODE_GENERIC
    , text    = c::BROTLI_MODE_TEXT
    , font    = c::BROTLI_MODE_FONT
    };
    
    /// @param window_size [10..24]
    template
    < typename OUT_CONTAINER_TYPE = std::vector< std::uint8_t >
    , typename IN_CONTAINER_TYPE  = std::vector< std::uint8_t >
    >
    static inline auto compress
    ( const IN_CONTAINER_TYPE& in
    , const int quality     = 11
    , const int window_size = 22
    , const mode_type mode = mode_type::generic
    ) -> OUT_CONTAINER_TYPE
    {
      if ( window_size < 10 or window_size > 24 )
      {  std::cerr << "a";throw std::runtime_error( "brotli `window_size` parameter be require [10..24]." );
      }
      
      OUT_CONTAINER_TYPE out;
      
      const auto in_size = in.size() * sizeof( in[0] );
      
      out.resize( c::BrotliEncoderMaxCompressedSize( in_size ) );
      
      std::size_t encoded_size = out.size() * sizeof( out[0] );
      
      if
      ( not c::BrotliEncoderCompress
        ( quality
        , window_size
        , static_cast< c::BrotliEncoderMode >( static_cast< std::uint32_t >( mode ) )
        , in_size
        , reinterpret_cast< const std::uint8_t* >( in.data() )
        , &encoded_size
        , (std::uint8_t*)( &out[0] )
        )
      )
      {  std::cerr << "b"; throw std::runtime_error( "brotli BrotliEncoderCompress was failed." );
      }
      
      out.resize( encoded_size );
      
      return out;
    }
    
    template
    < std::size_t OUT_BUFFER_SIZE = 65536
    , typename OUT_CONTAINER_TYPE = std::vector< std::uint8_t >
    , typename IN_CONTAINER_TYPE  = std::vector< std::uint8_t >
    >
    static inline auto decompress
    ( const IN_CONTAINER_TYPE& in
    ) -> OUT_CONTAINER_TYPE
    {
      OUT_CONTAINER_TYPE out;
      
      out.resize( OUT_BUFFER_SIZE );
      
      std::size_t decoded_size = out.size();
      
      if
      ( not c::BrotliDecoderDecompress
        ( in.size()
        , reinterpret_cast< const std::uint8_t* >( in.data() )
        , &decoded_size
        , (std::uint8_t*)( &out[0] )
        )
      )
        throw std::runtime_error( "brotli BrotliEncoderCompressStream was failed." );
      
      out.resize( decoded_size );
      
      return out;
    }
    
  }
}