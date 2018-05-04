/************************************************************************
*    FILE NAME:       vertex3d.h
*
*    DESCRIPTION:     3D vertex class
************************************************************************/  

#ifndef __vertex_3d_h__
#define __vertex_3d_h__

// Game lib dependencies
#include <common/point.h>
#include <common/uv.h>
#include <common/normal.h>

class CVertex3D
{
public:

    // Verts
    CPoint<float> vert;

    // Vertex normal
    CNormal<float> norm;

    // uv
    CUV uv;
};


class CVertex3D_no_txt
{
public:

    // Verts
    CPoint<float> vert;

    // Vertex normal
    CNormal<float> norm;
};

#endif  // __vertex_2d_h__
