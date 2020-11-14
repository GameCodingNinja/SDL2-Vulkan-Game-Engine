/************************************************************************
*    FILE NAME:       objectdata3d.h
*
*    DESCRIPTION:     Class that holds a 3D object data for
************************************************************************/

#pragma once

// Physical component dependency
#include <objectdata/iobjectdata.h>

// Game lib dependencies
#include <common/point.h>
#include <objectdata/objectvisualdata3d.h>
#include <objectdata/objectphysicsdata3d.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
struct XMLNode;

class CObjectData3D : public iObjectData
{
public:

    // Constructor / Destructor
    CObjectData3D();
    CObjectData3D( const CObjectData3D & obj );
    virtual ~CObjectData3D();

    // Load the object data from the passed in node
    void loadFromNode( const XMLNode & node, const std::string & group, const std::string & name ) override;

    // Create the objects from data
    void createFromData( const std::string & group ) override;

    // Access functions for the visual data
    const iObjectVisualData & getVisualData() const override;

    // Access functions for the physics data
    const CObjectPhysicsData3D & getPhysicsData() const override;

    // Access functions for the data name
    const std::string & getName() const override;

    // Access functions for the data group
    const std::string & getGroup() const override;

    // Access functions for the radius
    float getRadius() const override;

    // Access functions for the radius squared
    float getRadiusSquared() const override;

    // Is the data type 3D?
    bool is3D() const override
    { return true; }

private:

    // Visual data of the object
    CObjectVisualData3D m_visualData;

    // Physics data of the object
    CObjectPhysicsData3D m_physicsData;

    // The name of the object data
    std::string m_name;

    // The group the object data is in
    std::string m_group;

    // Square rooted and un-square rooted radius
    float m_radius;
    float m_radiusSquared;
};
