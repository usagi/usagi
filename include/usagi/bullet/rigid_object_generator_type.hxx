#pragma once

#include "rigid_object_type.hxx"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include <memory>
#include <chrono>
#include <vector>
#include <limits>

namespace usagi
{
  namespace bullet
  {
    
    class rigid_object_generator_type
      : public std::enable_shared_from_this< rigid_object_generator_type >
    {
      std::shared_ptr< btCollisionShape > shape;
      
      btVector3 position;
      btQuaternion rotation;
      btScalar mass = 0;
      btScalar restitution = 0;
      btVector3 inertia;
      
    public:
      
      using shared_type = std::shared_ptr< rigid_object_generator_type >;
      
      static auto sphere( const btScalar radius = 1 )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btSphereShape >( radius ) ) ); }
      
      static auto box( const btVector3 box_half_extents = btVector3{ 1, 1, 1 } )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btBoxShape >( box_half_extents ) ) ); }
      
      static auto capsule( const btScalar radius, const btScalar height )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btCapsuleShape >( radius, height ) ) ); }
      
      static auto cylinder( const btVector3 half_extents = btVector3{ 1, 1, 1 } )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btCylinderShape >( half_extents ) ) ); }
      
      static auto cone( const btScalar radius, const btScalar height )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btConeShape >( radius, height ) ) ); }
      
      static auto static_plane( const btVector3 normal, const btScalar height )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btStaticPlaneShape >( normal, height ) ) ); }
      
      static auto multi_sphere( const btVector3* positions, const btScalar* radiuses, const int number_of_spheres )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btMultiSphereShape >( positions, radiuses, number_of_spheres ) ) ); }
      
      template
      < typename POSITIONS_TYPE = std::vector< btVector3 >
      , typename RADIUSES_TYPE  = std::vector< btScalar  >
      >
      static auto multi_sphere
      ( const POSITIONS_TYPE& positions
      , const RADIUSES_TYPE& radiuses
      )
      {
        if ( positions.size() != radiuses.size() )
          throw std::runtime_error( "rigid_object_generator_type::multi_sphere: positions.size() != radiuses.size()" );
        
        if ( positions.size() > std::numeric_limits< int >::max() )
          throw std::runtime_error( "rigid_object_generator_type::multi_sphere: positions.size() > std::numeric_limits< int >::max()" );
        
        return multi_sphere( positions.data(), radiuses.data(), static_cast< int >( positions.size() ) );
      }
      
      static auto convex_triangle_mesh( btStridingMeshInterface* mesh_interface, const bool calc_aabb = true )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btConvexTriangleMeshShape >( mesh_interface, calc_aabb ) ) ); }
      
      template
      < typename SHARED_VERTICES_TYPE = std::shared_ptr< std::vector< btVector3 > >
      , typename SHARED_INDICES_TYPE  = std::shared_ptr< std::vector< int       > >
      >
      static auto convex_triangle_mesh
      ( const SHARED_VERTICES_TYPE& shared_vertices
      , const SHARED_INDICES_TYPE&  shared_indices
      , const int vertex_stride = sizeof( SHARED_VERTICES_TYPE::element_type::element_type )
      , const int index_stride  = sizeof( SHARED_INDICES_TYPE ::element_type::element_type )
      , const bool calc_aabb = true
      )
      {
        auto mesh_interface = btTriangleIndexVertexArray
        ( static_cast< int >( shared_indices->size() )
        , shared_vertices->data()
        , index_stride
        , static_cast< int >( shared_vertices->size() )
        , reinterpret_cast< const btScalar* >( shared_vertices->data() )
        , vertex_stride
        );
        return convex_triangle_mesh( &mesh_interface, calc_aabb );
      }
      
      static auto convex_bvh_triangle_mesh( btStridingMeshInterface* mesh_interface, const bool calc_aabb = true )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btBvhTriangleMeshShape >( mesh_interface, calc_aabb ) ) ); }
      
      template
      < typename SHARED_VERTICES_TYPE = std::shared_ptr< std::vector< btVector3 > >
      , typename SHARED_INDICES_TYPE  = std::shared_ptr< std::vector< int       > >
      >
      static auto convex_bvh_triangle_mesh
      ( const SHARED_VERTICES_TYPE& shared_vertices
      , const SHARED_INDICES_TYPE&  shared_indices
      , const int vertex_stride = sizeof( SHARED_VERTICES_TYPE::element_type::element_type )
      , const int index_stride  = sizeof( SHARED_INDICES_TYPE ::element_type::element_type )
      , const bool calc_aabb = true
      )
      {
        auto mesh_interface = btTriangleIndexVertexArray
        ( static_cast< int >( shared_indices->size() )
        , shared_vertices->data()
        , index_stride
        , static_cast< int >( shared_vertices->size() )
        , reinterpret_cast< const btScalar* >( shared_vertices->data() )
        , vertex_stride
        );
        return convex_bvh_triangle_mesh( &mesh_interface, calc_aabb );
      }
      
      static auto convex_hull
      ( const btScalar* points = nullptr
      , const int number_of_points = 0
      , const int stride = sizeof( btVector3 )
      )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btConvexHullShape >( points, number_of_points, stride ) ) ); }
      
      template < typename POINTS_TYPE = std::vector< btVector3 > >
      static auto convex_hull
      ( const POINTS_TYPE& points )
      {
        return convex_hull( reinterpret_cast< const btScalar* >( points.data() ), points.size(), sizeof( POINTS_TYPE::element_type ) );
      }
      
      static auto uniform_scaling
      ( btConvexShape* convex_shape, const btScalar scaling )
      { return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btUniformScalingShape >( convex_shape, scaling ) ) ); }
      
      static auto uniform_scaling
      ( const shared_rigid_object_type& shared_rigid_object, const btScalar scaling )
      {
        return uniform_scaling( std::dynamic_pointer_cast< btConvexShape >( shared_rigid_object->collision_shape ).get(), scaling );
      }
      
      static auto heightfield_terrain
      ( int height_stick_width
      , int height_stick_length
      , const void* heightfield_data
      , btScalar height_scale
      , btScalar min_height
      , btScalar max_height
      , int up_axis
      , PHY_ScalarType height_data_type
      , bool flip_quad_edges
      )
      {
        return std::make_shared< rigid_object_generator_type >
        ( rigid_object_generator_type
          ( std::make_shared< btHeightfieldTerrainShape >
            ( height_stick_width
            , height_stick_length
            , heightfield_data
            , height_scale
            , min_height
            , max_height
            , up_axis
            , height_data_type
            , flip_quad_edges
            )
          )
        );
      }
      
      static auto heightfield_terrain
      ( int height_stick_width
      , int height_stick_length
      , const void* heightfield_data
      , btScalar max_height
      , int up_axis
      , bool use_float_data
      , bool flip_quad_edges
      )
      {
        return std::make_shared< rigid_object_generator_type >
        ( rigid_object_generator_type
          ( std::make_shared< btHeightfieldTerrainShape >
            ( height_stick_width
            , height_stick_length
            , heightfield_data
            , max_height
            , up_axis
            , use_float_data
            , flip_quad_edges
            )
          )
        );
      }
      
      static auto compound
      ( const bool enable_dynamic_aabb_tree = true
      , const int initial_child_capacity = 0
      )
      {
        return std::make_shared< rigid_object_generator_type >( rigid_object_generator_type( std::make_shared< btCompoundShape >( enable_dynamic_aabb_tree, initial_child_capacity ) ) );
      }
      
      rigid_object_generator_type
      ( const std::shared_ptr< btCollisionShape >& shape_ = std::make_shared< btSphereShape >( 1 )
      )
        : shape( shape_ )
        , position( 0, 0, 0 )
        , rotation( 0, 0, 0, 1 )
        , inertia( 0, 0, 0 )
      { }
      
      decltype( auto ) set_shape( const std::shared_ptr< btCollisionShape >& in = std::make_shared< btSphereShape >( 1 ) )
      {
        shape = in;
        return shared_from_this();
      }
      
      decltype( auto ) add_child_shape( const std::shared_ptr< btCollisionShape >& in, const btTransform& transform )
      {
        auto s = std::dynamic_pointer_cast< btCompoundShape >( shape );
        
        if ( not s )
          throw std::runtime_error( "rigid_object_generator_type::add_child_shape: current shape is not a btCompoundShape." );
        
        s->addChildShape( transform, in.get() );
        
        return shared_from_this();
      }
      
      decltype( auto ) remove_child_shape( const std::shared_ptr< btCollisionShape >& in )
      {
        auto s = std::dynamic_pointer_cast< btCompoundShape >( shape );
        
        if ( not s )
          throw std::runtime_error( "rigid_object_generator_type::add_child_shape: current shape is not a btCompoundShape." );
        
        s->removeChildShape( in.get() );
        
        return shared_from_this();
      }
      
      decltype( auto ) recalculate_local_aabb()
      {
        auto s = std::dynamic_pointer_cast< btCompoundShape >( shape );
        
        if ( not s )
          throw std::runtime_error( "rigid_object_generator_type::add_child_shape: current shape is not a btCompoundShape." );
        
        s->recalculateLocalAabb();
        
        return shared_from_this();
      }
      
      template < typename T = btVector3 >
      decltype( auto ) set_position( const T& in = T{ 0, 0, 0 } )
      {
        position[0] = in[0];
        position[1] = in[1];
        position[2] = in[2];
        return shared_from_this();
      }
      
      template < typename T = btQuaternion >
      decltype( auto ) set_rotation( const T& in = T{ 0, 0, 0, 1 } )
      {
        rotation[0] = in[0];
        rotation[1] = in[1];
        rotation[2] = in[2];
        rotation[3] = in[3];
        return shared_from_this();
      }
      
      decltype( auto ) set_mass( const btScalar in )
      {
        mass = in;
        return shared_from_this();
      }
      
      decltype( auto ) set_restitution( const btScalar in )
      {
        restitution = in;
        return shared_from_this();
      }
      
      template < typename T = btVector3 >
      decltype( auto ) set_inertia( const T& in = T{ 0, 0, 0 } )
      {
        inertia[0] = in[0];
        inertia[1] = in[1];
        inertia[2] = in[2];
        return shared_from_this();
      }
      
      template < typename T = rigid_object_type >
      auto generate()
      {
        if ( not shape )
          set_shape();
        
        auto o = T::make_shared();
        
        o->initialize
        ( shape
        , position
        , rotation
        , mass
        , restitution
        , inertia
        );
        
        return o;
      }
      
      static auto make_shared()
      { return std::make_shared< rigid_object_generator_type >(); }
      
    };
    
    using shared_rigid_object_generator_type = rigid_object_generator_type::shared_type;
    
  }
}