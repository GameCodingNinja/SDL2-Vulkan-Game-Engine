
/************************************************************************
*    FILE NAME:       objectphysics3d.h
*
*    DESCRIPTION:     Class containing the 3D object's physics data
************************************************************************/

#pragma once

// Physical component dependency
#include <objectdata/iobjectphysicsdata.h>

// Game lib dependencies
#include <common/point.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
struct XMLNode;

class CObjectPhysicsData3D : public iObjectPhysicsData
{
public:

    // Constructor
    CObjectPhysicsData3D();
    virtual ~CObjectPhysicsData3D();

    // Load thes object data from node
    void loadFromNode( const XMLNode & objectNode ) override;
    
    // Get the name of the physics world
    const std::string & getWorld() const override;

    // Get the body type
    const std::string & getBodyShape() const override;

    // Get the mass of physics body
    float getMass() const override;

    // Get the damping
    float getLinearDamping() const override;
    float getAngularDamping() const override;

    // Get the friction
    float getFriction() const override;
    float getRollingFriction() const override;

    // Get the restitution - the amount of bounce
    float getRestitution() const override;

    // Get the collision filter
    short getCollisionGroup() const override;

    // Get the collision mask
    short getCollisionMask() const override;

    // Get the plane normal
    const CPoint<float> & getPlaneNormal() const override;

    // Specify if physics is active
    bool isActive() const override;

private:

    // The name of the physics world
    std::string m_world;
    
    // Type of physics body
    std::string m_bodyShape;

    // Plane normal - direction the plane is facing
    CPoint<float> m_planeNormal;

    // The mass of this shape
    float m_mass;

    // The friction is how much drag this object has on another object  
    float m_friction;

    // The amount of friction when this object rolls  
    float m_rollingFiction;

    // The percetange of velocity retained upon colliding with this object
    float m_restitution;

    // The constant decceleration of movement and rotation
    float m_linearDamping, m_angularDamping;

    // Collision group and mask
    short m_collisionGroup, m_collisionMask;
};
