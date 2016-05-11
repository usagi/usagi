#pragma once

#include "helper.hxx"

#include "animation_state.hxx"

#include "../../external_library/glm.hxx"

#include <map>
#include <unordered_map>
#include <chrono>
#include <cmath>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      struct animation_type
      {
        
        struct channel_type
        {
          std::map< float, glm::vec3 > scalings;
          std::map< float, glm::quat > rotations;
          std::map< float, glm::vec3 > translations;
        };
        
        float duration;
        float ticks_per_second;
        
        // bone_name, channel
        std::unordered_map< std::string, channel_type > channels;
        
        auto transformation
        ( const std::string& bone_name
        , const animation_state_type::duration_type time
        ) const
          -> glm::mat4
        {
          if ( channels.find( bone_name ) == channels.end() )
            return glm::mat4( 1.0f );
          
          const auto time_in_typeicks = time.count() * ticks_per_second;
          const auto animation_time = std::fmod( time_in_typeicks, duration );
          
          const auto& channel = channels.at( bone_name );
          
          const auto scaling_matrix     = scaling    ( channel, animation_time );
          const auto rotation_matrix    = rotation   ( channel, animation_time );
          const auto translation_matrix = translation( channel, animation_time );
          
          return translation_matrix * rotation_matrix * scaling_matrix;
        }
        
      private:
        
        inline auto scaling
        ( const channel_type& channel
        , const float         animation_time
        ) const
          -> glm::mat4
        {
          const auto blended_scaling = blending_value( channel.scalings, animation_time );
          return glm::scale( glm::mat4( 1.0f ), blended_scaling );
        }
        
        inline auto rotation
        ( const channel_type& channel
        , const float         animation_time
        ) const
          -> glm::mat4
        {
          const auto blended_rotation = blending_value( channel.rotations, animation_time );
          return glm::mat4_cast( blended_rotation );
        }
        
        inline auto translation
        ( const channel_type& channel
        , const float         animation_time
        ) const
          -> glm::mat4
        {
          const auto blended_typeranslation = blending_value( channel.translations, animation_time );
          return glm::translate( glm::mat4( 1.0f ), blended_typeranslation );
        }
        
        template < class T >
        inline auto blending_value
        ( const T&    map
        , const float animation_time
        ) const
          -> typename T::mapped_type
        {
          if ( map.size() == 1)
            return map.begin() -> second;
          
          auto upper_bound = map.upper_bound( animation_time );
          
          float upper_typeime, lower_typeime;
          typename T::mapped_type upper_value, lower_value;
          
          if ( upper_bound not_eq map.end() )
          { 
            upper_value = upper_bound -> second;
            upper_typeime  = upper_bound -> first;
          }
          else
          {
            upper_value = map.begin() -> second;
            upper_typeime  = duration;
          }
          
          lower_value = ( --upper_bound ) -> second;
          lower_typeime  = upper_bound -> first;
          
          if ( upper_value == lower_value )
            return upper_value;
          
          const auto blending_factor = ( animation_time - lower_typeime ) / ( upper_typeime - lower_typeime );
          
          return blending_value_blend( lower_value, upper_value, blending_factor );
        }
        
        template < class T >
        inline auto blending_value_blend
        ( const T& lower_value
        , const T& upper_value
        , const float blending_factor
        ) const
          -> T
        {
          const auto reverse_blending_factor = 1.0f - blending_factor;
          
          const auto blending_lower_value = lower_value * reverse_blending_factor;
          const auto blending_upper_value = upper_value * blending_factor;
          
          return blending_lower_value + blending_upper_value;
        }
        
      };
      
      template <>
      inline auto animation_type::blending_value_blend< glm::quat >
      ( const glm::quat& lower_value
      , const glm::quat& upper_value
      , const float blending_factor
      ) const
        -> glm::quat
      {
        const auto reverse_blending_factor = 1.0f - blending_factor;
        
        const auto blending_lower_value = glm::mat4_cast( lower_value ) * reverse_blending_factor;
        const auto blending_upper_value = glm::mat4_cast( upper_value ) * blending_factor;
        
        return glm::quat_cast( blending_lower_value + blending_upper_value );
      }
      
      using shared_animation_type     = std::shared_ptr< animation_type >;
      using animation_map_type        = std::unordered_map< std::string, shared_animation_type >;
      using shared_animation_map_type = std::shared_ptr< animation_map_type >;
      
    }
  }
}