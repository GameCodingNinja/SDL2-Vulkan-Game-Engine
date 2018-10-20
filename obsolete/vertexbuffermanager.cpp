
/************************************************************************
*    FILE NAME:       vertexbuffermanager.cpp
*
*    DESCRIPTION:     vertex buffer manager class singleton
************************************************************************/

// Physical component dependency
#include <managers/vertexbuffermanager.h>

// Game lib dependencies
#include <common/quad2d.h>
#include <common/shaderdata.h>
#include <common/scaledframe.h>
#include <common/uv.h>

#include <iostream>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CVertBufMgr::CVertBufMgr()
    : m_currentVBOID(0),
      m_currentIBOID(0),
      m_currentMaxFontIndices(0)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CVertBufMgr::~CVertBufMgr()
{
    // Free all vertex buffers in all groups
    /*for( auto & mapMapIter : m_vertexBuf2DMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            glDeleteBuffers(1, &mapIter.second);
        }
    }

    // Free all index buffers in all groups
    for( auto & mapMapIter : m_indexBuf2DMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            glDeleteBuffers(1, &mapIter.second);
        }
    }*/
}


/************************************************************************
*    DESC: Create VBO
************************************************************************/
uint32_t CVertBufMgr::createVBO(
    const std::string & group,
    const std::string & name,
    const std::vector<CVertex2D> & vertVec )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_vertexBuf2DMapMap.find( group );
    if( mapMapIter == m_vertexBuf2DMapMap.end() )
        mapMapIter = m_vertexBuf2DMapMap.emplace( group, std::map<const std::string, uint32_t>() ).first;

    // See if this vertex buffer ID has already been loaded
    auto mapIter = mapMapIter->second.find( name );

    // If it's not found, create the vertex buffer and add it to the list
    /*if( mapIter == mapMapIter->second.end() )
    {
        uint32_t vboID = 0;
        glGenBuffers( 1, &vboID );
        glBindBuffer( GL_ARRAY_BUFFER, vboID );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CVertex2D)*vertVec.size(), vertVec.data(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // Insert the new vertex buffer info
        mapIter = mapMapIter->second.emplace( name, vboID ).first;
    }*/

    return mapIter->second;
}


/************************************************************************
*    DESC:  Create a IBO buffer
************************************************************************/
uint32_t CVertBufMgr::createIBO( const std::string & group, const std::string & name, uint8_t indexData[], int sizeInBytes )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_indexBuf2DMapMap.find( group );
    if( mapMapIter == m_indexBuf2DMapMap.end() )
            mapMapIter = m_indexBuf2DMapMap.emplace( group, std::map<const std::string, uint32_t>() ).first;

    // See if this intex buffer ID has already been loaded
    auto mapIter = mapMapIter->second.find( name );

    // If it's not found, create the intex buffer and add it to the list
    /*if( mapIter == mapMapIter->second.end() )
    {
        uint32_t iboID = 0;
        glGenBuffers( 1, &iboID );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, indexData, GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        // Insert the new intex buffer info
        mapIter = mapMapIter->second.emplace( name, iboID ).first;
    }*/

    return mapIter->second;
}


/************************************************************************
*    DESC:  Create a dynamic font IBO buffer
************************************************************************/
uint32_t CVertBufMgr::createDynamicFontIBO( const std::string & group, const std::string & name, uint16_t * pIndexData, int maxIndicies )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_indexBuf2DMapMap.find( group );
    if( mapMapIter == m_indexBuf2DMapMap.end() )
        mapMapIter = m_indexBuf2DMapMap.emplace( group, std::map<const std::string, uint32_t>() ).first;

    // See if this intex buffer ID has already been loaded
    auto mapIter = mapMapIter->second.find( name );

    // If it's not found, create the intex buffer and add it to the list
    /*if( mapIter == mapMapIter->second.end() )
    {
        uint32_t iboID = 0;
        glGenBuffers( 1, &iboID );

        // Insert the new intex buffer info
        mapIter = mapMapIter->second.emplace( name, iboID ).first;
    }

    // If the new indices are greater then the current, init the IBO with the newest
    if( maxIndicies > m_currentMaxFontIndices )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mapIter->second );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * maxIndicies, pIndexData, GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        m_currentMaxFontIndices = maxIndicies;
    }*/

    return mapIter->second;
}


