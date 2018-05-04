/************************************************************************
*    FILE NAME:       vertex2d.h
*
*    DESCRIPTION:     2D vertex class
************************************************************************/  

#ifndef __vertex_2d_h__
#define __vertex_2d_h__

// Game lib dependencies
#include <common/point.h>
#include <common/uv.h>

class CVertex2D
{
public:

    // Verts
    CPoint<float> vert;

    // uv
    CUV uv;
};

#endif  // __vertex_2d_h__
