
/************************************************************************
*    FILE NAME:       normal.h
*
*    DESCRIPTION:     3D Normal class
************************************************************************/  

#pragma once

// Physical component dependency
#include <common/point.h>

template <typename type>
class CNormal : public CPoint<type>
{
public:

    // constructor
    CNormal()
    {
    }

    // Copy constructor
    CNormal( float _x, float _y, float _z )
        : CPoint<type>( _x, _y, _z )
    {
    }
    
    // Copy constructor
    CNormal( const CPoint<type> & obj ) :
        CPoint<type>( obj.x, obj.y, obj.z )
    {
    }

};
