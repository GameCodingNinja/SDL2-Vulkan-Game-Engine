/************************************************************************
*    FILE NAME:       mathfunc.h
*
*    DESCRIPTION:     Math utility functions
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>

class CCamera;
class CMatrix;

namespace NMathFunc
{
    // 2D dot product
    float DotProduct2D( const CPoint<float> & p1, const CPoint<float> & p2 );
    CWorldValue DotProduct2D( const CPoint<CWorldValue> & p1, const CPoint<CWorldValue> & p2 );
    CWorldValue DotProduct2D( const CPoint<CWorldValue> & p1, const CPoint<float> & p2 );

    // 2D cross products
    float CrossProduct2D( const CPoint<float> & p1, const CPoint<float> & p2 );
    CPoint<float> CrossProduct2D( const CPoint<float> & p, float s );
    CPoint<float> CrossProduct2D( float s, const CPoint<float> & p );

    // Perform a modulus operation on the passed in floats
    float Modulus( float v1, float v2 );

    // Get the mouse's position in 3D
    CPoint<float> MouseTo3D( float mx, float my, CCamera matCam, CMatrix matProj );
}
