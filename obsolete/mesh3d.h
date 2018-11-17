/************************************************************************
*    FILE NAME:       mesh3d.h
*
*    DESCRIPTION:     3D mesh class
************************************************************************/  

#ifndef __mesh_3d_h__
#define __mesh_3d_h__

// Game lib dependencies
#include <common/texture.h>

// Standard lib dependencies
#include <vector>

class CMesh3D
{
public:

    // Loaded texture data
    std::vector<CTexture> m_textureVec;

    // VBO
    GLuint m_vbo = 0;

    // IBO
    GLuint m_ibo = 0;

    // Number of IBO needed for rendering
    GLuint m_iboCount = 0;

};

#endif  // __mesh_3d_h__
