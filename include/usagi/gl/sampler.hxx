#pragma once

#include <boost/noncopyable.hpp>

#include <memory>

namespace usagi
{
  namespace gl
  {
    
    class sampler_type
      : private boost::noncopyable
    {
      GLuint id = 0;
    public:
      
      sampler_type()
      {
        glGenSamplers( 1, &id );
      }
      
      ~sampler_type()
      {
        glDeleteSamplers( 1, &id );
      }
      
      auto get_id() { return id; }
      
      auto bind( const GLuint texture_unit = 0 )
      {
        glBindSampler( texture_unit, id );
      }
      
      auto unbind( const GLuint texture_unit = 0 )
      {
        glBindSampler( texture_unit, 0 );
      }
      
      auto parameter( const GLenum name, const GLfloat parameter ) { glSamplerParameterf( id, name, parameter ); }
      auto parameter( const GLenum name, const GLint   parameter ) { glSamplerParameteri( id, name, parameter ); }
      
      auto parameter( const GLenum name, const GLfloat* parameters ) { glSamplerParameterfv( id, name, parameters ); }
      auto parameter( const GLenum name, const GLint*   parameters ) { glSamplerParameteriv( id, name, parameters ); }
    };
    
    using shared_sampler_type = std::shared_ptr< sampler_type >;
    
    static inline auto make_shared_sampler()
      -> shared_sampler_type
    {
      return std::make_shared< sampler_type >();
    }
    
  }
}
