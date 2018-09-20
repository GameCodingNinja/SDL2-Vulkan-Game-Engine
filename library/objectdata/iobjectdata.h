
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

class iObjectData
{
public:

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
    
    static CSize<int> m_null_size;
    static std::string m_null_string;
};

#endif  // __iobjectdata_h__


