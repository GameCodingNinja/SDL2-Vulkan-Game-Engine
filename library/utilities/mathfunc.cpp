/************************************************************************
*    FILE NAME:       mathfunc.cpp
*
*    DESCRIPTION:     Math utility functions
************************************************************************/  

// Physical component dependency
#include <utilities/mathfunc.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <utilities/settings.h>
#include <common/camera.h>

namespace NMathFunc
{
    /************************************************************************
    *    DESC:  Calculate the dot product of the passed in values  
    *
    *	 param: const CPoint/CWorldPoint & p1 - point to compare
    *			const CPoint/CWorldPoint & p2 - point to compare 
    *
    *	 ret:	float/CWorldValue - result of dot product
    ************************************************************************/
    float DotProduct2D( const CPoint<float> & p1, const CPoint<float> & p2 )
    {
        return p1.x * p2.x + p1.y * p2.y;
    }

    CWorldValue DotProduct2D( const CPoint<CWorldValue> & p1, const CPoint<CWorldValue> & p2 )
    {
        return p1.x * p2.x + p1.y * p2.y;
    } 

    CWorldValue DotProduct2D( const CPoint<CWorldValue> & p1, const CPoint<float> & p2 )
    {
        return p1.x * p2.x + p1.y * p2.y;
    }


    /************************************************************************
    *    DESC:  Calculate the cross product of the passed in values  
    *
    *	 param: CPoint/float - value to compare
    *			CPoint/float - value to compare 
    *
    *	 ret:	float/CPoint - result of cross product
    ************************************************************************/
    float CrossProduct2D( const CPoint<float> & p1, const CPoint<float> & p2 )
    {
        return p1.x * p2.y - p1.y * p2.x;
    }

    CPoint<float> CrossProduct2D( const CPoint<float> & p, float s )
    {
        return CPoint<float>( s * p.y, -s * p.x );
    }
 
    CPoint<float> CrossProduct2D( float s, const CPoint<float> & p )
    {
        return CPoint<float>( -s * p.y, s * p.x );
    }


    /************************************************************************
    *    DESC:  Perform a modulus operation on the passed in floats 
    *
    *	 param: float v1, v2 - values to mod
    *
    *	 ret:	float - result
    ************************************************************************/
    float Modulus( float v1, float v2 )
    {
        return v1 - v2 * floor(v1 / v2);
    }


    /************************************************************************
    *    DESC:  Get the mouse's position in 3D
    *
    *	 param: float mx, my - mouse position
    *           CCamera matCam - camera matrix
    *           CMatrix matProj - projection matrix
    *
    *	 ret:   CPoint<float> - result
    ************************************************************************/
    CPoint<float> MouseTo3D( float mx, float my, CCamera matCam, CMatrix matProj )
    {
        // Combine the camera and projection transformations and then invert the result
        // so we can use it like we're reversing all transformations on the mouse point.
        CMatrix m = matCam.getMatrix() * matProj;
        m.invert();

        // Convert to values between -1 and 1.
        CPoint<float> mp;
        mp.x = (2.f * (mx / (CSettings::Instance().getSize().w - 1.f))) - 1.f;
        mp.y = 1.f - (2.f * (my / (CSettings::Instance().getSize().h - 1.f)));
        mp.z = 1;

        // Undo the projection transformation.
        CPoint<float> r;
        m.transform( r, mp );

        // Temporary code to project onto the xy plane.
        r.x *= -matCam.getPos().z - r.z;
        r.y *= -matCam.getPos().z - r.z;
        r.z = 0;

        return r;
    }
}
