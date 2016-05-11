#pragma once

#include "helper.hxx"
#include "material.hxx"
#include "animation.hxx"
#include "vertex_buffer.hxx"
#include "bone.hxx"

#include <boost/gil/gil_all.hpp>
#include <boost/optional.hpp>

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include <unordered_map>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      class mesh_type
      {
        friend class node_type;
        
        GLuint  _triangle_vb_id;
        GLuint  _triangle_ib_id;
        
        GLsizei _count_of_indices;
        
        GLuint  _triangle_vao_id;
        
        /// @brief このメッシュのマテリアル
        const shared_material_type _shared_material;
        
        /// @brief このメッシュのボーン群
        const shared_bone_offsets_type _shared_bone_offsets;
        
        /// @brief このメッシュのボーンインデックスへボーンの名前から参照するためのマップ
        const shared_bone_map_type _shared_bone_map;
        
        /// @brief このメッシュが関わるアニメーションを含むアニメーション群
        const shared_animation_map_type _shared_animation_map;
        
        using vertices_buffer_type = std::vector< vertex_buffer_type >;
        using indices_buffer_type  = std::vector< GLuint >;
        
        /// @brief 内部用: 頂点バッファーとインデックスバッファーを assimp のメッシュに基いて生成する
        auto initialize_prepare_buffers
        ( const aiMesh*         mesh
        , vertices_buffer_type& vb
        , indices_buffer_type&  ib
        )
          ->void
        {
          constexpr auto indices_of_triangle = 3;
          
          for ( auto n_vertex = 0u; n_vertex < mesh->mNumVertices; ++n_vertex )
            vb.emplace_back
            ( std::move( helper::to_glm_vec4( mesh->mVertices           + n_vertex ) )
            , std::move( mesh->mColors[ 0 ]        ? helper::to_glm_vec4( mesh->mColors[ 0 ]        + n_vertex ) : glm::vec4( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 0 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 0 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 1 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 1 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 2 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 2 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 3 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 3 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 4 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 4 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 5 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 5 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 6 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 6 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mTextureCoords[ 7 ] ? helper::to_glm_vec2( mesh->mTextureCoords[ 7 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
            , std::move( mesh->mNormals            ? helper::to_glm_vec3( mesh->mNormals            + n_vertex ) : glm::vec3( std::nanf("") ) )
            , std::move( mesh->mTangents           ? helper::to_glm_vec3( mesh->mTangents           + n_vertex ) : glm::vec3( std::nanf("") ) )
            , std::move( mesh->mBitangents         ? helper::to_glm_vec3( mesh->mBitangents         + n_vertex ) : glm::vec3( std::nanf("") ) )
            , std::move( glm::vec4( 0.0f ) )
            , std::move( glm::vec4( 0.0f ) )
            );
          
          for ( auto n_face = 0u; n_face < mesh->mNumFaces; ++ n_face )
          {
            const auto face = mesh->mFaces + n_face;
            
            if ( face->mNumIndices not_eq indices_of_triangle )
              throw std::runtime_error( "required must be indices of face is 3. try create_model with aiProcess_Triangulate." );
            
            ib.emplace_back( std::move( face->mIndices[0] ) );
            ib.emplace_back( std::move( face->mIndices[1] ) );
            ib.emplace_back( std::move( face->mIndices[2] ) );
            
          }
          
          _count_of_indices = ib.size();
        }
        
        /// @brief 内部用: アニメーションのためのボーン情報を assimp のメッシュに基いて生成し頂点バッファーへ格納する
        auto initialize_animation_bone
        ( const aiMesh*         mesh
        , vertices_buffer_type& vb
        )
          ->void
        {
          // アニメーション・ボーンまわり
          
          const auto bones = mesh->mBones;
          
          for ( auto n_bone = 0u; n_bone < mesh->mNumBones; ++n_bone )
          {
            const auto bone = bones[ n_bone ];
            
            const std::string bone_name( bone->mName.C_Str() );
            
            unsigned bone_index = 0;
            
            if ( _shared_bone_map->find( bone_name ) == _shared_bone_map->end() )
            {
              bone_index = _shared_bone_map->size();
              _shared_bone_offsets->push_back( glm::mat4( 1.0f ) );
              
              constexpr auto max_bones = 48u;
              
              if ( _shared_bone_offsets->size() > max_bones )
                throw std::runtime_error
                ( "bone offset size " + std::to_string( _shared_bone_offsets->size() )
                + " over shader::max_bones " + std::to_string( max_bones )
                + "."
                );
            }
            else
              bone_index = _shared_bone_map->at( bone_name );
            
            _shared_bone_map->at( bone_name ) = bone_index;
            
            // TODO: もしかしたら .x 形式以外では bone_offset に transpose していると怪奇現象化するかも。要確認
            // pattern: .x is ok
            _shared_bone_offsets->at( bone_index ) = glm::transpose( helper::to_glm_mat4( bone->mOffsetMatrix ) );
            // transpose しない場合:
            // _shared_bone_offsets->at( bone_index ) = helper::to_glm_mat4( bone->mOffsetMatrix );
            
            for ( auto n_weight = 0u; n_weight < bone->mNumWeights; ++n_weight )
            {
              const auto& weight = bone->mWeights[ n_weight ];
              
              auto& vertex = vb[ weight.mVertexId ];
              bool overflow_check = true;
              
              for ( auto n = 0; n < 4; ++n )
                if ( vertex.bone_weights[ n ] == 0.0f )
                {
                  vertex.bone_ids    [ n ] = bone_index;
                  vertex.bone_weights[ n ] = weight.mWeight;
                  overflow_check = false;
                  break;
                }
                
              if ( overflow_check )
                throw std::runtime_error( "bone buffer is not enought, need limit data bone/vertex <= 4, or fix engine." );
            }
          }
        }
        
        /// @brief 内部用: 頂点バッファー、インデックスバッファーを生成
        auto initialize_generate_buffers
        ( const vertices_buffer_type& vb
        , const indices_buffer_type&  ib
        )
          ->void
        {
          glGenVertexArrays( 1, &_triangle_vao_id );
          glBindVertexArray( _triangle_vao_id );
          
          glGenBuffers( 1, &_triangle_vb_id);
          glGenBuffers( 1, &_triangle_ib_id);
          
          glBindBuffer( GL_ARRAY_BUFFER, _triangle_vb_id );
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _triangle_ib_id );
          
          glBufferData
          ( GL_ARRAY_BUFFER
          , vb.size() * vertex_buffer_type::size_of_memory
          , vb.data()->to_ptr<void>()
          , GLenum( GL_STATIC_DRAW )
          );
          
          glBufferData
          ( GL_ELEMENT_ARRAY_BUFFER
          , ib.size() * sizeof( indices_buffer_type::value_type )
          , reinterpret_cast< const void* >( ib.data() )
          , GLenum( GL_STATIC_DRAW )
          );
          
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
          glBindBuffer( GL_ARRAY_BUFFER        , 0 );
          glBindVertexArray( 0 );
        }
        
      public:
        
        /// @brief assimp のメッシュに基いて usagi::gl::model で取り扱い可能なメッシュオブジェクトを構築
        mesh_type
        ( const aiMesh*                       mesh
        , const shared_shared_materials_type& shared_shared_materials
        , const shared_bone_offsets_type&     shared_bone_offsets
        , const shared_bone_map_type&         shared_bone_map
        , const shared_animation_map_type&    shared_animation_map
        )
          : _shared_material     ( shared_shared_materials->at( mesh->mMaterialIndex ) )
          , _shared_bone_offsets ( shared_bone_offsets   )
          , _shared_bone_map     ( shared_bone_map       )
          , _shared_animation_map( shared_animation_map  )
        {
          // initialize_generate_buffers 後は不要となる一時オブジェクトのためメンバー化する必要は無い
          vertices_buffer_type vb;
          indices_buffer_type  ib;
          
          // vb, ib は参照渡しで変更される
          initialize_prepare_buffers ( mesh, vb, ib );
          initialize_animation_bone  ( mesh, vb     );
          
          // vb, ib はここでメインメモリーからVRAMのオブジェクトへ転送されメインメモリー上では不要となる
          initialize_generate_buffers(       vb, ib );
        }
        
        ~mesh_type()
        {
          glDeleteBuffers( 1, &_triangle_ib_id );
          glDeleteBuffers( 1, &_triangle_vb_id );
          glDeleteVertexArrays( 1, &_triangle_vao_id );
        }
        
        /// @brief 必要なマテリアルをバインドしメッシュを描画する
        auto draw()
          ->void
        {
          // vertex transfar
          
          constexpr GLenum    attribute     = GL_FLOAT;
          constexpr GLboolean normalize_on  = static_cast< GLboolean >( true  );
          constexpr GLboolean normalize_off = static_cast< GLboolean >( false );
          
          const auto program_id = get_current_program();
          
          {
            glBindVertexArray( _triangle_vao_id );
            
            glBindBuffer( GL_ARRAY_BUFFER, _triangle_vb_id );
            
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _triangle_ib_id );
            
            {
              const auto location_of_vs_position     = glGetAttribLocation( program_id, "position"     );
              const auto location_of_vs_color        = glGetAttribLocation( program_id, "color"        );
              const auto location_of_vs_texcoord0    = glGetAttribLocation( program_id, "texcoord0"    );
              const auto location_of_vs_texcoord1    = glGetAttribLocation( program_id, "texcoord1"    );
              const auto location_of_vs_texcoord2    = glGetAttribLocation( program_id, "texcoord2"    );
              const auto location_of_vs_texcoord3    = glGetAttribLocation( program_id, "texcoord3"    );
              const auto location_of_vs_texcoord4    = glGetAttribLocation( program_id, "texcoord4"    );
              const auto location_of_vs_texcoord5    = glGetAttribLocation( program_id, "texcoord5"    );
              const auto location_of_vs_texcoord6    = glGetAttribLocation( program_id, "texcoord6"    );
              const auto location_of_vs_texcoord7    = glGetAttribLocation( program_id, "texcoord7"    );
              const auto location_of_vs_normal       = glGetAttribLocation( program_id, "normal"       );
              const auto location_of_vs_tangent      = glGetAttribLocation( program_id, "tangent"      );
              const auto location_of_vs_bitangent    = glGetAttribLocation( program_id, "bitangent"    );
              const auto location_of_vs_bone_ids     = glGetAttribLocation( program_id, "bone_ids"     );
              const auto location_of_vs_bone_weights = glGetAttribLocation( program_id, "bone_weights" );
              
              if ( location_of_vs_position not_eq -1 )
              {
                glVertexAttribPointer
                ( location_of_vs_position
                , vertex_buffer_type::count_of_position_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_position )
                );
                glEnableVertexAttribArray( location_of_vs_position );
              }
              
              if ( location_of_vs_color not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_color
                , vertex_buffer_type::count_of_color_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_color )
                );
                glEnableVertexAttribArray( location_of_vs_color );
              }
             
              if ( location_of_vs_texcoord0 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord0
                , vertex_buffer_type::count_of_texcoord0_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord0 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord0 );
              }
              if ( location_of_vs_texcoord1 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord1
                , vertex_buffer_type::count_of_texcoord1_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord1 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord1 );
              }
              if ( location_of_vs_texcoord2 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord2
                , vertex_buffer_type::count_of_texcoord2_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord2 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord2 );
              }
              if ( location_of_vs_texcoord3 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord3
                , vertex_buffer_type::count_of_texcoord3_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord3 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord3 );
              }
              if ( location_of_vs_texcoord4 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord4
                , vertex_buffer_type::count_of_texcoord4_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord4 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord4 );
              }
              if ( location_of_vs_texcoord5 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord5
                , vertex_buffer_type::count_of_texcoord5_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord5 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord5 );
              }
              if ( location_of_vs_texcoord6 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord6
                , vertex_buffer_type::count_of_texcoord6_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord6 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord6 );
              }
              if ( location_of_vs_texcoord7 not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_texcoord7
                , vertex_buffer_type::count_of_texcoord7_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_texcoord7 )
                );
                glEnableVertexAttribArray( location_of_vs_texcoord7 );
              }
             
              if ( location_of_vs_normal not_eq -1 )
              {
                glVertexAttribPointer( location_of_vs_normal 
                , vertex_buffer_type::count_of_normal_elements 
                , attribute
                , normalize_on
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast<void*>( vertex_buffer_type::memory_offset_of_normal )
                );
                glEnableVertexAttribArray( location_of_vs_normal );
              }
              
              if ( location_of_vs_tangent not_eq -1 )
              {
                glVertexAttribPointer
                ( location_of_vs_tangent
                , vertex_buffer_type::count_of_tangent_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast< void* >( vertex_buffer_type::memory_offset_of_tangent )
                );
                glEnableVertexAttribArray( location_of_vs_tangent );
              }
              
              if ( location_of_vs_bitangent not_eq -1 )
              {
                glVertexAttribPointer
                ( location_of_vs_bitangent
                , vertex_buffer_type::count_of_bitangent_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast< void* >( vertex_buffer_type::memory_offset_of_bitangent )
                );
                glEnableVertexAttribArray( location_of_vs_bitangent );
              }
              
              if ( location_of_vs_bone_ids not_eq -1 )
              {
                glVertexAttribPointer
                ( location_of_vs_bone_ids
                , vertex_buffer_type::count_of_bone_ids_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast< void* >( vertex_buffer_type::memory_offset_of_bone_ids )
                );
                glEnableVertexAttribArray( location_of_vs_bone_ids );
              }
              
              if ( location_of_vs_bone_weights not_eq -1 )
              {
                glVertexAttribPointer
                ( location_of_vs_bone_weights
                , vertex_buffer_type::count_of_bone_weights_elements
                , attribute
                , normalize_off
                , vertex_buffer_type::size_of_memory
                , reinterpret_cast< void* >( vertex_buffer_type::memory_offset_of_bone_weights )
                );
                glEnableVertexAttribArray( location_of_vs_bone_weights );
              }
            }
            
            _shared_material->bind();
            
            glDrawElements( GL_TRIANGLES, _count_of_indices, GL_UNSIGNED_INT, nullptr );
            
            _shared_material->unbind();
          }
          
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
          glBindBuffer( GL_ARRAY_BUFFER, 0 );
          glBindVertexArray( 0 );
        }
        
      };
      
      using meshes_type = std::vector< mesh_type >;
      using shared_meshes_type = std::shared_ptr< meshes_type >;
      
      static inline auto make_shared_meshes() { return std::make_shared< meshes_type >(); }
      
    }
  }
}