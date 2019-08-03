
/************************************************************************
*    FILE NAME:       objectphysicsdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's physics data
************************************************************************/

#ifndef __object_physics_data_2d_h__
#define __object_physics_data_2d_h__

// Physical component dependency
#include <objectdata/iobjectphysicsdata.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
struct XMLNode;

class CObjectPhysicsData2D : public iObjectPhysicsData
{
public:

    // Constructor
    CObjectPhysicsData2D();
    virtual ~CObjectPhysicsData2D();

    // Load thes object data from node
    void loadFromNode( const XMLNode & objectNode ) override;
    
    // Get the name of the physics world
    const std::string & getWorld() const override;

    // Get the body type
    b2BodyType getBodyType() const override;
    
    // Get the damping
    float getLinearDamping() const override;
    float getAngularDamping() const override;

    // Is the rotation fixed
    bool isRotationFixed() const override;
    
    // Get the fixture data
    const std::vector<CFixture> & getFixtureVec() const override;

    // Specify if physics is active
    bool isActive() const override;

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
