#pragma once

namespace usagi
{
  namespace iterator
  {
    template< typename ITERATOR >
    struct range_object_type
    {
      const ITERATOR a, b;
      auto begin() const { return a; }
      auto end() const { return b; }
    };
    
    template< typename ITERATOR >
    static auto make_range_object_type( const ITERATOR& a, const ITERATOR& b )
    { return range_object_type< ITERATOR >{ a, b }; }
    
    template< typename ITERATABLE >
    static auto make_range_object_type( const ITERATABLE& in )
    { return make_range_object_type( in.cbegin(), in.cend() ); }
    
  } 
}
