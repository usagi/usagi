#pragma once

#include "../../external_library/glm.hxx"
#include "../../external_library/stb_image.hxx"

#include "../texture.hxx"
#include "../uniform.hxx"

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include <unordered_map>
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
      
      class material_type
      {
      public:
      
        std::unordered_map< std::string, uniform_value_type  > parameters;
        std::unordered_map< std::string, shared_texture_type > textures;
        
        // assimp からの生成用
        material_type( aiMaterial* material, const std::string& path_prefix = "" )
        {
          reset_parameters( material );
          reset_textures( material, path_prefix );
        }
        
        /// @todo http://assimp.sourceforge.net/lib_html/material_8h.html <-- 全部に対応する
        auto reset_parameters( aiMaterial* material )
          -> void
        {
          // Note: Assimpの aiMaterial::Get のパラメーターは
          // カンマを含む黒魔法 AI_MATKEY_COLOR_DIFFUSE などで
          // ( const char* pkey, unsigned type, unsigned idx) を渡そうとする。
          const auto get_material_color_to_glm_vec3 =
          [ material ]
          ( const char* pkey, unsigned type, unsigned idx, float default_value )
            -> glm::vec3
          {
            aiColor3D color;
            
            const auto succeeded = material->Get( pkey, type, idx, color );
            
            return ( succeeded == aiReturn_SUCCESS )
              ? glm::vec3( color.r, color.g, color.b )
              : glm::vec3( default_value )
              ;
          };
          
          parameters["diffuse"]     = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_DIFFUSE    , 1.0f );
          parameters["specular"]    = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_SPECULAR   , 0.0f );
          parameters["ambient"]     = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_AMBIENT    , 0.0f );
          parameters["emissive"]    = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_EMISSIVE   , 0.0f );
          
          parameters["transparent"] = glm::vec1( get_material_color_to_glm_vec3( AI_MATKEY_COLOR_TRANSPARENT, 1.0f )[0] );
          parameters["reflective"]  = glm::vec1( get_material_color_to_glm_vec3( AI_MATKEY_COLOR_REFLECTIVE , 0.0f )[0] );
        }
        
        auto reset_textures( aiMaterial* material, const std::string& path_prefix = "" )
          -> void
        {
          const auto process_texture =
            [&]
            ( const aiTextureType type, const std::string& name )
            {
              const auto count_of_textures = material -> GetTextureCount( type );
              
              for ( auto number_of_texture = 0u; number_of_texture < count_of_textures; ++number_of_texture )
              {
                aiString path;
                material->GetTexture( type, number_of_texture, &path, nullptr, nullptr, nullptr, nullptr, nullptr );
                
                int width = 0, height = 0, elements = 0;
                auto data = stbi_load( ( path_prefix + path.C_Str() ).c_str(), &width, &height, &elements, 0 );
                
                if ( not data )
                  continue;
                
                auto shared_texture = make_shared_texture();
                
                switch ( elements )
                {
                  case 4:
                    shared_texture->load_image( reinterpret_cast< glm::u8vec4* >( data ), width, height );
                  case 3:
                    shared_texture->load_image( reinterpret_cast< glm::u8vec3* >( data ), width, height );
                  case 2:
                    shared_texture->load_image( reinterpret_cast< glm::u8vec2* >( data ), width, height );
                  case 1:
                    shared_texture->load_image( reinterpret_cast< glm::u8vec1* >( data ), width, height );
                  default:
                    throw std::runtime_error( "unknown element size from stbi_load" );
                }
                
                textures[ name + std::to_string( number_of_texture ) ] = std::move( shared_texture );
                
                stbi_image_free( data );
              }
            };
          
          process_texture( aiTextureType_DIFFUSE     , "diffuse"      );
          process_texture( aiTextureType_SPECULAR    , "specular"     );
          process_texture( aiTextureType_AMBIENT     , "ambient"      );
          process_texture( aiTextureType_EMISSIVE    , "emissive"     );
          process_texture( aiTextureType_HEIGHT      , "height"       );
          process_texture( aiTextureType_NORMALS     , "normals"      );
          process_texture( aiTextureType_SHININESS   , "shininess"    );
          process_texture( aiTextureType_OPACITY     , "opacity"      );
          process_texture( aiTextureType_DISPLACEMENT, "displacement" );
          process_texture( aiTextureType_LIGHTMAP    , "lightmap"     );
          process_texture( aiTextureType_REFLECTION  , "reflection"   );
        }
        
        auto bind()
        {
          for ( const auto& parameter_pair : parameters )
            set_uniform( parameter_pair.first, parameter_pair.second );
          
          std::underlying_type_t< GLenum > current_texture_unit_number = 0;
          
          for ( const auto& texture_pair : textures )
          {
            glActiveTexture( static_cast< GLenum >( static_cast< std::underlying_type_t< GLenum > >( GL_TEXTURE0 ) + current_texture_unit_number ) );
            
            texture_pair.second->bind( current_texture_unit_number );
            
            set_uniform( texture_pair.first, static_cast< GLint >( current_texture_unit_number ) );
            
            ++current_texture_unit_number;
          }
        }
        
        auto unbind()
        {
          std::underlying_type_t< GLenum > current_texture_unit_number = 0;
          
          for ( const auto& texture_pair : textures )
            texture_pair.second->unbind( current_texture_unit_number++ );
        }
        
      };
      
      using shared_material_type         = std::shared_ptr< material_type >;
      using shared_materials_type        = std::vector< shared_material_type >;
      using shared_shared_materials_type = std::shared_ptr< shared_materials_type >;
      
    }
  }
}