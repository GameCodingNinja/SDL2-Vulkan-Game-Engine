/************************************************************************
*    FILE NAME:       objectdata2d.h
*
*    DESCRIPTION:     Class that holds a 2D object data
************************************************************************/

#ifndef __object_data_2d_h__
#define __object_data_2d_h__

// Game lib dependencies
#include <common/point.h>
#include <objectdata/objectvisualdata2d.h>
#include <objectdata/objectphysicsdata2d.h>
#include <objectdata/iobjectdata.h>

// Standard lib dependencies
#include <string>
#include <utility>

// Forward Declarations
struct XMLNode;

class CObjectData2D : public iObjectData
{
public:

    // Constructor/Destructor
    CObjectData2D();
    CObjectData2D( const CObjectData2D & obj );
    ~CObjectData2D();

    // Load the object data from the passed in node
    void loadFromNode( const XMLNode & node, const std::string & group, const std::string & name );

    // Create the objects from data
    void createFromData( const std::string & group );

    // Access functions for the visual data
    const CObjectVisualData2D & getVisualData() const;

    // Access functions for the physics data
    const CObjectPhysicsData2D & getPhysicsData() const;

    // Access functions for the data name
    const std::string & getName() const override;

    // Access functions for the data group
    const std::string & getGroup() const override;

    // Access functions for the size
    const CSize<int> & getSize() const override;

    // Access functions for the radius
    float getRadius() const override;

    // Access functions for the radius squared
    float getRadiusSquared() const override;

    // Is the generation type font
    bool isGenTypeFont() const override;

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
    CSize<int> m_size;

    // Square rooted and un-square rooted radius
    float m_radius;
    float m_radiusSquared;
};

#endif  // __object_data_2d_h__
