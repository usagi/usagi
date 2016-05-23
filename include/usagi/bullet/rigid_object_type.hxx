#pragma once

#include <btBulletDynamicsCommon.h>

#include <memory>
#include <chrono>
#include <vector>

namespace usagi
{
  namespace bullet
  {
    
    class rigid_object_type
      : public std::enable_shared_from_this< rigid_object_type >
    {
      friend class discrete_world_type;
      friend class rigid_object_generator_type;
      
      std::shared_ptr< btCollisionShape     > collision_shape;
      std::unique_ptr< btDefaultMotionState > motion_state;
      std::unique_ptr< btRigidBody          > rigid_body;
      
    public:
      
      auto serialize() const
        -> std::vector< std::uint8_t >
      {
        btDefaultSerializer serializer;
        
        serializer.startSerialization();
        collision_shape->serializeSingleShape( &serializer );
        serializer.finishSerialization();
        
        const auto begin = serializer.getBufferPointer();
        const auto end   = begin + serializer.getCurrentBufferSize();
        
        return { begin, end };
      }
      
      auto get_position() const
      {
        return rigid_body->getCenterOfMassPosition();
      }
      
      auto initialize
      ( const std::shared_ptr< btCollisionShape > shape = std::make_shared< btSphereShape >( 1 )
      , const btVector3& position = btVector3( 0, 0, 0 )
      , const btQuaternion& rotation = btQuaternion( 0, 0, 0, 1 )
      , const btScalar mass = 0
      , const btScalar restitution = 0
      , btVector3 inertia = btVector3( 0, 0, 0 )
      )
      {
        try { shared_from_this(); }
        catch ( const std::bad_weak_ptr& ) { throw std::logic_error( "You must change to use rigid_object_type::make_shared() for a construction." ); }
        
        collision_shape = shape;
        
        motion_state = std::make_unique< btDefaultMotionState >( btTransform( rotation, position ) );
        
        collision_shape->calculateLocalInertia( mass, inertia );
        
        btRigidBody::btRigidBodyConstructionInfo rigid_body_construction_info
        ( mass, motion_state.get()
        , collision_shape.get()
        , inertia
        );
        
        rigid_body_construction_info.m_restitution = restitution;
        rigid_body = std::make_unique< btRigidBody >( rigid_body_construction_info );
      }
      
      static auto make_shared() { return std::make_shared< rigid_object_type >(); }
    };

    using shared_rigid_object_type = std::shared_ptr< rigid_object_type >;
    
  }
}