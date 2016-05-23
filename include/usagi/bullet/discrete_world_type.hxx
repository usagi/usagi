#pragma once

#include "rigid_object_type.hxx"

#include <memory>
#include <set>
#include <chrono>
#include <vector>

namespace usagi
{
  namespace bullet
  {
    
    class discrete_world_type
      : public std::enable_shared_from_this< discrete_world_type >
    {
      std::unique_ptr< btBroadphaseInterface               > broad_phase;
      std::unique_ptr< btSequentialImpulseConstraintSolver > solver;
      std::unique_ptr< btDefaultCollisionConfiguration     > collision_configuration;
      std::unique_ptr< btCollisionDispatcher               > dispatcher;
      std::unique_ptr< btDiscreteDynamicsWorld             > world;
      
      std::set< shared_rigid_object_type > objects;
      
    public:
      
      using shared_type = std::shared_ptr< discrete_world_type >;
      
      /// @note http://bulletphysics.org/mediawiki-1.5.8/index.php/binary_serialization
      auto serialize() const
        -> std::vector< std::uint8_t >
      {
        btDefaultSerializer serializer;
        
        world->serialize( &serializer );
        
        const auto begin = serializer.getBufferPointer();
        const auto end   = begin + serializer.getCurrentBufferSize();
        
        return { begin, end };
      }
      
      decltype( auto ) initialize( std::unique_ptr< btBroadphaseInterface > broad_phase_ = std::make_unique< btDbvtBroadphase >() )
      {
        try { shared_from_this(); }
        catch ( const std::bad_weak_ptr& ) { throw std::logic_error( "You must change to use discrete_world_type::make_shared() for a construction." ); }
        
        broad_phase = std::move( broad_phase_ );
        
        solver = std::make_unique< btSequentialImpulseConstraintSolver >();
        
        btDefaultCollisionConstructionInfo collision_construction_info;
        collision_configuration = std::make_unique< btDefaultCollisionConfiguration >( collision_construction_info );
        
        dispatcher = std::make_unique< btCollisionDispatcher >( collision_configuration.get() );
        
        world      = std::make_unique< btDiscreteDynamicsWorld >
                    ( dispatcher.get()
                    , broad_phase.get()
                    , solver.get()
                    , collision_configuration.get()
                    );
        
        return shared_from_this();
      }
      
      decltype( auto ) set_gravity( const btVector3& in = btVector3{ 0.0, -9.80665, 0.0 } )
      {
        world->setGravity( in );
        return shared_from_this();
      }
      
      decltype( auto ) add( const shared_rigid_object_type& in )
      {
        if ( objects.emplace( in ).second )
          world->addRigidBody( in->rigid_body.get() );
        return shared_from_this();
      }
      
      decltype( auto ) remove( const shared_rigid_object_type& in )
      {
        if ( objects.erase( in ) != 0 )
          world->removeRigidBody( in->rigid_body.get() );
        return shared_from_this();
      }
      
      auto step
      ( const std::chrono::duration< btScalar >& duration        = std::chrono::milliseconds( 1 )
      , const int sub_step = 1
      , const std::chrono::duration< btScalar >& fixed_time_step = std::chrono::milliseconds( 1 )
      )
      {
        world->stepSimulation
        ( duration.count()
        , sub_step
        , fixed_time_step.count()
        );
      }
      
      static auto make_shared()
      {
        return std::make_shared< discrete_world_type >();
      }
      
    };
    
    using shared_discrete_world_type = discrete_world_type::shared_type;
    
  }
}