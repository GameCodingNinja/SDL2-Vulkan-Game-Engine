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
#include <memory>
#include <cstdint>

class CCollisionMesh3D
{
public:

    // Shared pointer of verticies
    std::shared_ptr<CPoint<float>> vbo;
    
    // Index buffer
    std::shared_ptr<uint16_t> ibo;

    // Number of IBO needed for rendering
    uint m_iboCount = 0;

};

#endif  // __collision_mesh_3d_h__
