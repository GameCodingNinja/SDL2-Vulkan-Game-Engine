/************************************************************************
*    FILE NAME:       quad2d.h
*
*    NOTE: This class assumes 4 verts per quad using vert sharing.
*
*    DESCRIPTION:     quad 2d class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/vertex.h>

class CQuad2D
{
public:

    CQuad2D(){};
    CQuad2D( const CQuad2D &obj )
    {
        *this = obj;
    }

    // Verts
    NVertex::vert_uv vert[4];

};
