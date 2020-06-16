
/************************************************************************
*    FILE NAME:       iphysicscomponent.h
*
*    DESCRIPTION:     iphysicscomponent Class
************************************************************************/

#ifndef __i_physics_component_h__
#define __i_physics_component_h__

// Game lib dependencies
#include <common/point.h>

// Forward Declarations
class CSprite;
class b2Body;

class iPhysicsComponent
{
public:

    // Constructor
    iPhysicsComponent();

    // Destructor
    virtual ~iPhysicsComponent();
    
    // Init the physics by creating the body and fixture
    // NOTE: Function must be called externally at the right time
    //       when the sprite has been setup with it's initial offsets
    virtual void init( const CSprite & sprite ){}
    
    // Update the physics
    virtual void update( CSprite * pSprite ){}
    
    // Is this component active?
    virtual bool isActive() { return false; }
    
    // Get the body
    virtual b2Body * getBody() { return nullptr; }
    
    // Set the physics position and rotation
    virtual void setTransform( float x, float y, float angle = 0, bool resetVelocity = true ){}
    
    // Set the linear velocity
    virtual void setLinearVelocity( float x, float y ){}
    
    // Set the linear velocity
    virtual void setAngularVelocity( float angle ){}
    
    // Set the angular impulse
    virtual void applyAngularImpulse( float value, bool wake = false ){}
    
    // Remove the body
    virtual void removeBody(){}

    // Add the body
    virtual void addBody(){}

    // Set the linear velocity
    virtual void setLinearVelocity(const CPoint<float> & vec){}

    // Set the angular velocity
    virtual void setAngularVelocity(const CPoint<float> & vec){}

    // Set the restitution
    virtual void setRestitution(const float rest){}
};

#endif
