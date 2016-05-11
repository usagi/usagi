#pragma once

#include "../external_library/glm.hxx"
#include "sampler.hxx"

#include <boost/variant.hpp>
#include <boost/noncopyable.hpp>

#include <memory>
#include <cstdint>

namespace usagi
{
  namespace gl
  {
    
    using texture_source_data_type = boost::variant
    < std::vector< glm::f32vec4 >
    , std::vector< glm::f32vec3 >
    , std::vector< glm::f32vec2 >
    , std::vector< glm::f32vec1 >
    , std::vector< float >
    , std::vector< glm::u8vec4 >
    , std::vector< glm::u8vec3 >
    , std::vector< glm::u8vec2 >
    , std::vector< glm::u8vec1 >
    , std::vector< std::uint8_t >
    >;
    
    class texture_type
      : private boost::noncopyable
    {
      GLuint id = 0;
      GLenum type = GL_TEXTURE_2D;
      shared_sampler_type shared_sampler;
    public:
      
      texture_type()
        : shared_sampler( make_shared_sampler() )
      {
        glGenTextures( 1, &id );
      }
      
      ~texture_type()
      {
        glDeleteTextures( 1, &id );
      }
      
      auto get_id() { return id; }
      
      auto get_sampler_id() { return shared_sampler->get_id(); }
      
      auto bind( const GLuint texture_unit = 0 )
      {
        glBindTexture( type, id );
        shared_sampler->bind( texture_unit );
      }
      
      auto unbind( const GLuint texture_unit = 0 )
      {
        shared_sampler->unbind( texture_unit );
        glBindTexture( type, 0 );
      }
      
      auto generate_mipmap()
      {
        bind();
        glGenerateMipmap( type );
        unbind();
      }
      
      auto sampler_parameter( const GLenum name, const GLfloat parameter ) { shared_sampler->parameter( name, parameter ); }
      auto sampler_parameter( const GLenum name, const GLint   parameter ) { shared_sampler->parameter( name, parameter ); }
      
      auto sampler_parameter( const GLenum name, const GLfloat* parameters ) { shared_sampler->parameter( name, parameters ); }
      auto sampler_parameter( const GLenum name, const GLint*   parameters ) { shared_sampler->parameter( name, parameters ); }
      
      // f32vec4
      
      auto load_image( const glm::f32vec4* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RGBA ), width, 0, GL_RGBA, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec4* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RGBA ), width, height, 0, GL_RGBA, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec4* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RGBA ), width, height, depth, 0, GL_RGBA, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::f32vec4 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::f32vec4 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::f32vec4 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // f32vec3
      
