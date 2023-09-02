
/************************************************************************
*    FILE NAME:       iphysicscomponent.h
*
*    DESCRIPTION:     iphysicscomponent Class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/point.h>

// Standard lib dependencies
#include <cstdint>

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

    // Destroy the physics
    virtual void destroy(){}
    
    // Update the physics
    virtual void update( CSprite * pSprite ){}
    
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

    // Is body type
    virtual bool isBodyTypeStatic(){return false;}
    virtual bool isBodyTypeKinematic(){return false;}
    virtual bool isBodyTypeDynamic(){return false;}

    // Set/get the active state of this body
    virtual void setActive(bool value) {}
    virtual bool isActive() { return false; }

    // Set/get the awake state of this body
    virtual void setAwake(bool value){}
    virtual bool isAwake(){ return false; }

    // Set/get the fixed rotation state of this body
    virtual void setFixedRotation(bool value) {}
    virtual bool isFixedRotation() { return false; }

    // Set/get the sleeping allowed state of this body
    virtual void setSleepingAllowed(bool value){}
    virtual bool isSleepingAllowed(){ return false; }

    // Set the same contact filtering to all the fixtures
    virtual void setContactFilter(uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF, int16_t groupIndex = 0, int fixtureIndex = -1){}
};
