
/************************************************************************
*    FILE NAME:       physicscomponent2d.cpp
*
*    DESCRIPTION:     Class for handling the physics part of the sprite
*                     Box2D works in meters
************************************************************************/

// Physical component dependency
#include <physics/physicscomponent2d.h>

// Game lib dependencies
#include <objectdata/iobjectdata.h>
#include <objectdata/iobjectphysicsdata.h>
#include <common/ivisualcomponent.h>
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <sprite/sprite.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Box2D lib dependencies
#include <Box2D/Box2D.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPhysicsComponent2D::CPhysicsComponent2D( const iObjectData & objectData ) :
    BODY_TYPE( objectData.getPhysicsData().getBodyType() ),
    m_pBody(nullptr),
    PIXELS_TO_METERS(0),
    METERS_TO_PIXELS(0),
    m_pWorld(nullptr)
{
    m_pWorld = &CPhysicsWorldManager2D::Instance().getWorld( objectData.getPhysicsData().getWorld() );

    // Re-init the constants to the values needed
    const_cast<float&>(METERS_TO_PIXELS) = m_pWorld->getPixelsPerMeter();
    const_cast<float&>(PIXELS_TO_METERS) = 1 / METERS_TO_PIXELS;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CPhysicsComponent2D::~CPhysicsComponent2D()
{
    // Destroy the physics
    destroy();
}


/************************************************************************
*    DESC:  Init the physics by creating the body and fixture
*           NOTE: Function must be called externally at the right time
*                 when the sprite has been setup with it's initial offsets
************************************************************************/
void CPhysicsComponent2D::init( const CSprite & sprite )
{
    if( sprite.getObjectData().getPhysicsData().isActive() )
    {
        createBody( sprite );
        createFixture( sprite );
    }
}

/************************************************************************
*    DESC:  Destroy the physics
************************************************************************/
void CPhysicsComponent2D::destroy()
{
    m_fixtureVec.clear();

    if( m_pBody != nullptr )
    {
        m_pWorld->destroyBody( m_pBody );
        m_pBody = nullptr;
    }
}


/************************************************************************
*    DESC:  Create the body
************************************************************************/
void CPhysicsComponent2D::createBody( const CSprite & sprite )
{
    const iObjectPhysicsData & physicsData = sprite.getObjectData().getPhysicsData();

    if( physicsData.getBodyType() != b2BodyType(-1) )
    {
        // Fill in the body structure
        b2BodyDef bodyDef;
        bodyDef.type = physicsData.getBodyType();
        bodyDef.linearDamping = physicsData.getLinearDamping();
        bodyDef.angularDamping = physicsData.getAngularDamping();
        bodyDef.fixedRotation = physicsData.isRotationFixed();
        bodyDef.position.Set( sprite.getPos().getX() * PIXELS_TO_METERS, sprite.getPos().getY() * PIXELS_TO_METERS );
        bodyDef.angle = -sprite.getRot().getZ();
        bodyDef.userData = (void*)&sprite;
        bodyDef.bullet = physicsData.isBullet();
        bodyDef.allowSleep = physicsData.allowSleep();

        // Create the body
        m_pBody = m_pWorld->createBody( bodyDef );
    }
}


/************************************************************************
*    DESC:  Create the fixture
************************************************************************/
void CPhysicsComponent2D::createFixture( const CSprite & sprite )
{
    const auto & fixture = sprite.getObjectData().getPhysicsData().getFixtureVec();

    for( auto & iter : fixture )
    {
        // Create the fixture
        if( iter.m_shape == b2Shape::e_circle )
            createCircularShapeFixture( sprite, iter );

        else if( iter.m_shape == b2Shape::e_edge )
            createEdgeShapeFixture( sprite, iter );

        else if( iter.m_shape == b2Shape::e_polygon )
            createPolygonShapeFixture( sprite, iter );

        else if( iter.m_shape == b2Shape::e_chain )
            createChainShapeFixture( sprite, iter );
    }
}


/************************************************************************
*    DESC:  Create the circular shape fixture
************************************************************************/
void CPhysicsComponent2D::createCircularShapeFixture( const CSprite & sprite, const CFixture & fixture )
{
    b2CircleShape shape;
    shape.m_radius = fixture.m_radius * sprite.getScale().getX() * PIXELS_TO_METERS;

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    auto pFixture = m_pBody->CreateFixture( &f );
    if( pFixture == nullptr )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
                boost::str( boost::format("Failed to create fixture (%s).\n\n%s\nLine: %s")
                    % sprite.getObjectData().getName() % __FUNCTION__ % __LINE__ ));

    pFixture->SetFilterData( fixture.m_filter );
    m_fixtureVec.push_back( pFixture );
}


