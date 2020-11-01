
/************************************************************************
*    FILE NAME:       physicscomponent2d.h
*
*    DESCRIPTION:     Class for handling the physics part of the sprite
************************************************************************/

#ifndef __physics_component_2d_h__
#define __physics_component_2d_h__

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

    // Is this component active?
    bool isActive() override;
    
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

    // Pixels to meters conversion
    const float PIXELS_TO_METERS;
    const float METERS_TO_PIXELS;

    // Pointer to the world
    // NOTE: Do not free. We don't own this pointer.
    CPhysicsWorld2D * m_pWorld;
};

#endif
