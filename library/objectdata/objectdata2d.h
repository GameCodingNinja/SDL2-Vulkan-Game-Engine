/************************************************************************
*    FILE NAME:       objectdata2d.h
*
*    DESCRIPTION:     Class that holds a 2D object data
************************************************************************/

#pragma once

// Physical component dependency
#include <objectdata/iobjectdata.h>

// Game lib dependencies
#include <common/point.h>
#include <objectdata/objectvisualdata2d.h>
#include <objectdata/objectphysicsdata2d.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
struct XMLNode;

class CObjectData2D : public iObjectData
{
public:

    // Constructor/Destructor
    CObjectData2D();
    CObjectData2D( const CObjectData2D & obj );
    virtual ~CObjectData2D();

    // Load the object data from the passed in node
    void loadFromNode( const XMLNode & node, const std::string & group, const std::string & name ) override;

    // Create the objects from data
    void createFromData( const std::string & group ) override;

    // Access functions for the visual data
    const iObjectVisualData & getVisualData() const override;

    // Access functions for the physics data
    const iObjectPhysicsData & getPhysicsData() const override;

    // Access functions for the data name
    const std::string & getName() const override;

    // Access functions for the data group
    const std::string & getGroup() const override;

    // Access functions for the size
    const CSize<float> & getSize() const override;

    // Access functions for the radius
    float getRadius() const override;

    // Access functions for the radius squared
    float getRadiusSquared() const override;

    // Is the generation type font
    bool isGenTypeFont() const override;

    // Is the data type 2D?
    bool is2D() const override
    { return true; }

private:

    // Visual data of the object
    CObjectVisualData2D m_visualData;

    // Physics data of the object
    CObjectPhysicsData2D m_physicsData;

    // The name of the object data
    std::string m_name;

    // The group the object data is in
    std::string m_group;

    // The initial size of the object
    CSize<float> m_size;

    // Square rooted and un-square rooted radius
    float m_radius;
    float m_radiusSquared;
};
