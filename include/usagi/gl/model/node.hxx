#pragma once

#include "helper.hxx"
#include "mesh.hxx"
#include "animation.hxx"

#include "../../external_library/glm.hxx"

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      class node_type
      {
        std::string _name;
        
        using nodes_type = std::vector< node_type >;
        
        nodes_type _nodes;
        
        using index_type = std::int32_t;
        using indices_type = std::vector< index_type >;
        
        indices_type _indices;
        
        glm::mat4 _transformation;
        glm::mat4 _before_gpu_local_transformation;
        
        const shared_animation_map_type _shared_animation_map;
        
      public:
        
        node_type
        ( const aiNode* node
        , const shared_animation_map_type& shared_animation_map
        , const bool transpose_node
        )
          : _name( node->mName.C_Str() )
          , _shared_animation_map( shared_animation_map )
        {
          _nodes.reserve( node->mNumMeshes );
          
          for ( auto n = 0u; n < node->mNumChildren; ++n )
            _nodes.emplace_back( node->mChildren[ n ], shared_animation_map, transpose_node );
          
          _indices.reserve( node->mNumMeshes );
          
          for ( auto n = 0u; n < node->mNumMeshes; ++n )
            _indices.emplace_back( node->mMeshes[ n ] );
          
          _transformation = transpose_node
            ? glm::transpose( helper::to_glm_mat4( node->mTransformation ) )
            : helper::to_glm_mat4( node->mTransformation )
            ;
        }
        
        auto draw
        ( const shared_meshes_type& meshes
        )
         ->void
        {
          for ( auto& node : _nodes )
            node.draw( meshes );
          
          for ( const auto index : _indices )
            (*meshes)[ index ].draw();
        }
        
        auto transformation() const
         ->const glm::mat4&
        {
          return _transformation;
        }
        
        auto name() const
         ->const std::string&
        {
          return _name;
        }
        
        auto nodes() const
         ->const decltype( _nodes )&
        {
          return _nodes;
        }
        
      };
      
    }
  }
}