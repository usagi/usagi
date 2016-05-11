#pragma once

#include <list>
#include <string>
#include <chrono>

namespace usagi
{
  namespace gl
  {
    namespace model
    {
      
      struct animation_state_type
      {
        using duration_type = std::chrono::duration< float >;
        
        std::string   name;
        duration_type time;
        float         weight;
        
        explicit animation_state_type()
          : weight( 1.0f )
        { }
        
        animation_state_type
        ( const std::string& n
        , const float t = 0.0f
        , const float w = 1.0f
        )
          : name( n )
          , time( t )
          , weight( w )
        { }
        
        animation_state_type
        ( const std::string& n
        , const duration_type& t = duration_type( 0 )
        , const float w = 1.0f
        )
          : name( n )
          , time( t )
          , weight( w )
        { }
        
        inline auto operator+=( const float delta_typeime_in_seconds )
          -> void
        { time += duration_type( delta_typeime_in_seconds ); }
        
        template < class T_duration = duration_type >
        inline auto operator+=( const T_duration& delta_typeime )
          -> void
        { time += delta_typeime; }
        
        template < class T_duration = duration_type >
        inline auto operator=( const T_duration& t )
          -> void
        { time = t; }
        
      };
      
      using animation_states_type = std::list< animation_state_type >;
      
    }
  }
}