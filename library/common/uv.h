/************************************************************************
*    FILE NAME:       uv.h
*
*    DESCRIPTION:     uv class
************************************************************************/  

#ifndef __uv_h__
#define __uv_h__

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

class CUV
{
public:

    float u, v;

    CUV():u(0),v(0)
    {
    }

    CUV( float _u, float _v )
    {
            u = _u;
            v = _v;
    }

    // Copy constructor
    CUV( const CUV &obj )
    {
        *this = obj;
    }
};

#endif
