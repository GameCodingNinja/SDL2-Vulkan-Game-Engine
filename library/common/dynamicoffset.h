
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

    // Add to the bit mask
    void add( int value );

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
