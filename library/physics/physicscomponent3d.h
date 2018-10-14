
/************************************************************************
 *    FILE NAME:       physicscomponent3d.h
 *
 *    DESCRIPTION:     Class for handling the physics part of the sprite
 ************************************************************************/

#ifndef __physics_component_3d_h__
#define __physics_component_3d_h__

// Physical component dependency
#include <physics/iphysicscomponent.h>

// Game lib dependencies
#include <common/point.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CObjectData3D;
class CSprite;
class CPhysicsWorld3D;
class btRigidBody;
class asIScriptEngine;

class CPhysicsComponent3D : public iPhysicsComponent, boost::noncopyable
{
public:

    // Constructor
    CPhysicsComponent3D( const CObjectData3D & objectData );

    // Destructor
    ~CPhysicsComponent3D();

    //////////////////////////////////////////////////////
    // Register CScriptComponent2d Object with AngelScript
    //////////////////////////////////////////////////////
    //static void Register( asIScriptEngine * pEngine );

    // Init the physics by creating the body and fixture
    // NOTE: Function must be called externally at the right time
    //       when the sprite has been setup with it's initial offsets
    void init(const CSprite & sprite) override;

    // Remove the body
    void removeBody() override;

    // Add the body
    void addBody() override;

    // Set the linear velocity
    void setLinearVelocity(const CPoint<float> & vec) override;

    // Set the angular velocity
    void setAngularVelocity(const CPoint<float> & vec) override;

    // Set the restitution
    void setRestitution(const float rest) override;

    // Update the physics
    void update(CSprite * pSprite) override;

    // Is this component active?
    bool isActive() override;

private:

    // Pointer to the world
    // NOTE: Do not free. We don't own this pointer.
    CPhysicsWorld3D * m_pWorld;

    // Rigid body safe pointer
    std::unique_ptr<btRigidBody> m_upRigidBody;
};

#endif  // __physics_component_3d_h__


