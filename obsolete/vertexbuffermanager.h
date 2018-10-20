
/************************************************************************
*    FILE NAME:       vertexbuffermanager.h
*
*    DESCRIPTION:     vertex buffer manager class singleton
************************************************************************/  

#ifndef __vertex_buffer_manager_h__
#define __vertex_buffer_manager_h__

// Game lib dependencies
#include <common/rect.h>
#include <common/size.h>
#include <common/point.h>
#include <common/vertex2d.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class CShaderData;
class CScaledFrame;
class CQuad2D;
class CUV;

class CVertBufMgr
{
public:

    // Get the instance of the singleton class
    static CVertBufMgr & Instance()
    {
        static CVertBufMgr VertBufMgr;
        return VertBufMgr;
    }
    
    // Create VBO from vertex data
    uint32_t createVBO( 
        const std::string & group,
        const std::string & name,
        const std::vector<CVertex2D> & vertVec );

    // Create a IBO buffer
    uint32_t createIBO(
        const std::string & group,
        const std::string & name,
        uint8_t indexData[],
        int sizeInBytes );

    // Creates and manages the dynamic font indicies IBO
    uint32_t createDynamicFontIBO(
        const std::string & group,
        const std::string & name,
        uint16_t * pIndexData,
        int maxIndicies );

    // Create a scaled frame
    uint32_t createScaledFrame( 
        const std::string & group,
        const std::string & name,
        const CScaledFrame & scaledFrame,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & spriteSheetOffset,
        const std::vector<CVertex2D> & vertVec );

    void createQuad(
        const CPoint<float> & vert,
        const CSize<float> & vSize,
        const CUV & uv,
        const CSize<float> & uvSize,
        const CSize<float> & textureSize,
        const CSize<float> & frameSize,
        const CRect<float> & spriteSheetOffset,
        CQuad2D & quadBuf );
    
    // Generate a scaled frame
    void generateScaledFrame(
        std::vector<CVertex2D> & vertVec,
        const CScaledFrame & scaledFrame,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & spriteSheetOffset );

    // Function call used to manage what buffer is currently bound.
    void bind( uint32_t vboID, uint32_t iboID );

    // Unbind the buffers and reset the flag
    void unbind();

    // Delete buffer group
    void deleteBufferGroupFor2D( const std::string & group );
    
    // See if a VBO already exists
    uint32_t isVBO( const std::string & group, const std::string & name );
    
    // Get the current max font indices
    int getCurrentMaxFontIndices();

private:

    // Constructor
    CVertBufMgr();

    // Destructor
    ~CVertBufMgr();

private:

    // Map containing a group of VBO handles
    std::map< const std::string, std::map< const std::string, uint32_t > > m_vertexBuf2DMapMap;

    // Map containing a group of IBO handles
    std::map< const std::string, std::map< const std::string, uint32_t > > m_indexBuf2DMapMap;

    // Current vbo ID
    uint32_t m_currentVBOID;

    // Current IBO ID
    uint32_t m_currentIBOID;

    // Current dynamic font IBO indices size
    int m_currentMaxFontIndices;

};

#endif  // __vertex_buffer_manager_h__