/************************************************************************
*    DESC:  Create the edge shape fixture
*           NOTE: An edge is a line segment of two points
*                 This is no different then making a polygon from points
************************************************************************/
void CPhysicsComponent2D::createEdgeShapeFixture( const CSprite & sprite, const CFixture & fixture )
{
    // Do a sanity check because we need two points to define an edge
    if( fixture.m_vertVec.size() != 2 )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
            boost::str( boost::format("Physics object has incorrect number of points defined (%d/%s).\n\n%s\nLine: %s")
                % fixture.m_vertVec.size() % sprite.getObjectData().getName() % __FUNCTION__ % __LINE__ ));

    // Apply scale to the size and divide by 2
    // Object data holds size as int so need to convert it to a float
    const CSize<float> objectSize = sprite.getVisualComponent()->getSize();
    const CSize<float> scale( sprite.getScale().getX(), sprite.getScale().getY() );
    const CSize<float> size = objectSize * scale * 0.5f;

    // Convert the points to world location in meters
    std::vector<b2Vec2> pointVec;
    convertPoints( pointVec, fixture, size, scale );

    // These points must exist if this is to work. Use "at()" for bounds checking)
    b2EdgeShape shape;
    shape.Set( pointVec.at(0), pointVec.at(1) );

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    auto pFixture = m_pBody->CreateFixture( &f );
    if( pFixture == nullptr )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
                boost::str( boost::format("Failed to create fixture (%s).\n\n%s\nLine: %s")
                    % sprite.getObjectData().getName() % __FUNCTION__ % __LINE__ ));

    pFixture->SetFilterData( fixture.m_filter );
    m_fixtureVec.push_back( pFixture );
}


/************************************************************************
*    DESC:  Create the polygon shape fixture
************************************************************************/
void CPhysicsComponent2D::createPolygonShapeFixture( const CSprite & sprite, const CFixture & fixture )
{
    std::vector<b2Vec2> pointVec;

    // Apply scale to the size and divide by 2
    // Object data holds size as int so need to convert it to a float
    const CSize<float> objectSize = sprite.getVisualComponent()->getSize();
    const CSize<float> scale( sprite.getScale().getX(), sprite.getScale().getY() );
    const CSize<float> size = objectSize * scale * 0.5f;

    // Is this polygon shape defined by a vector of points?
    if( !fixture.m_vertVec.empty() )
    {
        // Convert the points to world location in meters
        convertPoints( pointVec, fixture, size, scale );
    }
	// If vector points are not supplied, build a square based on the object size
    else
    {
        pointVec.reserve( 4 );

        // Bottom and left mod have their signs flipped so that a positive mod always means
        // expansion of the side, and a negative mod always means a contraction of the side
        const float topMod = fixture.m_topMod * scale.h;
        const float bottomMod = -fixture.m_bottomMod * scale.h;
        const float leftMod = -fixture.m_leftMod * scale.w;
        const float rightMod = fixture.m_rightMod * scale.w;

        // Convert to meters
        // Box2D polygons are defined using Counter Clockwise Winding (CCW)
        pointVec.emplace_back(
            (-size.w + leftMod) * PIXELS_TO_METERS,
            (size.h + topMod) * PIXELS_TO_METERS );

        pointVec.emplace_back(
            (-size.w + leftMod) * PIXELS_TO_METERS,
            (-size.h + bottomMod) * PIXELS_TO_METERS );

        pointVec.emplace_back(
            (size.w + rightMod) * PIXELS_TO_METERS,
            (-size.h + bottomMod) * PIXELS_TO_METERS );

        pointVec.emplace_back(
            (size.w + rightMod) * PIXELS_TO_METERS,
            (size.h + topMod) * PIXELS_TO_METERS );
    }

    b2PolygonShape shape;
    shape.Set( pointVec.data(), pointVec.size() );

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    auto pFixture = m_pBody->CreateFixture( &f );
    if( pFixture == nullptr )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
                boost::str( boost::format("Failed to create fixture (%s).\n\n%s\nLine: %s")
                    % sprite.getObjectData().getName() % __FUNCTION__ % __LINE__ ));

    pFixture->SetFilterData( fixture.m_filter );
    m_fixtureVec.push_back( pFixture );
}


