/************************************************************************
*    FILE NAME:       mesh3d.h
*
*    DESCRIPTION:     3D mesh class
************************************************************************/  

#ifndef __mesh_3d_h__
#define __mesh_3d_h__

// Game lib dependencies
#include <common/mesh.h>
#include <common/point.h>
#include <common/normal.h>
#include <common/uv.h>

// Standard lib dependencies
#include <vector>
#include <memory>

class CMesh3D
{
public:

    bool meshEmpty() const
    { return m_meshVec.empty(); }
    
    bool textEmpty() const
    { return m_meshVec.back().m_textureVec.empty(); }
    
    bool allowCreate()
    { return m_meshVec.back().allowCreate(); }
    
    size_t size() const
    { return m_meshVec.size(); }
    
    CMesh & back()
    { return m_meshVec.back(); }
    
    const CMesh & back() const
    { return m_meshVec.back(); }

    void reserve( size_t value )
    { m_meshVec.reserve(value); }
    
    void emplace_back()
    { m_meshVec.emplace_back(); }
    
    void emplace_back( const CMesh & mesh )
    { m_meshVec.emplace_back( mesh ); }
    
    std::vector<CMesh> & getMeshVec()
    { return m_meshVec; }
    
    const std::vector<CMesh> & getMeshVec() const
    { return m_meshVec; }
    
    void clearBuffers()
    {
        for( auto & iter : m_meshVec )
            iter.clearBuffers();
    }
    
    std::vector<CTexture> & getTextureVec()
    { return m_textureVec; }

private:
    
    // Loaded mesh data
    std::vector<CMesh> m_meshVec;
    
    // Loaded texture data
    std::vector<CTexture> m_textureVec;

};

#endif  // __mesh_3d_h__
