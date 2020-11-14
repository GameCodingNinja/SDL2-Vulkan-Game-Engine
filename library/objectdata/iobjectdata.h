
/************************************************************************
*    FILE NAME:       iobjectdata.h
*
*    DESCRIPTION:     Object Data interface class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/size.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
class iObjectVisualData;
class iObjectPhysicsData;
struct XMLNode;

class iObjectData
{
public:

    // Constructor / Destructor
    iObjectData();
    virtual ~iObjectData();

    // Load the object data from the passed in node
    virtual void loadFromNode( const XMLNode & node, const std::string & group, const std::string & name ) = 0;

    // Create the objects from data
    virtual void createFromData( const std::string & group ) = 0;
    
    // Access functions for the visual data
    virtual const iObjectVisualData & getVisualData() const = 0;
    
    // Access functions for the physics data
    virtual const iObjectPhysicsData & getPhysicsData() const = 0;

    // Access functions for the data name
    virtual const std::string & getName() const = 0;

    // Access functions for the data group
    virtual const std::string & getGroup() const = 0;

    // Access functions for the radius
    virtual float getRadius() const
    { return 0.f; }

    // Access functions for the radius squared
    virtual float getRadiusSquared() const
    { return 0.f; }
    
    // Access functions for the size
    virtual const CSize<float> & getSize() const
    { return m_null_size; }
    
    // Is the generation type font
    virtual bool isGenTypeFont() const
    { return false; }

    // Is the data type 2D?
    virtual bool is2D() const
    { return false; }

    // Is the data type 3D?
    virtual bool is3D() const
    { return false; }
    
private:

    static CSize<float> m_null_size;
};
