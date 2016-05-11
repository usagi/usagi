#pragma once

#include "../external_library/glm.hxx"
#include "program.hxx"

// note: 先に boost/variant.hpp がロードされる場合はそれよりも前に定義される必要がある。
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40
#define BOOST_MPL_LIMIT_VECTOR_SIZE 40

#include <boost/variant.hpp>

namespace usagi
{
  namespace gl
  {
    
    using uniform_key_type = std::string;
    
    using uniform_value_type = boost::variant
    < glm::f32vec4, glm::f32vec3, glm::f32vec2, glm::f32vec1
    , float
    
    , std::vector< glm::f32vec4 >
    , std::vector< glm::f32vec3 >
    , std::vector< glm::f32vec2 >
    , std::vector< glm::f32vec1 >
    , std::vector< float >
    
    , glm::i32vec4, glm::i32vec3, glm::i32vec2, glm::i32vec1
    , std::int32_t
    , std::vector< glm::i32vec4 >
    , std::vector< glm::i32vec3 >
    , std::vector< glm::i32vec2 >
    , std::vector< glm::i32vec1 >
    , std::vector< std::int32_t >
    
    , glm::f32mat4x4
    , glm::f32mat4x3, glm::f32mat3x4
    , glm::f32mat4x2, glm::f32mat2x4
    , glm::f32mat3x3
    , glm::f32mat3x2, glm::f32mat2x3
    , glm::f32mat2x2
    
    , std::vector< glm::f32mat4x4 >
    , std::vector< glm::f32mat4x3 >
    , std::vector< glm::f32mat3x4 >
    , std::vector< glm::f32mat4x2 >
    , std::vector< glm::f32mat2x4 >
    , std::vector< glm::f32mat3x3 >
    , std::vector< glm::f32mat3x2 >
    , std::vector< glm::f32mat2x3 >
    , std::vector< glm::f32mat2x2 >
    
    >;
    
    static inline auto get_uniform_location( const uniform_key_type& key )
    {
      return glGetUniformLocation( get_current_program(), key.c_str() );
    }
    
    // f32
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32vec4& value )
    {
      glUniform4fv( get_uniform_location( key ), 1, &value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32vec3& value )
    {
      glUniform3fv( get_uniform_location( key ), 1, &value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32vec2& value )
    {
      glUniform2fv( get_uniform_location( key ), 1, &value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32vec1& value )
    {
      glUniform1f( get_uniform_location( key ), value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const float value )
    {
      glUniform1f( get_uniform_location( key ), value );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32vec4 >& value )
    {
      glUniform4fv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32vec3 >& value )
    {
      glUniform3fv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32vec2 >& value )
    {
      glUniform2fv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32vec1 >& value )
    {
      glUniform1fv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< float >& value )
    {
      glUniform1fv( get_uniform_location( key ), value.size(), &value[0] );
    }
    
    // i32
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::i32vec4& value )
    {
      glUniform4iv( get_uniform_location( key ), 1, &value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::i32vec3& value )
    {
      glUniform3iv( get_uniform_location( key ), 1, &value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::i32vec2& value )
    {
      glUniform2iv( get_uniform_location( key ), 1, &value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::i32vec1& value )
    {
      glUniform1i( get_uniform_location( key ), value[0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::int32_t value )
    {
      glUniform1i( get_uniform_location( key ), value );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::i32vec4 >& value )
    {
      glUniform4iv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::i32vec3 >& value )
    {
      glUniform3iv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::i32vec2 >& value )
    {
      glUniform2iv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::i32vec1 >& value )
    {
      glUniform1iv( get_uniform_location( key ), value.size(), &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< std::int32_t >& value )
    {
      glUniform1iv( get_uniform_location( key ), value.size(), &value[0] );
    }
    
    // mat
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat4x4& value, const GLboolean transpose )
    {
      glUniformMatrix4fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat4x3& value, const GLboolean transpose )
    {
      glUniformMatrix4x3fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat3x4& value, const GLboolean transpose )
    {
      glUniformMatrix3x4fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat4x2& value, const GLboolean transpose )
    {
      glUniformMatrix4x2fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat2x4& value, const GLboolean transpose )
    {
      glUniformMatrix2x4fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat3x3& value, const GLboolean transpose )
    {
      glUniformMatrix3fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat3x2& value, const GLboolean transpose )
    {
      glUniformMatrix3x2fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat2x3& value, const GLboolean transpose )
    {
      glUniformMatrix2x3fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat2x2& value, const GLboolean transpose )
    {
      glUniformMatrix2fv( get_uniform_location( key ), 1, transpose, &value[0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat4x4 >& value, const GLboolean transpose )
    {
      glUniformMatrix4fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat4x3 >& value, const GLboolean transpose )
    {
      glUniformMatrix4x3fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat3x4 >& value, const GLboolean transpose )
    {
      glUniformMatrix3x4fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat4x2 >& value, const GLboolean transpose )
    {
      glUniformMatrix4x2fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat2x4 >& value, const GLboolean transpose )
    {
      glUniformMatrix2x4fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat3x3 >& value, const GLboolean transpose )
    {
      glUniformMatrix3fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat3x2 >& value, const GLboolean transpose )
    {
      glUniformMatrix3x2fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat2x3 >& value, const GLboolean transpose )
    {
      glUniformMatrix2x3fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    static inline auto set_uniform( const uniform_key_type& key, const std::vector< glm::f32mat2x2 >& value, const GLboolean transpose )
    {
      glUniformMatrix2fv( get_uniform_location( key ), value.size(), transpose, &value[0][0][0] );
    }
    
    // omit transpose version
    
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat4x4& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat4x3& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat3x4& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat4x2& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat2x4& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat3x3& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat3x2& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat2x3& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type& key, const glm::f32mat2x2& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat4x4 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat4x3 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat3x4 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat4x2 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat2x4 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat3x3 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat3x2 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat2x3 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    static inline auto set_uniform( const uniform_key_type key, const std::vector< glm::f32mat2x2 >& value ) { set_uniform( key, value, static_cast< GLboolean >( false ) ); }
    
    // variant
    
    static inline auto set_uniform( const uniform_key_type& key, const uniform_value_type& value )
    {
      apply_visitor( [&]( const auto& value ) { set_uniform( key, value ); }, value );
    }
    
  }
}
