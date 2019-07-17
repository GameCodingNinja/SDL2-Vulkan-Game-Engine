
/************************************************************************
*    FILE NAME:       objectdata3d.cpp
*
*    DESCRIPTION:     Class that holds a 3D object data for
************************************************************************/

// Physical component dependency
#include <objectdata/objectdata3d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <math.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectData3D::CObjectData3D() :
    m_radius(0),
    m_radiusSquared(0)
{
}


/************************************************************************
*    DESC:  Copy Constructor
************************************************************************/
CObjectData3D::CObjectData3D( const CObjectData3D & obj )
{
    *this = obj;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CObjectData3D::~CObjectData3D()
{
}


/************************************************************************
*    DESC:  Load the object data from the passed in node
************************************************************************/
void CObjectData3D::loadFromNode( const XMLNode & node, const std::string & group, const std::string & name )
{
    m_name = name;
    m_group = group;

    // Load the visual data
    m_visualData.loadFromNode( node );

    // Load the physics data
    m_physicsData.loadFromNode( node );
}


/************************************************************************
*    DESC:  Create the objects from data
************************************************************************/
void CObjectData3D::createFromData( const std::string & group )
{
    // Create the visuals
    m_visualData.createFromData( group );

    // Calculate the radii
    //m_radiusSquared = pow((float)m_size.w / 2, 2) + pow((float)m_size.h / 2, 2);
    //m_radius = sqrt( m_radiusSquared );
}


/************************************************************************
*    DESC:  Access functions for the visual data
************************************************************************/
const iObjectVisualData & CObjectData3D::getVisualData() const
{
    return m_visualData;
}


/************************************************************************
*    DESC:  Access functions for the physics data
************************************************************************/
const CObjectPhysicsData3D & CObjectData3D::getPhysicsData() const
{
    return m_physicsData;
}


/************************************************************************
*    DESC:  Access functions for the data name
************************************************************************/
const std::string & CObjectData3D::getName() const
{
    return m_name;
}


/************************************************************************
*    DESC:  Access functions for the data group
************************************************************************/
const std::string & CObjectData3D::getGroup() const
{
    return m_group;
}


/************************************************************************
*    DESC:  Access functions for the radius
************************************************************************/
float CObjectData3D::getRadius() const
{
    return m_radius;
}


/************************************************************************
*    DESC:  Access functions for the radius squared
************************************************************************/
float CObjectData3D::getRadiusSquared() const
{
    return m_radiusSquared;
}
