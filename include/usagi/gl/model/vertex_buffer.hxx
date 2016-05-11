#pragma once

#include "../../external_library/glm.hxx"

#include <cstddef>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      struct vertex_buffer_type
      {
        using float_type = float;
        
        glm::vec4 position;
        glm::vec4 color;
        glm::vec2 texcoord0;
        glm::vec2 texcoord1;
        glm::vec2 texcoord2;
        glm::vec2 texcoord3;
        glm::vec2 texcoord4;
        glm::vec2 texcoord5;
        glm::vec2 texcoord6;
        glm::vec2 texcoord7;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec4 bone_ids;
        glm::vec4 bone_weights;
        
        vertex_buffer_type
        ( glm::vec4&& pos
        , glm::vec4&& col
        , glm::vec2&& tex0
        , glm::vec2&& tex1
        , glm::vec2&& tex2
        , glm::vec2&& tex3
        , glm::vec2&& tex4
        , glm::vec2&& tex5
        , glm::vec2&& tex6
        , glm::vec2&& tex7
        , glm::vec3&& nor
        , glm::vec3&& tan
        , glm::vec3&& bit
        , glm::vec4&& bid
        , glm::vec4&& bwt
        )
          : position     ( std::move( pos ) )
          , color        ( std::move( col ) )
          , texcoord0    ( std::move( tex0 ) )
          , texcoord1    ( std::move( tex1 ) )
          , texcoord2    ( std::move( tex2 ) )
          , texcoord3    ( std::move( tex3 ) )
          , texcoord4    ( std::move( tex4 ) )
          , texcoord5    ( std::move( tex5 ) )
          , texcoord6    ( std::move( tex6 ) )
          , texcoord7    ( std::move( tex7 ) )
          , normal       ( std::move( nor ) )
          , tangent      ( std::move( tan ) )
          , bitangent    ( std::move( bit ) )
          , bone_ids     ( std::move( bid ) )
          , bone_weights ( std::move( bwt ) )
        { }
        
        template < class T = float_type >
        auto to_ptr() const
          -> const T*
        {
          return reinterpret_cast< const T* >( &position.x );
        }
        
        static constexpr auto size_of_element = sizeof( float_type );
        
        static constexpr auto size_of_memory
          = ( sizeof( decltype( position     ) )
            + sizeof( decltype( color        ) )
            + sizeof( decltype( texcoord0    ) )
            + sizeof( decltype( texcoord1    ) )
            + sizeof( decltype( texcoord2    ) )
            + sizeof( decltype( texcoord3    ) )
            + sizeof( decltype( texcoord4    ) )
            + sizeof( decltype( texcoord5    ) )
            + sizeof( decltype( texcoord6    ) )
            + sizeof( decltype( texcoord7    ) )
            + sizeof( decltype( normal       ) )
            + sizeof( decltype( tangent      ) )
            + sizeof( decltype( bitangent    ) )
            + sizeof( decltype( bone_ids     ) )
            + sizeof( decltype( bone_weights ) )
            );
        
        static constexpr auto count_of_elements = size_of_memory / size_of_element;
        
        static constexpr auto count_of_position_elements     = sizeof( decltype( position )     ) / size_of_element;
        static constexpr auto count_of_color_elements        = sizeof( decltype( color )        ) / size_of_element;
        static constexpr auto count_of_texcoord0_elements    = sizeof( decltype( texcoord0 )    ) / size_of_element;
        static constexpr auto count_of_texcoord1_elements    = sizeof( decltype( texcoord1 )    ) / size_of_element;
        static constexpr auto count_of_texcoord2_elements    = sizeof( decltype( texcoord2 )    ) / size_of_element;
        static constexpr auto count_of_texcoord3_elements    = sizeof( decltype( texcoord3 )    ) / size_of_element;
        static constexpr auto count_of_texcoord4_elements    = sizeof( decltype( texcoord4 )    ) / size_of_element;
        static constexpr auto count_of_texcoord5_elements    = sizeof( decltype( texcoord5 )    ) / size_of_element;
        static constexpr auto count_of_texcoord6_elements    = sizeof( decltype( texcoord6 )    ) / size_of_element;
        static constexpr auto count_of_texcoord7_elements    = sizeof( decltype( texcoord7 )    ) / size_of_element;
        static constexpr auto count_of_normal_elements       = sizeof( decltype( normal )       ) / size_of_element;
        static constexpr auto count_of_tangent_elements      = sizeof( decltype( tangent )      ) / size_of_element;
        static constexpr auto count_of_bitangent_elements    = sizeof( decltype( bitangent )    ) / size_of_element;
        static constexpr auto count_of_bone_ids_elements     = sizeof( decltype( bone_ids )     ) / size_of_element;
        static constexpr auto count_of_bone_weights_elements = sizeof( decltype( bone_weights ) ) / size_of_element;
        
        static constexpr auto memory_offset_of_position     = std::size_t( 0 );
        static constexpr auto memory_offset_of_color        = memory_offset_of_position  + sizeof( decltype( position  ) );
        static constexpr auto memory_offset_of_texcoord0    = memory_offset_of_color     + sizeof( decltype( color     ) );
        static constexpr auto memory_offset_of_texcoord1    = memory_offset_of_texcoord0 + sizeof( decltype( texcoord0 ) );
        static constexpr auto memory_offset_of_texcoord2    = memory_offset_of_texcoord1 + sizeof( decltype( texcoord1 ) );
        static constexpr auto memory_offset_of_texcoord3    = memory_offset_of_texcoord2 + sizeof( decltype( texcoord2 ) );
        static constexpr auto memory_offset_of_texcoord4    = memory_offset_of_texcoord3 + sizeof( decltype( texcoord3 ) );
        static constexpr auto memory_offset_of_texcoord5    = memory_offset_of_texcoord4 + sizeof( decltype( texcoord4 ) );
        static constexpr auto memory_offset_of_texcoord6    = memory_offset_of_texcoord5 + sizeof( decltype( texcoord5 ) );
        static constexpr auto memory_offset_of_texcoord7    = memory_offset_of_texcoord6 + sizeof( decltype( texcoord6 ) );
        static constexpr auto memory_offset_of_normal       = memory_offset_of_texcoord7 + sizeof( decltype( texcoord7 ) );
        static constexpr auto memory_offset_of_tangent      = memory_offset_of_normal    + sizeof( decltype( normal    ) );
        static constexpr auto memory_offset_of_bitangent    = memory_offset_of_tangent   + sizeof( decltype( tangent   ) );
        static constexpr auto memory_offset_of_bone_ids     = memory_offset_of_bitangent + sizeof( decltype( bitangent ) );
        static constexpr auto memory_offset_of_bone_weights = memory_offset_of_bone_ids  + sizeof( decltype( bone_ids  ) );
      };

    }
  }
}