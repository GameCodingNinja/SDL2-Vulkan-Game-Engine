
/************************************************************************
*    FILE NAME:       iobjectdata.h
*
*    DESCRIPTION:     Object Data interface class
************************************************************************/

#ifndef __iobjectdata_h__
#define __iobjectdata_h__

// Game lib dependencies
#include <common/size.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
class iObjectVisualData;
class iObjectPhysicsData;

class iObjectData
{
public:
    
    // Access functions for the visual data
    virtual const iObjectVisualData & getVisualData() const
    { return m_null_objectVisualData; }
    
    // Access functions for the physics data
    virtual const iObjectPhysicsData & getPhysicsData() const
    { return m_null_objectPhysicsData; }

    // Access functions for the data name
    virtual const std::string & getName() const
    { return m_null_string; }

    // Access functions for the data group
    virtual const std::string & getGroup() const
    { return m_null_string; }

    // Access functions for the radius
    virtual float getRadius() const
    { return 0.f; }

    // Access functions for the radius squared
    virtual float getRadiusSquared() const
    { return 0.f; }
    
    // Access functions for the size
    virtual const CSize<int> & getSize() const
    { return m_null_size; }
    
    // Is the generation type font
    virtual bool isGenTypeFont() const
    { return false; }
    
private:
    
    static iObjectVisualData m_null_objectVisualData;
    static iObjectPhysicsData m_null_objectPhysicsData;
    static CSize<int> m_null_size;
    static std::string m_null_string;
};

#endif  // __iobjectdata_h__


