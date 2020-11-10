
/************************************************************************
*    FILE NAME:       radian.h
*
*    DESCRIPTION:     3D Radian class
************************************************************************/  

#pragma once

// Physical component dependency
#include <common/point.h>

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

template <typename type>
class CRadian : public CPoint<type>
{
public:

    // constructor
    CRadian()
    {
    }

    // Copy constructor
    CRadian( float _x, float _y, float _z ) :
        CPoint<type>( _x, _y, _z )
    {
    }

    // Copy constructor
    CRadian( const CPoint<type> & obj ) :
        CPoint<type>( obj.x, obj.y, obj.z )
    {
    }
};
