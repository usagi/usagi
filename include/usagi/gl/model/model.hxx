#pragma once

#include "helper.hxx"
#include "material.hxx"
#include "mesh.hxx"
#include "node.hxx"
#include "bone.hxx"
#include "animation.hxx"

#include "../uniform.hxx"

#include "../../external_library/glm.hxx"

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include <memory>
#include <map>
#include <unordered_map>
#include <functional>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      class model_type
        : public std::enable_shared_from_this< model_type >
      {
      public:
        using shared_type       = std::shared_ptr<       model_type >;
        using const_shared_type = std::shared_ptr< const model_type >;
        
      private:
        /// @brief アニメーションデータ
        shared_animation_map_type _shared_animation_map;
        
        /// @brief ボーン名->ボーンID
        shared_bone_map_type _shared_bone_map;
        
        /// @brief ボーンオフセット群
        shared_bone_offsets_type _shared_bone_offsets;
        
        /// @brief アニメーション名
        std::vector< std::string > _animation_names;
        
        /// @brief モデルデータに含まれるマテリアル群を格納
        shared_shared_materials_type _shared_shared_materials;
        
        /// @brief モデルデータに含まれるメッシュ群を格納
        shared_meshes_type _shared_meshes;
        
        /// @brief ノード
        node_type _node;
        
        /// @brief ルートノードの変形行列の逆行列（アニメーション適用で使用）
        glm::mat4 _global_inverse_transformation;
        
        /// @brief シャドウ生成対象か
        bool _shadow = true;
        
        inline auto apply_animation_recursive
        ( const animation_states_type& animation_states
        , const node_type& node
        , std::vector< glm::mat4 >& bone_animation_typeransformations
        , const glm::mat4&& parent_transformation
        )
         ->void
        {
          for ( auto& animation_state : animation_states )
          {
            glm::mat4 node_transformation( node.transformation() );
            
            if ( animation_state.name == "" )
            {
              //std::cerr << "warn: empty animation name.\n";
              continue;
            }
            
            auto animation_iterator = _shared_animation_map->find( animation_state.name );
            
            if( animation_iterator not_eq _shared_animation_map->end() )
              node_transformation = animation_iterator->second->transformation( node.name(), animation_state.time );
            else
              throw std::runtime_error( std::string( "animation_state.name " ) + animation_state.name + " is not found." );
            
            const auto global_transformation = parent_transformation * node_transformation;
            
            const auto shared_bone_map_iterator = _shared_bone_map->find( node.name() );
            if( shared_bone_map_iterator not_eq _shared_bone_map->end() )
            {
              const auto bone_index = shared_bone_map_iterator->second;
              bone_animation_typeransformations[ bone_index ]
                = _global_inverse_transformation * global_transformation * (*_shared_bone_offsets)[ bone_index ];
            }
            
            for ( const auto& child_node : node.nodes() )
              apply_animation_recursive
                ( animation_states
                , child_node
                , bone_animation_typeransformations
                , std::move( global_transformation )
                );
            
            // TODO: 複数のアニメーションの合成に対応する。現状: 最初の１つで break
            break;
          }
        }
        
        /// @brief アニメーション状態を適用
        inline auto apply_animation
        ( const animation_states_type& animation_states
        )
          ->void
        {
          if ( _shared_bone_offsets->empty() or animation_states.empty() )
            return;
          
          // update animation matrices
          
          // アニメーション最終変形行列
          std::vector< glm::mat4 > bone_animation_typeransformations;
          bone_animation_typeransformations.resize( _shared_bone_offsets->size(), glm::mat4( 1.0f ) );
          
          apply_animation_recursive( animation_states, _node, bone_animation_typeransformations, glm::mat4( 1.0f ) );
          
          const auto location_of_bones = get_uniform_location( "bones" );
          
          if ( location_of_bones not_eq -1 )
            glUniformMatrix4fv
              ( location_of_bones
              , bone_animation_typeransformations.size()
              , static_cast< GLboolean >( false )
              , &bone_animation_typeransformations[0][0][0]
              );
        }
        
      public:
        
        /// @brief const aiScene* からモデルデータ（メッシュ群、ノードグラフ）を生成
        explicit model_type
        ( const aiScene*     scene
        , const std::string& path_prefix    = ""
        , const bool         transpose_node = false
        )
          : _shared_animation_map   ( std::make_shared< animation_map_type >()    )
          , _shared_bone_map        ( std::make_shared< bone_map_type >()         )
          , _shared_bone_offsets    ( std::make_shared< bone_offsets_type >()     )
          , _shared_shared_materials( std::make_shared< shared_materials_type >() )
          , _shared_meshes          ( std::make_shared< meshes_type >()           )
          , _node
            ( scene->mRootNode
            , _shared_animation_map
            , transpose_node
            )
          , _global_inverse_transformation( glm::inverse( helper::to_glm_mat4( scene->mRootNode->mTransformation ) ) )
        {
          // シーンからマテリアル群を _materials に生成
          _shared_shared_materials->reserve( scene->mNumMaterials );
          
          for ( auto n = 0u; n < scene->mNumMaterials; ++n )
            _shared_shared_materials->emplace_back( std::make_shared< material_type >( scene->mMaterials[ n ], path_prefix ) );

          // シーンからメッシュ群を _meshes に生成
          _shared_meshes->reserve( scene->mNumMeshes );
          
          for ( auto n = 0u; n < scene->mNumMeshes; ++n )
            _shared_meshes->emplace_back
              ( scene->mMeshes[ n ]
              , _shared_shared_materials
              , _shared_bone_offsets
              , _shared_bone_map
              , _shared_animation_map
              );
          
          // アニメーション情報群を保存
          for ( auto n = 0u; n < scene->mNumAnimations; ++n )
          {
            const auto animation = scene->mAnimations[ n ];
            
            auto shared_animation = std::make_shared< animation_type >();
            
            shared_animation->duration         = animation->mDuration;
            shared_animation->ticks_per_second = animation->mTicksPerSecond == 0.0f ? 25.0f : animation->mTicksPerSecond;
            
            for ( auto n_channels = 0u; n_channels < animation->mNumChannels; ++n_channels )
            {
              const auto channel = animation->mChannels[ n_channels ];
              
              animation_type::channel_type ch;
              
              for ( auto n_key = 0u; n_key < channel->mNumScalingKeys; ++ n_key )
              {
                const auto key = channel->mScalingKeys[ n_key ];
                ch.scalings.emplace( std::move( key.mTime ), helper::to_glm_vec3( &key.mValue ) );
              }
              
              if ( ch.scalings.empty() )
                throw std::runtime_error( std::string("animation[") + animation->mName.C_Str() + "], bone[" + channel->mNodeName.C_Str() + "] scaling keyframes are empty." );
              
              for ( auto n_key = 0u; n_key < channel->mNumRotationKeys; ++ n_key )
              {
                const auto key = channel->mRotationKeys[ n_key ];
                ch.rotations.emplace( std::move( key.mTime ), helper::to_glm_quat( &key.mValue ) );
              }
              
              if ( ch.rotations.empty() )
                throw std::runtime_error( std::string("animation[") + animation->mName.C_Str() + "], bone[" + channel->mNodeName.C_Str() + "] rotations keyframes are empty." );
              
              for ( auto n_key = 0u; n_key < channel->mNumPositionKeys; ++ n_key )
              {
                const auto key = channel->mPositionKeys[ n_key ];
                ch.translations.emplace( std::move( key.mTime ), helper::to_glm_vec3( &key.mValue ) );
              }
              
              if ( ch.translations.empty() )
                throw std::runtime_error( std::string("animation[") + animation->mName.C_Str() + "], bone[" + channel->mNodeName.C_Str() + "] translations keyframes are empty." );
              
              shared_animation->channels.emplace( std::string( channel->mNodeName.C_Str() ), std::move( ch ) );
            }
            
            const std::string animation_name = animation->mName.C_Str();
            
            _animation_names.emplace_back( animation_name );
            
            _shared_animation_map->emplace
              ( std::move( animation_name )
              , std::move( shared_animation )
              );
          }
        }
        
        auto set_shadow( const bool shadow_ )
          -> void
        {
           _shadow = shadow_;
        }
        
        auto get_shadow() const
          -> bool
        {
          return _shadow;
        }
        
        /// @brief 描画
        auto draw()
         -> void
        {
          draw( animation_states_type() );
        }
        
        auto draw
        ( const animation_state_type& animation_state 
        )
         ->void
        {
          draw( animation_states_type{ animation_state } );
        }
        
        auto draw
        ( const animation_states_type& animation_states
        )
         ->void
        {
          apply_animation( animation_states );
          _node.draw( _shared_meshes );
        }
        
        /// @brief ボーンの名前のリストを提供
        auto get_bone_names() const
          -> std::vector< std::string >
        {
          std::vector< std::string > r;
          r.reserve( _shared_bone_map->size() );
          for ( const auto& m : *_shared_bone_map )
            r.emplace_back( m.first );
          return r;
        }
        
        /// @brief アニメーションの名前のリストを提供
        auto get_animation_names() const
          -> std::vector< std::string >
        {
          return _animation_names;
        }
        
        static constexpr unsigned int default_importer_readfile_flags
          = aiProcess_CalcTangentSpace
          //| aiProcess_JoinIdenticalVertices // 有効化すると結果的にボーン情報が削除される
          | aiProcess_GenNormals
          //| aiProcess_GenSmoothNormals // 有効化するとWebGL1でAssimpが落ちる
          //| aiProcess_PreTransformVertices // 有効化するとアニメーション関連情報が削除される
          | aiProcess_ValidateDataStructure
          | aiProcess_ImproveCacheLocality
          | aiProcess_RemoveRedundantMaterials
          | aiProcess_FixInfacingNormals
          //| aiProcess_SortByPType
          | aiProcess_FindInvalidData
          | aiProcess_GenUVCoords
          | aiProcess_FindInstances
          | aiProcess_OptimizeMeshes
          | aiProcess_OptimizeGraph
          | aiProcess_Triangulate
          ;
        
        /// @brief ファイルからモデルデータを生成
        static auto make_shared
        ( const std::string& file_path
        , unsigned int importer_readfile_flags = default_importer_readfile_flags
        )
          -> shared_type
        {
          // Assimp::Importer ctor
          //  http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html#a2c207299ed05f1db1ad1e6dab005f719
          //  ctorはパラメーター無し版とコピーctor版のみ。
          Assimp::Importer i;
          
          // Assimp::Importer::ReadFile
          //  http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html#a174418ab41d5b8bc51a044895cb991e5
          //  C文字列版、std::string版があり、ReadFileの他にメモリー上の任意のアドレスから読み込むReadFileMemoryもある。
          //  第二パラメーターで以下のポストプロセスのフラグを扱える。
          //    http://assimp.sourceforge.net/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410
          //      aiProcess_CalcTangentSpace        : 接線空間（タンジェント・バイタンジェント）を計算
          //      aiProcess_JoinIdenticalVertices   : 重複する頂点座標の削除
          //      aiProcess_MakeLeftHanded          : 左手座標系へ変換
          //      aiProcess_Triangulate             : 三角面へ変換
          //      aiProcess_RemoveComponent         : コンポーネント（アニメーション、材質、光源、カメラ、テクスチャー、頂点）を除去
          //      aiProcess_GenNormals              : 面法線を生成
          //      aiProcess_GenSmoothNormals        : スムーズ法線を頂点に生成
          //      aiProcess_SplitLargeMeshes        : 巨大なメッシュをサブメッシュに分割
          //      aiProcess_PreTransformVertices    : ノードグラフから変形前のローカル頂点座標と変形行列を除去（スケルタルアニメーション用データが消えます）
          //      aiProcess_LimitBoneWeights        : １つの頂点に影響するボーン数を制限
          //      aiProcess_ValidateDataStructure   : データ構造の整合性を確認
          //      aiProcess_ImproveCacheLocality    : 頂点キャッシュの局所性により三角形を最適化
          //      aiProcess_RemoveRedundantMaterials: 冗長または未使用の材質を除去
          //      aiProcess_FixInfacingNormals      : 面法線が内向きの場合に逆転させる
          //      aiProcess_SortByPType             : 異なる複数のプリミティブ型からなるメッシュをサブメッシュに分割
          //      aiProcess_FindDegenerates         : メッシュを線と点にする
          //      aiProcess_FindInvalidData         : 不正な法線や不正なUV座標を除去または修正する
          //      aiProcess_GenUVCoords             : 非UVマップ（球状または円筒状）からUVマップへ変換
          //      aiProcess_TransformUVCoords       : UV変形を適用する
          //      aiProcess_FindInstances           : 重複するメッシュを探し初出のメッシュに置き換える
          //      aiProcess_OptimizeMeshes          : メッシュ最適化を行う
          //      aiProcess_OptimizeGraph           : シーングラフ最適化を行う
          //      aiProcess_FlipUVs                 : UV座標を反転
          //      aiProcess_FlipWindingOrder        : 面生成を時計回りにする
          //      aiProcess_SplitByBoneCount        : ボーン数によりメッシュを分割
          //      aiProcess_Debone                  : ボーンをロスレスないし閾値まで除去
          //  このポストプロセスは ApplyPostProcessing() を後で呼んで行う事もできる。
          
          auto flags = importer_readfile_flags;
          auto transpose_node = false;
          
          {
            auto ext = file_path.substr( file_path.rfind('.') + 1 );
            if ( ext == "x" )
              flags |= aiProcess_FlipUVs;
            else if ( ext == "cob" || ext == "fbx")
              transpose_node = true;
          }
          
          auto scene = i.ReadFile( file_path, flags );
          
          if ( not scene )
            throw std::runtime_error( i.GetErrorString() );
          
          const auto r = std::make_shared< model_type >( scene, file_path.substr( 0, file_path.find_last_of( '/' ) ), transpose_node );
          
          i.FreeScene();
          
          return r;
        }
        
      };
      
      using shared_model_type = model_type::shared_type;
      
    }
  }
}
