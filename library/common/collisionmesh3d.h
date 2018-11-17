/************************************************************************
*    FILE NAME:       collisionmesh3d.h
*
*    DESCRIPTION:     3D collision mesh class
************************************************************************/  

#ifndef __collision_mesh_3d_h__
#define __collision_mesh_3d_h__

// Game lib dependencies
#include <common/defs.h>
#include <common/point.h>

// Standard lib dependencies
#include <vector>

class CCollisionMesh3D
{
public:

    // Shared pointer of verticies
    std::vector<CPoint<float>> m_vbo;
    
    // Index buffer
    std::vector<uint16_t> m_ibo;
};

#endif
