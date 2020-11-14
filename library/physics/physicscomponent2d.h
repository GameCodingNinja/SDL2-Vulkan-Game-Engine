
/************************************************************************
*    FILE NAME:       physicscomponent2d.h
*
*    DESCRIPTION:     Class for handling the physics part of the sprite
************************************************************************/

#pragma once

// Physical component dependency
#include <physics/iphysicscomponent.h>

// Game lib dependencies
#include <common/size.h>

// Box2D lib dependencies
#include <Box2D/Dynamics/b2Body.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class iObjectData;
class CSprite;
class CPhysicsWorld2D;
class b2Body;
class CFixture;
class b2Fixture;

class CPhysicsComponent2D : public iPhysicsComponent, boost::noncopyable
{
public:

    // Constructor
    CPhysicsComponent2D( const iObjectData & objectData );

    // Destructor
    virtual ~CPhysicsComponent2D();

    // Init the physics by creating the body and fixture
    // NOTE: Function must be called externally at the right time
    //       when the sprite has been setup with it's initial offsets
    void init( const CSprite & sprite ) override;

    // Destroy the physics
    void destroy() override;

    // Update the physics
    void update( CSprite * pSprite ) override;
    
    // Get the body
    b2Body * getBody() override;

    // Set the physics position and rotation
    void setTransform( float x, float y, float angle = 0, bool resetVelocity = true ) override;
    
    // Set the linear velocity
    void setLinearVelocity( float x, float y ) override;
    
    // Set the linear velocity
    void setAngularVelocity( float angle ) override;
    
    // Set the angular impulse
    void applyAngularImpulse( float value, bool wake = false ) override;

    // Is body type
    bool isBodyTypeStatic() override;
    bool isBodyTypeKinematic() override;
    bool isBodyTypeDynamic() override;

    // Set/get the active state of this body
    void setActive(bool value) override;
    bool isActive() override;

    // Set/get the awake state of this body
    void setAwake(bool value) override;
    bool isAwake() override;

    // Set/get the fixed rotation state of this body
    void setFixedRotation(bool value) override;
    bool isFixedRotation() override;

    // Set/get the sleeping allowed state of this body
    void setSleepingAllowed(bool value) override;
    bool isSleepingAllowed() override;

    // Set the same contact filtering to all the fixtures
    void setContactFilter(uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF, int16_t groupIndex = 0, int fixtureIndex = -1) override;

private:

    // Create the body
    void createBody( const CSprite & sprite );

    // Create the fixture
    void createFixture( const CSprite & sprite );

    // Create the circular shape fixture
    void createCircularShapeFixture( const CSprite & sprite, const CFixture & fixture );

    // Create the edge shape fixture
    void createEdgeShapeFixture( const CSprite & sprite, const CFixture & fixture );

    // Create the polygon shape fixture
    void createPolygonShapeFixture( const CSprite & sprite, const CFixture & fixture );

    // Create the chain shape fixture
    void createChainShapeFixture( const CSprite & sprite, const CFixture & fixture );
    
    // Convert the points to world location in meters
    void convertPoints(
        std::vector<b2Vec2> & polyPointVec,
        const CFixture & fixture,
        const CSize<float> & size,
        const CSize<float> & scale );

private:
    
    // Body type
    const b2BodyType BODY_TYPE;
    
    // The physics body the sprite belongs to
    // NOTE: If we own this pointer, we'll need to free it
    b2Body * m_pBody = nullptr;

    // vector list of fixtures
    // NOTE: If we DON't own this pointer!
    std::vector<b2Fixture *> m_fixtureVec;

    // Pixels to meters conversion
    const float PIXELS_TO_METERS;
    const float METERS_TO_PIXELS;

    // Pointer to the world
    // NOTE: Do not free. We don't own this pointer.
    CPhysicsWorld2D * m_pWorld;
};
