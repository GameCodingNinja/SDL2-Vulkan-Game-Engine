/************************************************************************
*    FILE NAME:       quad2d.h
*
*    NOTE: This class assumes 4 verts per quad using vert sharing.
*
*    DESCRIPTION:     quad 2d class
************************************************************************/  

#ifndef __quad_2d_h__
#define __quad_2d_h__

// Game lib dependencies
#include <common/vertex2d.h>

class CQuad2D
{
public:

    CQuad2D(){};
    CQuad2D( const CQuad2D &obj )
    {
        *this = obj;
    }

    // Verts
    CVertex2D vert[4];

};

#endif  // __face_2d_h__