/************************************************************************
*    DESC:  Create a scaled frame
*    NOTE: This is a bit of a brute force implementation but writing an
*          algorithm that takes into account an index buffer is difficult
************************************************************************/
uint32_t CVertBufMgr::createScaledFrame(
    const std::string & group,
    const std::string & name,
    const CScaledFrame & scaledFrame,
    const CSize<int> & textureSize,
    const CSize<int> & glyphSize,
    const CSize<int> & frameSize,
    const CRect<float> & spriteSheetOffset,
    const std::vector<CVertex2D> & vertVec )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_vertexBuf2DMapMap.find( group );
    if( mapMapIter == m_vertexBuf2DMapMap.end() )
        mapMapIter = m_vertexBuf2DMapMap.emplace( group, std::map<const std::string, uint32_t>() ).first;

    // See if this vertex buffer ID has already been loaded
    auto mapIter = mapMapIter->second.find( name );

    // If it's not found, create the vertex buffer and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        std::vector<CVertex2D> vertVecTmp;
        vertVecTmp.reserve(16);

        // Generate the scaled frame
        generateScaledFrame( vertVecTmp, scaledFrame, textureSize, glyphSize, frameSize, spriteSheetOffset );

        // Add in any additional verts
        if( !vertVecTmp.empty() )
            vertVecTmp.insert( vertVecTmp.end(), vertVec.begin(), vertVec.end() );

        uint32_t vboID = 0;
        /*glGenBuffers( 1, &vboID );
        glBindBuffer( GL_ARRAY_BUFFER, vboID );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CVertex2D)*vertVecTmp.size(), vertVecTmp.data(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ARRAY_BUFFER, 0 );*/

        // Insert the new vertex buffer info
        mapIter = mapMapIter->second.emplace( name, vboID ).first;
    }

    return mapIter->second;
}


