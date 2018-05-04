
/************************************************************************
*    FILE NAME:       iphysicscomponent.h
*
*    DESCRIPTION:     iphysicscomponent Class
************************************************************************/

#ifndef __i_physics_component_h__
#define __i_physics_component_h__

class iPhysicsComponent
{
public:
    
    // Set the physics position and rotation
    virtual void setTransform( float x, float y, float angle = 0, bool resetVelocity = true ){}
    
    // Set the linear velocity
    virtual void setLinearVelocity( float x, float y ){}
    
    // Set the linear velocity
    virtual void setAngularVelocity( float angle ){}
    
    // Set the angular impulse
    virtual void applyAngularImpulse( float value, bool wake = false ){}
};

#endif
