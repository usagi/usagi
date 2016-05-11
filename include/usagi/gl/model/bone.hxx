#pragma once

#include "../../external_library/glm.hxx"

#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      struct vertex_bone_type
      {
        glm::vec4 bone_ids;
        glm::vec4 bone_weights;
        
        vertex_bone_type()
          : bone_ids    ( 0.0f )
          , bone_weights( 0.0f )
        { }
        
        auto push( float id, float weight )
          -> void
        {
          constexpr auto max_count = 4;
          
          for ( auto n = 0; n < max_count; ++n )
            if ( bone_weights[ n ] == 0.0f )
            {
              bone_ids    [ n ] = id;
              bone_weights[ n ] = weight;
              return;
            }
          
          throw std::runtime_error( "count of bones is not enought. need count <= 4 or fix up the wonderland.renderer." );
        }
      };
      
      using vertex_bones_type = std::vector< vertex_bone_type >;
      
      using bone_offset_type         = glm::mat4;
      using bone_offsets_type        = std::vector< bone_offset_type >;
      using shared_bone_offsets_type = std::shared_ptr< bone_offsets_type >;
      
      using bone_name_type       = std::string;
      using bone_index_type      = std::uint32_t;
      using bone_map_type        = std::unordered_map< bone_name_type, bone_index_type >;
      using shared_bone_map_type = std::shared_ptr< bone_map_type >;
      
    }
  }
}