/************************************************************************
*    FILE NAME:       mesh.h
*
*    DESCRIPTION:     Mesh class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <system/memorybuffer.h>

// Standard lib dependencies
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CTexture;

class CMesh
{
public:

    // Texture handles for this vbo and ibo
    std::vector<CTexture> m_textureVec;

    // VBO buffer
    CMemoryBuffer m_vboBuffer;

    // IBO buffer
    CMemoryBuffer m_iboBuffer;

    // Number of IBO needed for rendering
    int m_iboCount = 0;
};
