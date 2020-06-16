
/************************************************************************
*    FILE NAME:       dynamicoffset.h
*
*    DESCRIPTION:     Dynamic Offset class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/point.h>
#include <common/size.h>
#include <utilities/bitmask.h>

class CDynamicOffset
{
public:

    enum EDynamicOffset
    {
        EDO_NULL=0,
        EDO_LEFT=1,
        EDO_RIGHT=2,
        EDO_HORZ_CENTER=4,
        EDO_TOP=8,
        EDO_BOTTOM=16,
        EDO_VERT_CENTER=32,
    };

    // Add to the bit mask
    void add( EDynamicOffset value );

    // Set/Get X
    void setX( float value );

    // Set/Get Y
    void setY( float value );

    // Is the dynamic offset being used
    bool isEmpty() const;

    // Get the dynamic position
    CPoint<float> getPos( CSize<float> defaultHalfSize );
    CPoint<float> getPos();

private:

    // bit mask parameters
    CBitmask<uint> m_parameters;

    // offset
    CPoint<float> point;
};
