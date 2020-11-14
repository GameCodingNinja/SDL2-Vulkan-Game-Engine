
/************************************************************************
*    FILE NAME:       iobjectphysicsdata.h
*
*    DESCRIPTION:     Object physics Data interface class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/point.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Box2D lib dependencies
#include <Box2D/Box2D.h>

// Forward Declarations
struct XMLNode;

class CFixture
{
public:

    // Shape of the fixture
    b2Shape::Type m_shape = b2Shape::Type(-1);

    // Radius if shape is a circle
    float m_radius = 0.0f;

    // The friction is how much drag this object has on another object  
    float m_friction = 0.2f;

    // The density is how much the object resists movement  
    float m_density = 0.2f;

    // The percetange of velocity retained upon colliding with this object
    float m_restitution = 0.2f;

    // Amount to adjust the top, bottom, left, and right side size of the mesh
    float m_topMod = 0, m_bottomMod = 0, m_leftMod = 0, m_rightMod = 0;
    
    // Flag to define if chain shape is a loop
    bool m_chainLoop = false;
    
    // Flag to indicate if fixture is a sensor. Reports collision but doesn't react to it
    bool m_sensor = false;
    
    // Collision filter
    b2Filter m_filter;

    // Polygon point vector
    std::vector<b2Vec2> m_vertVec;
};

class iObjectPhysicsData
{
public:

    // Constructor / Destructor
    iObjectPhysicsData();
    virtual ~iObjectPhysicsData();
    
    // Load thes object data from node
    virtual void loadFromNode( const XMLNode & objectNode )
    {}
    
    // Get the name of the physics world
    virtual const std::string & getWorld() const
    { return m_null_string; }

    // Get the body type
    virtual b2BodyType getBodyType() const
    { return b2_staticBody; }
    
    // Get the damping
    virtual float getLinearDamping() const
    { return 0.f; }
    virtual float getAngularDamping() const
    { return 0.f; }

    // Is the rotation fixed
    virtual bool isRotationFixed() const
    { return false; }
    
    // Get the fixture data
    virtual const std::vector<CFixture> & getFixtureVec() const
    { return m_null_fixture_vec; }

    // Specify if physics is active
    virtual bool isActive() const
    { return false; }
    

    // Get the body type
    virtual const std::string & getBodyShape() const
    { return m_null_string; }

    // Get the mass of physics body
    virtual float getMass() const
    { return 0.f; }

    // Get the friction
    virtual float getFriction() const
    { return 0.f; }
    virtual float getRollingFriction() const
    { return 0.f; }

    // Get the restitution - the amount of bounce
    virtual float getRestitution() const
    { return 0.f; }

    // Get the collision filter
    virtual short getCollisionGroup() const
    { return 0; }

    // Get the collision mask
    virtual short getCollisionMask() const
    { return 0; }

    // Get the plane normal
    virtual const CPoint<float> & getPlaneNormal() const
    { return m_null_point; }
    
    // Is this body a bullet
    virtual bool isBullet() const
    { return false; }

    // Is sleeping allowed?
    virtual bool allowSleep() const
    { return true; }
    
private:
    
    static std::string m_null_string;
    static std::vector<CFixture> m_null_fixture_vec;
    static CPoint<float> m_null_point;
};