      auto load_image( const glm::f32vec3* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RGB ), width, 0, GL_RGB, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec3* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RGB ), width, height, 0, GL_RGB, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec3* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RGB ), width, height, depth, 0, GL_RGB, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::f32vec3 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::f32vec3 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::f32vec3 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // f32vec2
      
      auto load_image( const glm::f32vec2* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RG ), width, 0, GL_RG, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec2* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RG ), width, height, 0, GL_RG, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec2* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RG ), width, height, depth, 0, GL_RG, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::f32vec2 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::f32vec2 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::f32vec2 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // f32vec1
      
      auto load_image( const glm::f32vec1* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RED ), width, 0, GL_RED, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec1* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RED ), width, height, 0, GL_RED, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::f32vec1* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RED ), width, height, depth, 0, GL_RED, GL_FLOAT, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::f32vec1 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::f32vec1 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::f32vec1 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // float
      
      auto load_image( const float* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RED ), width, 0, GL_RED, GL_FLOAT, data );
        unbind();
      }
      
      auto load_image( const float* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RED ), width, height, 0, GL_RED, GL_FLOAT, data );
        unbind();
      }
      
      auto load_image( const float* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RED ), width, height, depth, 0, GL_RED, GL_FLOAT, data );
        unbind();
      }
      
      auto load_image( const std::vector< float > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< float > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< float > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // u8vec4
      
      auto load_image( const glm::u8vec4* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RGBA ), width, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec4* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RGBA ), width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec4* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RGBA ), width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::u8vec4 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::u8vec4 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::u8vec4 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // u8vec3
      
      auto load_image( const glm::u8vec3* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RGB ), width, 0, GL_RGB, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec3* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RGB ), width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec3* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RGB ), width, height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::u8vec3 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::u8vec3 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::u8vec3 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // u8vec2
      
      auto load_image( const glm::u8vec2* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RG ), width, 0, GL_RG, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec2* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RG ), width, height, 0, GL_RG, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec2* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RG ), width, height, depth, 0, GL_RG, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::u8vec2 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::u8vec2 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::u8vec2 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // u8vec1
      
      auto load_image( const glm::u8vec1* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RED ), width, 0, GL_RED, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec1* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RED ), width, height, 0, GL_RED, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const glm::u8vec1* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RED ), width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, &(*data)[0] );
        unbind();
      }
      
      auto load_image( const std::vector< glm::u8vec1 > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< glm::u8vec1 > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< glm::u8vec1 > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      // std::uint8_t
      
      auto load_image( const std::uint8_t* data, const GLsizei width )
      {
        type = GL_TEXTURE_1D;
        bind();
        glTexImage1D( type, 0, static_cast< GLint >( GL_RED ), width, 0, GL_RED, GL_UNSIGNED_BYTE, data );
        unbind();
      }
      
      auto load_image( const std::uint8_t* data, const GLsizei width, const GLsizei height )
      {
        type = GL_TEXTURE_2D;
        bind();
        glTexImage2D( type, 0, static_cast< GLint >( GL_RED ), width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data );
        unbind();
      }
      
      auto load_image( const std::uint8_t* data, const GLsizei width, const GLsizei height, const GLsizei depth )
      {
        type = GL_TEXTURE_3D;
        bind();
        glTexImage3D( type, 0, static_cast< GLint >( GL_RED ), width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, data );
        unbind();
      }
      
      auto load_image( const std::vector< std::uint8_t > data, const GLsizei width )
      { load_image( &data[0], width ); }
      
      auto load_image( const std::vector< std::uint8_t > data, const GLsizei width, const GLsizei height )
      { load_image( &data[0], width, height ); }
      
      auto load_image( const std::vector< std::uint8_t > data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { load_image( &data[0], width, height, depth ); }
      
      auto load_image( const texture_source_data_type& data, const GLsizei width )
      { apply_visitor( [&]( const auto& data ){ this->load_image( data, width ); }, data ); }
      
      auto load_image( const texture_source_data_type& data, const GLsizei width, const GLsizei height )
      { apply_visitor( [&]( const auto& data ){ this->load_image( data, width, height ); }, data ); }
      
      auto load_image( const texture_source_data_type& data, const GLsizei width, const GLsizei height, const GLsizei depth )
      { apply_visitor( [&]( const auto& data ){ this->load_image( data, width, height, depth ); }, data ); }
      
    };
    
    using shared_texture_type = std::shared_ptr< texture_type >;
    
    static inline auto make_shared_texture()
      -> shared_texture_type
    {
      return std::make_shared< texture_type >();
    }
    
    static inline auto make_shared_texture( const texture_source_data_type& data, const GLsizei width )
      -> shared_texture_type
    {
      auto t = make_shared_texture();
      t->load_image( data, width );
      return t;
    }
    
    static inline auto make_shared_texture( const texture_source_data_type& data, const GLsizei width, const GLsizei height )
      -> shared_texture_type
    {
      auto t = make_shared_texture();
      t->load_image( data, width, height );
      return t;
    }
    
    static inline auto make_shared_texture( const texture_source_data_type& data, const GLsizei width, const GLsizei height, const GLsizei depth )
      -> shared_texture_type
    {
      auto t = make_shared_texture();
      t->load_image( data, width, height, depth );
      return t;
    }
    
  }
}