/************************************************************************
*    DESC: Generate a scaled frame
*    NOTE: This is a bit of a brute force implementation but writing an
*          algorithm that takes into account an index buffer is difficult
************************************************************************/
void CVertBufMgr::generateScaledFrame(
    std::vector<CVertex2D> & vertVec,
    const CScaledFrame & scaledFrame,
    const CSize<int> & textureSize,
    const CSize<int> & glyphSize,
    const CSize<int> & frameSize,
    const CRect<float> & spriteSheetOffset )
{
    // Offsets to center the mesh
    const CPoint<float> center((frameSize.w / 2.f), (frameSize.h / 2.f));
    const CSize<float> frameLgth( (float)frameSize.w - ((float)scaledFrame.m_frame.w * 2.f), (float)frameSize.h - ((float)scaledFrame.m_frame.h * 2.f) );
    const CSize<float> uvLgth( glyphSize.w - ((float)scaledFrame.m_frame.w * 2.f), glyphSize.h - ((float)scaledFrame.m_frame.h * 2.f) );

    CQuad2D quadBuf[8];

    // Left frame
    createQuad( CPoint<float>(-center.x, center.y-(float)scaledFrame.m_frame.h),
                CSize<float>((float)scaledFrame.m_frame.w, -frameLgth.h),
                CUV(0, (float)scaledFrame.m_frame.h),
                CSize<float>((float)scaledFrame.m_frame.w, uvLgth.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[0] );

    // top left
    createQuad( CPoint<float>(-center.x, center.y),
                CSize<float>((float)scaledFrame.m_frame.w, -(float)scaledFrame.m_frame.h),
                CUV(0, 0),
                CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[1] );

    // top
    createQuad( CPoint<float>(-(center.x-(float)scaledFrame.m_frame.w), center.y),
                CSize<float>(frameLgth.w, -(float)scaledFrame.m_frame.h),
                CUV((float)scaledFrame.m_frame.w, 0),
                CSize<float>(uvLgth.w, (float)scaledFrame.m_frame.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[2] );

    // top right
    createQuad( CPoint<float>(center.x-(float)scaledFrame.m_frame.w, center.y),
                CSize<float>((float)scaledFrame.m_frame.w, -(float)scaledFrame.m_frame.h),
                CUV((float)scaledFrame.m_frame.w + uvLgth.w,0),
                CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[3] );

    // right frame
    createQuad( CPoint<float>(center.x-scaledFrame.m_frame.w, center.y-scaledFrame.m_frame.h),
                CSize<float>(scaledFrame.m_frame.w, -frameLgth.h),
                CUV(scaledFrame.m_frame.w + uvLgth.w, scaledFrame.m_frame.h),
                CSize<float>(scaledFrame.m_frame.w, uvLgth.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[4] );

    // bottom right
    createQuad( CPoint<float>(center.x-(float)scaledFrame.m_frame.w, -(center.y-(float)scaledFrame.m_frame.h)),
                CSize<float>((float)scaledFrame.m_frame.w, -(float)scaledFrame.m_frame.h),
                CUV((float)scaledFrame.m_frame.w + uvLgth.w, (float)scaledFrame.m_frame.h + uvLgth.h),
                CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[5] );

    // bottom frame
    createQuad( CPoint<float>(-(center.x-(float)scaledFrame.m_frame.w), -(center.y-(float)scaledFrame.m_frame.h)),
                CSize<float>(frameLgth.w, -(float)scaledFrame.m_frame.h),
                CUV((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h + uvLgth.h),
                CSize<float>(uvLgth.w, (float)scaledFrame.m_frame.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[6] );

    // bottom left
    createQuad( CPoint<float>(-center.x, -(center.y-(float)scaledFrame.m_frame.h)),
                CSize<float>((float)scaledFrame.m_frame.w, -(float)scaledFrame.m_frame.h),
                CUV(0, (float)scaledFrame.m_frame.h + uvLgth.h),
                CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                textureSize,
                frameSize,
                spriteSheetOffset,
                quadBuf[7] );

    // Piece together the needed unique verts
    /*vertVec.push_back( quadBuf[0].vert[0] );
    vertVec.push_back( quadBuf[0].vert[1] );
    vertVec.push_back( quadBuf[0].vert[2] );
    vertVec.push_back( quadBuf[0].vert[3] );
    vertVec.push_back( quadBuf[1].vert[1] );
    vertVec.push_back( quadBuf[1].vert[2] );
    vertVec.push_back( quadBuf[2].vert[1] );
    vertVec.push_back( quadBuf[2].vert[3] );
    vertVec.push_back( quadBuf[3].vert[1] );
    vertVec.push_back( quadBuf[3].vert[3] );
    vertVec.push_back( quadBuf[4].vert[0] );
    vertVec.push_back( quadBuf[4].vert[3] );
    vertVec.push_back( quadBuf[5].vert[0] );
    vertVec.push_back( quadBuf[5].vert[3] );
    vertVec.push_back( quadBuf[6].vert[0] );
    vertVec.push_back( quadBuf[7].vert[0] );*/
}


/************************************************************************
*    DESC:  Create a quad
************************************************************************/
void CVertBufMgr::createQuad(
    const CPoint<float> & vert,
    const CSize<float> & vSize,
    const CUV & uv,
    const CSize<float> & uvSize,
    const CSize<float> & textureSize,
    const CSize<float> & frameSize,
    const CRect<float> & spriteSheetOffset,
    CQuad2D & quadBuf )
{
    // For OpenGL pixel perfect rendering is an even size graphic,
    // for DirectX, it's an odd size graphic.

    // Check if the width or height is odd. If so, we offset
    // by 0.5 for proper orthographic rendering
    float additionalOffsetX = 0;
    if( (int)frameSize.w % 2 != 0 )
        additionalOffsetX = 0.5f;

    float additionalOffsetY = 0;
    if( (int)frameSize.h % 2 != 0 )
        additionalOffsetY = 0.5f;

    // Calculate the third vertex of the first face
    quadBuf.vert[0].vert.x = vert.x + additionalOffsetX;
    quadBuf.vert[0].vert.y = vert.y + additionalOffsetY + vSize.h;
    quadBuf.vert[0].uv.u = spriteSheetOffset.x1 + (uv.u / textureSize.w);
    quadBuf.vert[0].uv.v = spriteSheetOffset.y1 + ((uv.v + uvSize.h) / textureSize.h);

    // Calculate the second vertex of the first face
    quadBuf.vert[1].vert.x = vert.x + additionalOffsetX + vSize.w;
    quadBuf.vert[1].vert.y = vert.y + additionalOffsetY;
    quadBuf.vert[1].uv.u = spriteSheetOffset.x1 + ((uv.u + uvSize.w) / textureSize.w);
    quadBuf.vert[1].uv.v = spriteSheetOffset.y1 + (uv.v / textureSize.h);

    // Calculate the first vertex of the first face
    quadBuf.vert[2].vert.x = quadBuf.vert[0].vert.x;
    quadBuf.vert[2].vert.y = quadBuf.vert[1].vert.y;
    quadBuf.vert[2].uv.u = quadBuf.vert[0].uv.u;
    quadBuf.vert[2].uv.v = quadBuf.vert[1].uv.v;

    // Calculate the second vertex of the second face
    quadBuf.vert[3].vert.x = quadBuf.vert[1].vert.x;
    quadBuf.vert[3].vert.y = quadBuf.vert[0].vert.y;
    quadBuf.vert[3].uv.u = quadBuf.vert[1].uv.u;
    quadBuf.vert[3].uv.v = quadBuf.vert[0].uv.v;
}


/************************************************************************
*    DESC: See if a VBO already exists
************************************************************************/
uint32_t CVertBufMgr::isVBO( const std::string & group, const std::string & name )
{
    // See if the group exists
    auto mapMapIter = m_vertexBuf2DMapMap.find( group );
    if( mapMapIter == m_vertexBuf2DMapMap.end() )
        return 0;

    // See if this vertex buffer ID has already been created
    auto mapIter = mapMapIter->second.find( name );
    if( mapIter == mapMapIter->second.end() )
        return 0;

    return mapIter->second;
}


/************************************************************************
*    DESC:  Function call used to manage what buffer is currently bound.
*           This insures that we don't keep rebinding the same buffer
************************************************************************/
void CVertBufMgr::bind( uint32_t vboID, uint32_t iboID )
{
    /*if( m_currentVBOID != vboID )
    {
        // save the current binding
        m_currentVBOID = vboID;

        // Have OpenGL bind this buffer now
        glBindBuffer( GL_ARRAY_BUFFER, vboID );
    }

    if( m_currentIBOID != iboID )
    {
        // save the current binding
        m_currentIBOID = iboID;

        // Have OpenGL bind this buffer now
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );
    }*/
}


/************************************************************************
*    DESC:  Unbind the buffers and reset the flag
************************************************************************/
void CVertBufMgr::unbind()
{
    m_currentVBOID = 0;
    m_currentIBOID = 0;
    //glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


/************************************************************************
*    DESC:  Delete buffer group
************************************************************************/
void CVertBufMgr::deleteBufferGroupFor2D( const std::string & group )
{
    /*{
        auto mapMapIter = m_vertexBuf2DMapMap.find( group );
        if( mapMapIter != m_vertexBuf2DMapMap.end() )
        {
            // Delete all the buffers in this group
            for( auto & mapIter : mapMapIter->second )
            {
                glDeleteBuffers(1, &mapIter.second);
            }

            // Erase this group
            m_vertexBuf2DMapMap.erase( mapMapIter );
        }
    }

    {
        auto mapMapIter = m_indexBuf2DMapMap.find( group );
        if( mapMapIter != m_indexBuf2DMapMap.end() )
        {
            // Delete all the buffers in this group
            for( auto & mapIter : mapMapIter->second )
            {
                glDeleteBuffers(1, &mapIter.second);
            }

            // Erase this group
            m_indexBuf2DMapMap.erase( mapMapIter );
        }
    }*/
}


/************************************************************************
*    DESC:  Get the current max font indices
************************************************************************/
int CVertBufMgr::getCurrentMaxFontIndices()
{
    return m_currentMaxFontIndices;
}