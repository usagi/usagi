#pragma once

#include <boost/uuid/sha1.hpp>

#include <vector>
#include <array>

namespace usagi
{
  namespace hash
  {
    using sha1_digest_type = std::array< std::uint32_t, 5 >;

    static inline auto sha1( const std::uint8_t* data, const std::size_t size )
      -> sha1_digest_type
    {
      boost::uuids::detail::sha1 sha1;
      std::uint32_t digest[5];
      
      sha1.process_bytes( data, size );
      sha1.get_digest( digest );
      
      return { digest[0], digest[1], digest[2], digest[3], digest[4] };
    }
    
    static inline auto sha1( const std::vector< std::uint8_t >& data )
      -> sha1_digest_type
    {
      return sha1( &data[0], data.size() );
    }
    
  }
}