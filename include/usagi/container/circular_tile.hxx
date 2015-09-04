#pragma once

#include "indices_2d.hxx"
#include "../math/circular_modulus.hxx"

#include <boost/circular_buffer.hpp>
#include <cstdint>
#include <functional>

namespace usagi
{
  namespace container
  {
    template
    < typename T >
    class circular_tile
      : boost::circular_buffer< boost::circular_buffer< T > >
    {
    public:
      using type = circular_tile<T>;
      
      using cols_type = boost::circular_buffer< T >;
      using rows_type = boost::circular_buffer< cols_type >;
      
      using operate_cols_functor_signature = auto ( cols_type& ) -> void;
      using operate_cols_functor_type = std::function< operate_cols_functor_signature >;
      
      using apply_functor_signature = auto ( T& ) -> void;
      using apply_functor_type = std::function< apply_functor_signature >;
      
      using on_arris_resize_functor_signature = auto ( const type& ref, std::size_t& new_size ) -> void;
      using on_arris_resize_functor_type = std::function< on_arris_resize_functor_signature >;

    protected:

      inline static auto make_cols( const std::size_t size, const T& default_value = T() )
      { return cols_type( size, default_value ); }

      auto make_cols( const T& default_value = T() )
      { return make_cols( rows_type::front().size(), default_value ); }
      
      template < typename DATA_TYPE = std::initializer_list<T> >
      auto make_cols( const DATA_TYPE& data )
      {
        cols_type cols( rows_type::front().size() );
        for ( const auto& datum : data )
          cols.push_back( datum );
        return cols;
      }

      template < typename VALUE_TYPE, typename MESSAGE_TYPE >
      inline static auto throw_if_zero( const VALUE_TYPE value, const MESSAGE_TYPE& message )
      {
        if ( value == 0 )
          throw std::logic_error( message );
      }

    public:

      on_arris_resize_functor_type on_cols_resize_before;
      on_arris_resize_functor_type on_cols_resize_after;
      
      on_arris_resize_functor_type on_rows_resize_before;
      on_arris_resize_functor_type on_rows_resize_after;

      circular_tile( const std::size_t cols_size, const std::size_t rows_size, const T& default_value = T() )
        : rows_type( rows_size, make_cols( cols_size, default_value ) )
        , on_cols_resize_before( get_nothing_functor_on_arris_resize() )
        , on_cols_resize_after ( get_nothing_functor_on_arris_resize() )
        , on_rows_resize_before( get_nothing_functor_on_arris_resize() )
        , on_rows_resize_after ( get_nothing_functor_on_arris_resize() )
      {
        throw_if_zero( cols_size, "cols_size == 0" );
        throw_if_zero( rows_size, "rows_size == 0" );
      }

      circular_tile( const std::size_t size, const T& default_value = T() )
        : circular_tile( size, size, default_value )
      { }

      static inline auto get_nothing_functor_on_arris_resize()
      { return []( const auto&, auto& ){ }; }
      
      auto cols_size() const { return this->front().size(); }
      auto rows_size() const { return rows_type::size(); }
      
      auto size() const { return cols_size() * rows_size(); }

      auto resize_rows( const std::size_t size, const T& default_value = T() )
      {
        on_rows_resize_before( *this, size );
        
        throw_if_zero( size, "the new rows size is 0; circular_tile cannot use 0 size rows." );
        rows_type::resize( size, make_cols( default_value ) );
        
        on_rows_resize_after( *this, size );
      }
      
      auto resize_cols( const std::size_t size, const T& default_value = T() )
      {
        on_cols_resize_before( *this, size );
        
        throw_if_zero( size, "the new cols size is 0; circular_tile cannot use 0 size cols." );
        for ( auto i = rows_type::begin(), e = rows_type::end(); i != e; ++i )
          i->resize( size, default_value );
        
        on_cols_resize_after( *this, size );
      }

      auto resize( const std::size_t cols_size, const std::size_t rows_size, const T& default_value = T() )
      {
        resize_rows( rows_size, default_value );
        resize_cols( cols_size, default_value );
      }

      auto begin() const -> decltype( rows_type::begin() ) { return rows_type::begin(); }
      auto end()   const -> decltype( rows_type::end() ){ return rows_type::end();   }
      
      auto get_center_index_x()
      { return cols_size() >> 1; }

      auto get_center_index_y()
      { return rows_size() >> 1; }

      auto get_center_indices()
      { return indices_2d{ get_center_index_x(), get_center_index_y() }; }

      template < typename INDEX_TYPE >
      decltype(auto) operator()( const INDEX_TYPE col, const INDEX_TYPE row )
      { return (*this)[ math::circular_modulus( row, rows_size() ) ][ math::circular_modulus( col, cols_size() ) ]; }

      template < typename INDICES_TYPE >
      decltype(auto) operator()( const INDICES_TYPE& indices )
      { return operator()( indices.x, indices.y ); }

      template < typename INDEX_TYPE >
      decltype(auto) from_center( const INDEX_TYPE delta_col, const INDEX_TYPE delta_row )
      { return operator()( get_center_index_x() + delta_col, get_center_index_y() + delta_row ); }

      template < typename INDICES_TYPE >
      decltype(auto) from_center( const INDICES_TYPE& delta_indices )
      { return operator()( get_center_index_x() + delta_indices.x, get_center_index_y() + delta_indices.y() ); }

      auto operate_cols( const operate_cols_functor_type& f )
      {
        for ( auto i = rows_type::begin(), e = rows_type::end(); i != e; ++i )
          f( *i );
      }

      auto apply( const apply_functor_type& f )
      {
        for ( auto ir = rows_type::begin(), er = rows_type::end(); ir != er; ++ir )
          for ( auto ic = ir->begin(), ec = ir->end(); ic != ec; ++ic )
            f( *ic );
      }

      auto shift_to_right( const T& default_value = T() )
      { operate_cols( [&]( auto& c ){ c.push_back( default_value ); } ); }
      
      auto shift_to_left( const T& default_value = T() )
      { operate_cols( [&]( auto& c ){ c.push_front( default_value ); } ); }
      
      auto shift_to_top( const T& default_value = T() )
      { rows_type::push_front( make_cols( default_value ) ); }
      
      auto shift_to_bottom( const T& default_value = T() )
      { rows_type::push_back( make_cols( default_value ) ); }

      auto throw_if_ne_rows_size( const std::size_t size )
      {
        if ( size != rows_size() )
          throw std::runtime_error( "size != rows_size()" );
      }
      
      auto throw_if_ne_cols_size( const std::size_t size )
      {
        if ( size != cols_size() )
          throw std::runtime_error( "size != cols_size()" );
      }

      template < typename DATA_TYPE = std::initializer_list<T> >
      auto shift_to_right( const DATA_TYPE& data )
      {
        throw_if_ne_rows_size( data.size() );
        
        auto i = data.begin();
        operate_cols( [&]( auto& c ){ c.push_back( *i++ ); } );
      }
      
      template < typename DATA_TYPE = std::initializer_list<T> >
      auto shift_to_left( const DATA_TYPE& data )
      {
        throw_if_ne_rows_size( data.size() );
        
        auto i = data.begin();
        operate_cols( [&]( auto& c ){ c.push_front( *i++ ); } );
      }
      
      template < typename DATA_TYPE = std::initializer_list<T> >
      auto shift_to_top( const DATA_TYPE& data )
      {
        throw_if_ne_cols_size( data.size() );

        rows_type::push_front( make_cols( data ) );
      }
      
      template < typename DATA_TYPE = std::initializer_list<T> >
      auto shift_to_bottom( const DATA_TYPE& data )
      {
        throw_if_ne_cols_size( data.size() );

        rows_type::push_back( make_cols( data ) );
      }
    };

    template < typename T >
    static decltype(auto) operator<<( std::ostream& o , const circular_tile<T>& tiles )
    {
      for ( const auto& cols : tiles )
      {
        for ( const auto& value : cols )
          o << value << " ";
        o << "\n";
      }

      return o;
    }
  }
}
