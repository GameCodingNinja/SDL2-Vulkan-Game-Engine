
/************************************************************************
*    FILE NAME:       objectphysicsdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's physics data
************************************************************************/

#ifndef __object_physics_data_2d_h__
#define __object_physics_data_2d_h__

// Box2D lib dependencies
#include <Box2D/Box2D.h>

// Standard lib dependencies
#include <string>
#include <vector>

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

class CObjectPhysicsData2D
{
public:

    // Constructor
    CObjectPhysicsData2D();
    ~CObjectPhysicsData2D();

    // Load thes object data from node
    void loadFromNode( const XMLNode & objectNode );
    
    // Get the name of the physics world
    const std::string & getWorld() const;

    // Get the body type
    b2BodyType getBodyType() const;
    
    // Get the damping
    float getLinearDamping() const;
    float getAngularDamping() const;

    // Is the rotation fixed
    bool isRotationFixed() const;
    
    // Get the fixture data
    const std::vector<CFixture> & getFixtureVec() const;

    // Specify if physics is active
    bool isActive() const;

private:

    // The name of the physics world
    std::string m_world;
    
    // Type of physics body
    b2BodyType m_bodyType;
    
    // The constant decceleration of movement and rotation
    float m_linearDamping, m_angularDamping;
    
    // If we want to prevent the object from rotating due to physicss
    bool m_fixedRotation;
    
    // vector of fixtures
    std::vector<CFixture> m_fixtureVec;
};

#endif  // __object_physics_data_2d_h__
