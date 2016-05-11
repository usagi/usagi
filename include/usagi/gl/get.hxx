#pragma once

namespace usagi
{
  namespace gl
  {
    
    template< typename T = void >
    static inline auto get( const GLenum name )
      -> T
    { }
    
    template< >
    inline auto get< GLboolean >( const GLenum name )
      -> GLboolean
    {
      GLboolean value;
      glGetBooleanv( name, &value );
      return value;
    }
    
    template< >
    inline auto get< GLdouble >( const GLenum name )
      -> GLdouble
    {
      GLdouble value;
      glGetDoublev( name, &value );
      return value;
    }
    
    template< >
    inline auto get< GLfloat >( const GLenum name )
      -> GLfloat
    {
      GLfloat value;
      glGetFloatv( name, &value );
      return value;
    }
    
    template< >
    inline auto get< GLint >( const GLenum name )
      -> GLint
    {
      GLint value;
      glGetIntegerv( name, &value );
      return value;
    }
    
    template< >
    inline auto get< glm::f64vec4 >( const GLenum name )
      -> glm::f64vec4
    {
      glm::f64vec4 value;
      glGetDoublev( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f64vec3 >( const GLenum name )
      -> glm::f64vec3
    {
      glm::f64vec3 value;
      glGetDoublev( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f64vec2 >( const GLenum name )
      -> glm::f64vec2
    {
      glm::f64vec2 value;
      glGetDoublev( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f64vec1 >( const GLenum name )
      -> glm::f64vec1
    {
      glm::f64vec1 value;
      glGetDoublev( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f32vec4 >( const GLenum name )
      -> glm::f32vec4
    {
      glm::f32vec4 value;
      glGetFloatv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f32vec3 >( const GLenum name )
      -> glm::f32vec3
    {
      glm::f32vec3 value;
      glGetFloatv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f32vec2 >( const GLenum name )
      -> glm::f32vec2
    {
      glm::f32vec2 value;
      glGetFloatv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::f32vec1 >( const GLenum name )
      -> glm::f32vec1
    {
      glm::f32vec1 value;
      glGetFloatv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::i32vec4 >( const GLenum name )
      -> glm::i32vec4
    {
      glm::i32vec4 value;
      glGetIntegerv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::i32vec3 >( const GLenum name )
      -> glm::i32vec3
    {
      glm::i32vec3 value;
      glGetIntegerv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::i32vec2 >( const GLenum name )
      -> glm::i32vec2
    {
      glm::i32vec2 value;
      glGetIntegerv( name, &value[0] );
      return value;
    }
    
    template< >
    inline auto get< glm::i32vec1 >( const GLenum name )
      -> glm::i32vec1
    {
      glm::i32vec1 value;
      glGetIntegerv( name, &value[0] );
      return value;
    }
  }
}