/************************************************************************
*    DESC:  Create the chain shape fixture
************************************************************************/
void CPhysicsComponent2D::createChainShapeFixture( const CSprite & sprite, const CFixture & fixture )
{
    // Do a sanity check because we need more then 1 point to define a chain
    if( fixture.m_vertVec.size() > 1 )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
            boost::str( boost::format("Physics object has incorrect number of points defined (%d/%s).\n\n%s\nLine: %s")
                % fixture.m_vertVec.size() % sprite.getObjectData().getName() % __FUNCTION__ % __LINE__ ));

    // Apply scale to the size and divide by 2
    // Object data holds size as int so need to convert it to a float
    const CSize<float> objectSize = sprite.getVisualComponent()->getSize();
    const CSize<float> scale( sprite.getScale().getX(), sprite.getScale().getY() );
    const CSize<float> size = objectSize * scale * 0.5f;

    // Convert the points to world location in meters
    std::vector<b2Vec2> pointVec;
    convertPoints( pointVec, fixture, size, scale );

    b2ChainShape shape;
    if( fixture.m_chainLoop )
        shape.CreateLoop( pointVec.data(), pointVec.size() );
    else
        shape.CreateChain( pointVec.data(), pointVec.size() );

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    auto pFixture = m_pBody->CreateFixture( &f );
    if( pFixture == nullptr )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
                boost::str( boost::format("Failed to create fixture (%s).\n\n%s\nLine: %s")
                    % sprite.getObjectData().getName() % __FUNCTION__ % __LINE__ ));

    pFixture->SetFilterData( fixture.m_filter );
    m_fixtureVec.push_back( pFixture );
}


/************************************************************************
*    DESC:  Convert the points to world location in meters
************************************************************************/
void CPhysicsComponent2D::convertPoints(
    std::vector<b2Vec2> & polyPointVec,
    const CFixture & fixture,
    const CSize<float> & size,
    const CSize<float> & scale )
{
    polyPointVec.reserve( fixture.m_vertVec.size() );

    // Convert to meters and world coordinates
    // Box2D polygons are defined using Counter Clockwise Winding (CCW)
    for( auto & iter : fixture.m_vertVec )
    {
        polyPointVec.emplace_back(
            ((iter.x * scale.w) - size.w) * PIXELS_TO_METERS,
            ((iter.y * scale.h) - size.h) * PIXELS_TO_METERS );
    }
}


/************************************************************************
*    DESC:  Update the physics
************************************************************************/
void CPhysicsComponent2D::update( CSprite * pSprite )
{
    if( m_pBody != nullptr )
    {
        CStatCounter::Instance().incPhysicsObjectsCounter();

        if( (BODY_TYPE > b2_staticBody) && m_pBody->IsAwake() )
        {
            const b2Vec2 & pos = m_pBody->GetPosition();
            const float angle = m_pBody->GetAngle();
            pSprite->setPos( pos.x * METERS_TO_PIXELS, pos.y * METERS_TO_PIXELS );
            pSprite->setRot( 0, 0, angle, false );
        }
    }
}


/************************************************************************
*    DESC:  Set the physics position and rotation
************************************************************************/
void CPhysicsComponent2D::setTransform( float x, float y, float angle, bool resetVelocity )
{
    if( m_pBody != nullptr )
    {
        m_pBody->SetTransform( b2Vec2( x * PIXELS_TO_METERS, y * PIXELS_TO_METERS ), angle );

        if( resetVelocity )
        {
            m_pBody->SetLinearVelocity( b2Vec2(0,0) );
            m_pBody->SetAngularVelocity( 0 );
        }
    }
}


