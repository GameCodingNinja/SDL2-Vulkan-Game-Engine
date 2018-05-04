/************************************************************************
*    FILE NAME:       face2d.h
*
*    DESCRIPTION:     2D face class
************************************************************************/  

#ifndef __face_2d_h__
#define __face_2d_h__

#include <common/vertex2d.h>

class CFace2D
{
public:

    CFace2D(){};
    CFace2D( const CFace2D &obj )
    {
        *this = obj;
    }

    // Verts
    CVertex2D vert[3];

};

#endif  // __face_2d_h__

