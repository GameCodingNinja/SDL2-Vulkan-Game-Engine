/************************************************************************
*    FILE NAME:       mesh.h
*
*    DESCRIPTION:     Mesh class
************************************************************************/  

#ifndef __mesh_h__
#define __mesh_h__

// Game lib dependencies
#include <common/meshbinaryfileheader.h>
#include <common/vertex3d.h>
#include <common/texture.h>

// Standard lib dependencies
#include <vector>
#include <cstdint>
#include <memory>

class CMesh
{
public:
    
    void allocateBuffers()
    {
        if( m_faceGroup.textureCount != 0 )
        {
            m_spVBO.reset( new CVertex3D[m_faceGroup.vertexBufCount] );
            m_spTextIndexBuf.reset( new uint16_t[m_faceGroup.textureCount] );
        }
        else
            m_spVBONoTxt.reset( new CVertex3D_no_txt[m_faceGroup.vertexBufCount] );
        
        m_spIndexBuf.reset( new uint16_t[m_faceGroup.indexBufCount] );
    }
    
    void clearBuffers()
    {
        m_spVBO.reset();
        m_spVBONoTxt.reset();
        m_spIndexBuf.reset();
        m_spTextIndexBuf.reset();
    }
    
    bool allowCreate()
    { return (m_vbo == 0); }
    
public:

    // Loaded texture data
    std::vector<CTexture> m_textureVec;
    
    CBinaryFaceGroup m_faceGroup;
    
    std::shared_ptr<CVertex3D> m_spVBO;
    std::shared_ptr<CVertex3D_no_txt> m_spVBONoTxt;
    std::shared_ptr<uint16_t> m_spIndexBuf;
    std::shared_ptr<uint16_t> m_spTextIndexBuf;

    // VBO
    uint32_t m_vbo = 0;

    // IBO
    uint32_t m_ibo = 0;

    // Number of IBO needed for rendering
    uint32_t m_iboCount = 0;
};

#endif  // __mesh_h__