/************************************************************************
*    DESC:  Set the linear velocity
************************************************************************/
void CPhysicsComponent2D::setLinearVelocity( float x, float y )
{
    if( m_pBody != nullptr )
        m_pBody->SetLinearVelocity( b2Vec2( x * PIXELS_TO_METERS, y * PIXELS_TO_METERS ) );
}


/************************************************************************
*    DESC:  Set the angular velocity
************************************************************************/
void CPhysicsComponent2D::setAngularVelocity( float value )
{
    if( m_pBody != nullptr )
        m_pBody->SetAngularVelocity( value );
}


/************************************************************************
*    DESC:  Set the angular impulse
************************************************************************/
void CPhysicsComponent2D::applyAngularImpulse( float value, bool wake )
{
    if( m_pBody != nullptr )
        m_pBody->ApplyAngularImpulse( value, wake );
}


/************************************************************************
*    DESC:  Get the body
************************************************************************/
b2Body * CPhysicsComponent2D::getBody()
{
    // Do a sanity check because we need more then 1 point to define a chain
    if( m_pBody == nullptr )
        throw NExcept::CCriticalException("Acess To Null Body!",
            boost::str( boost::format("Physics body in question is NULL.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    return m_pBody;
}


/************************************************************************
*    DESC:  Is body type
************************************************************************/
bool CPhysicsComponent2D::isBodyTypeStatic()
{
    return (BODY_TYPE == b2_staticBody);
}

bool CPhysicsComponent2D::isBodyTypeKinematic()
{
    return (BODY_TYPE == b2_kinematicBody);
}

bool CPhysicsComponent2D::isBodyTypeDynamic()
{
    return (BODY_TYPE == b2_dynamicBody);
}

/************************************************************************
*    DESC:  Set/get the active state of this body
************************************************************************/
void CPhysicsComponent2D::setActive(bool value)
{
    if( m_pBody != nullptr )
        m_pBody->SetActive(value);
}

bool CPhysicsComponent2D::isActive()
{
    if( m_pBody != nullptr )
        return m_pBody->IsActive();

    return false;
}


/************************************************************************
*    DESC:  Set/get the awake state of this body
************************************************************************/
void CPhysicsComponent2D::setAwake(bool value)
{
    if( m_pBody != nullptr )
        m_pBody->SetAwake(value);
}

bool CPhysicsComponent2D::isAwake()
{
    if( m_pBody == nullptr )
        return m_pBody->IsAwake();
    
    return false;
}


/************************************************************************
*    DESC:  Set/get the fixed rotation state of this body
************************************************************************/
void CPhysicsComponent2D::setFixedRotation(bool value)
{
    if( m_pBody != nullptr )
        m_pBody->SetFixedRotation(value);
}

bool CPhysicsComponent2D::isFixedRotation()
{
    if( m_pBody == nullptr )
        return m_pBody->IsFixedRotation();
    
    return false;
}


/************************************************************************
*    DESC:  Set/get the sleeping allowed state of this body
************************************************************************/
void CPhysicsComponent2D::setSleepingAllowed(bool value)
{
    if( m_pBody != nullptr )
        m_pBody->SetSleepingAllowed(value);
}

bool CPhysicsComponent2D::isSleepingAllowed()
{
    if( m_pBody == nullptr )
        return m_pBody->IsSleepingAllowed();
    
    return false;
}


/************************************************************************
*    DESC:  Set the same contact filtering to all the fixtures
************************************************************************/
void CPhysicsComponent2D::setContactFilter(uint16_t categoryBits, uint16_t maskBits, int16_t groupIndex, int fixtureIndex)
{
    if( m_pBody != nullptr && !m_fixtureVec.empty() )
    {
        b2Filter filter;
        filter.categoryBits = categoryBits;
        filter.maskBits = maskBits;
        filter.groupIndex = groupIndex;

        if( fixtureIndex < 0 )
        {
            for( auto iter : m_fixtureVec )
                iter->SetFilterData(filter);
        }
        else
        {
            if( fixtureIndex < static_cast<int>(m_fixtureVec.size()) )
                m_fixtureVec.at(fixtureIndex)->SetFilterData(filter);
            else
                NGenFunc::PostDebugMsg( "WARNING: Contact filtering out of range!" );
        }
    }
}
