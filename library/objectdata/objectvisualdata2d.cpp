
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.cpp
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <objectdata/objectvisualdata2d.h>

// Game lib dependencies
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/spritesheetmanager.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <common/build_defs.h>
#include <common/defs.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <iostream>
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectVisualData2D::CObjectVisualData2D() :
    m_vbo(0),
    m_ibo(0),
    m_genType(NDefs::EGT_NULL),
    m_textureSequenceCount(0),
    m_compressed(false),
    m_iboCount(0),
    m_vertexScale(1,1),
    m_defaultUniformScale(1),
    m_mirror(NDefs::EM_NULL)
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CObjectVisualData2D::~CObjectVisualData2D()
{
}


/************************************************************************
*    DESC:  Load the object data from node
************************************************************************/
void CObjectVisualData2D::loadFromNode( const XMLNode & objectNode )
{
    const XMLNode visualNode = objectNode.getChildNode( "visual" );
    if( !visualNode.isEmpty() )
    {
        if( NBDefs::IsMobileDevice() )
        {
            // See if we are to swap graphics based on screen resolution
            const XMLNode resSwapNode = visualNode.getChildNode("graphicResSwap");
            if( !resSwapNode.isEmpty() )
            {
                for( int i = 0; i < resSwapNode.nChildNode(); ++i )
                {
                    // Get the object data node
                    const XMLNode resNode = resSwapNode.getChildNode(i);

                    // Get the value of the height that is compaired against the render resolution of the device
                    const int heightOrLess = std::atoi( resNode.getAttribute( "heightOrLess" ) );

                    // If the current render resolution is less then or equil to the heightOrLess value, we found the swap res
                    if( static_cast<int>(CSettings::Instance().getSize().getH()) <= heightOrLess )
                    {
                        m_defaultUniformScale = std::atof( resNode.getAttribute( "defaultUniformScale" ) );

                        // File extension for these graphics
                        m_resExt = resNode.getAttribute( "ext" );

                        break;
                    }
                }
            }
        }

        // See if we have a texture to load
        const XMLNode textureNode = visualNode.getChildNode("texture");
        if( !textureNode.isEmpty() )
        {
            if( textureNode.isAttributeSet("count") )
                m_textureSequenceCount = std::atoi( textureNode.getAttribute( "count" ) );

            if( textureNode.isAttributeSet("file") )
                m_textureFilePath = textureNode.getAttribute( "file" );

            // Is this a compressed texture?
            if( textureNode.isAttributeSet("compressed") )
                m_compressed = (std::strcmp(textureNode.getAttribute( "compressed" ), "true") == 0);
        }

        // Get the mesh node
        const XMLNode meshNode = visualNode.getChildNode( "mesh" );
        if( !meshNode.isEmpty() )
        {
            if( meshNode.isAttributeSet("genType") )
            {
                std::string genTypeStr = meshNode.getAttribute( "genType" );

                if( genTypeStr == "quad" )
                    m_genType = NDefs::EGT_QUAD;

                else if( genTypeStr == "sprite_sheet" )
                    m_genType = NDefs::EGT_SPRITE_SHEET;

                else if( genTypeStr == "scaled_frame" )
                    m_genType = NDefs::EGT_SCALED_FRAME;

                else if( genTypeStr == "mesh_file" )
                    m_genType = NDefs::EGT_MESH_FILE;

                else if( genTypeStr == "font" )
                    m_genType = NDefs::EGT_FONT;
            }

            if( meshNode.isAttributeSet("file") )
                m_meshFilePath = meshNode.getAttribute( "file" );

            if( meshNode.isAttributeSet("mirror") )
            {
                std::string mirrorTypeStr = meshNode.getAttribute( "mirror" );

                if( mirrorTypeStr == "horizontal" )
                    m_mirror = NDefs::EM_HORIZONTAL;

                else if( mirrorTypeStr == "vertical" )
                    m_mirror = NDefs::EM_VERTICAL;

                else if( mirrorTypeStr == "horizontal_vertical" )
                    m_mirror = NDefs::EM_HORIZONTAL_VERTICAL;
            }

            const XMLNode spriteSheetNode = meshNode.getChildNode("spriteSheet");
            if( !spriteSheetNode.isEmpty() )
            {
                if( spriteSheetNode.isAttributeSet("defIndex") )
                    m_spriteSheet.setDefaultIndex( std::atoi( spriteSheetNode.getAttribute( "defIndex" ) ) );

                // Make sure all elements are defined for manually building the sprite sheet data
                if( spriteSheetNode.isAttributeSet("glyphCount") )
                {
                    m_spriteSheet.setGlyphCount( std::atoi( spriteSheetNode.getAttribute( "glyphCount" ) ) );

                    if( spriteSheetNode.isAttributeSet("columns") )
                    {
                        m_spriteSheet.setGlyphColumns( std::atoi( spriteSheetNode.getAttribute( "columns" ) ) );
                    }
                }

                if( spriteSheetNode.isAttributeSet("formatCodeOffset") )
                    m_spriteSheet.setFormatCodeOffset( std::atoi( spriteSheetNode.getAttribute( "formatCodeOffset" ) ) );

                bool loadAllGlyphs(false);
                if( spriteSheetNode.isAttributeSet("loadAllGlyphs") )
                    loadAllGlyphs = ( std::strcmp( spriteSheetNode.getAttribute("loadAllGlyphs"), "true" ) == 0 );

                // Get the sprite sheet glyph file
                if( spriteSheetNode.isAttributeSet("file") )
                    m_spriteSheetFilePath = spriteSheetNode.getAttribute( "file" );

                // See if any glyph Id's have been defined
                m_glyphIDs.reserve(spriteSheetNode.nChildNode());
                for( int i = 0; i < spriteSheetNode.nChildNode(); ++i )
                    m_glyphIDs.push_back( spriteSheetNode.getChildNode(i).getAttribute( "id" ) );

                // Build the sprite sheet from XML data
                if( !m_spriteSheetFilePath.empty() )
                {
                    // Make a copy of the file path because we may need to add a resource extension to it
                    std::string filePath = m_spriteSheetFilePath;

                    // Add in the resource swap file extension if needed
                    if( !m_resExt.empty() )
                        NGenFunc::AddFileExt( m_spriteSheetFilePath, filePath, m_resExt );

                    // This will return the sprite sheet
                    auto rSpriteSheet = CSpriteSheetMgr::Instance().load( filePath );

                    // Copy the needed glyph data from the manager
                    rSpriteSheet.copyTo( m_spriteSheet, m_glyphIDs, loadAllGlyphs );
                }
            }

            const XMLNode scaledFrameNode = meshNode.getChildNode( "scaledFrame" );
            if( !scaledFrameNode.isEmpty() )
            {
                m_scaledFrame.m_frame.w = std::atoi(scaledFrameNode.getAttribute( "thicknessWidth" ));
                m_scaledFrame.m_frame.h = std::atoi(scaledFrameNode.getAttribute( "thicknessHeight" ));

                if( scaledFrameNode.isAttributeSet("centerQuad") )
                    m_scaledFrame.m_centerQuad = (std::strcmp(scaledFrameNode.getAttribute( "centerQuad" ), "false") != 0);

                if( scaledFrameNode.isAttributeSet("frameBottom") )
                    m_scaledFrame.m_bottomFrame = (std::strcmp(scaledFrameNode.getAttribute( "frameBottom" ), "false") != 0);
            }
        }

        // Check for color
        m_color = NParseHelper::LoadColor( visualNode, m_color );

        // The shader node determines which shader to use
        const XMLNode shaderNode = visualNode.getChildNode( "shader" );
        if( !shaderNode.isEmpty() )
        {
            m_shaderID = shaderNode.getAttribute( "id" );
        }

        // Raise an exception if there's a genType but no shader
        if( (m_genType != NDefs::EGT_NULL) && m_shaderID.empty() )
        {
            throw NExcept::CCriticalException("Shader effect or techique not set!",
                boost::str( boost::format("Shader object data missing.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Load the image data from file
************************************************************************/
void CObjectVisualData2D::loadImage( const std::string & group )
{
    if( !m_textureFilePath.empty() )
    {
        if( m_textureSequenceCount > 0 )
        {
            m_textureIDVec.reserve( m_textureSequenceCount );

            for( int i = 0; i < m_textureSequenceCount; ++i )
            {
                const std::string file = boost::str( boost::format(m_textureFilePath) % i );

                std::string filePath = file;

                // Add in the resource swap file extension if needed
                if( !m_resExt.empty() )
                    NGenFunc::AddFileExt( file, filePath, m_resExt );

                CTextureMgr::Instance().loadImageFor2D( group, filePath );
            }
        }
        else
        {
            std::string filePath = m_textureFilePath;

            // Add in the resource swap file extension if needed
            if( !m_resExt.empty() )
                NGenFunc::AddFileExt( m_textureFilePath, filePath, m_resExt );

            CTextureMgr::Instance().loadImageFor2D( group, filePath );
        }
    }
}


/************************************************************************
*    DESC:  Create the object from data
************************************************************************/
void CObjectVisualData2D::createFromData( const std::string & group, CSize<int> & rSize )
{
    CTexture texture;

    // Create the texture from loaded image data
    createTexture( group, texture, rSize );

    if( m_genType == NDefs::EGT_QUAD )
    {
        // Generate a quad
        generateQuad( group );

        // For this generation type, the image size is the default scale
        // Size is an int and needs to handled this way
        m_vertexScale = rSize * m_defaultUniformScale;
        rSize = m_vertexScale;
    }
    // Load object data defined as a sprite sheet
    else if( m_genType == NDefs::EGT_SPRITE_SHEET )
    {
        // Build the simple (grid) sprite sheet from XML data
        if( m_spriteSheetFilePath.empty() )
            m_spriteSheet.build( rSize );

        // Generate a quad
        generateQuad( group );

        // For this generation type, the glyph size is the default scale
        rSize = m_vertexScale = m_spriteSheet.getGlyph().getSize() * m_defaultUniformScale;
    }
    else if( m_genType == NDefs::EGT_SCALED_FRAME )
    {
        if( !m_glyphIDs.empty() && !m_spriteSheetFilePath.empty() )
        {
            // Get the glyph to make the frame with
            auto rGlyph = m_spriteSheet.findGlyph( m_glyphIDs.back() );

            // Create the scaled frame using glyph info
            if( m_meshFilePath.empty() )
                generateScaledFrame( group, texture.getSize(), rGlyph.getSize(), rSize, rGlyph.getUV() );
            else
                generateScaledFrameMeshFile( group, texture.getSize(), rGlyph.getSize(), rSize, rGlyph.getUV() );
        }
        // Generate a scaled frame
        else if( m_meshFilePath.empty() )
            generateScaledFrame( group, texture.getSize(), texture.getSize(), rSize, CRect<float>() );

        else
            generateScaledFrameMeshFile( group, texture.getSize(), texture.getSize(), rSize, CRect<float>() );
    }
}


/************************************************************************
*    DESC:  Create the texture from loaded image data
************************************************************************/
void CObjectVisualData2D::createTexture( const std::string & group, CTexture & rTexture, CSize<int> & rSize )
{
    if( !m_textureFilePath.empty() )
    {
        if( m_textureSequenceCount > 0 )
        {
            m_textureIDVec.reserve( m_textureSequenceCount );

            for( int i = 0; i < m_textureSequenceCount; ++i )
            {
                const std::string file = boost::str( boost::format(m_textureFilePath) % i );

                std::string filePath = file;

                // Add in the resource swap file extension if needed
                if( !m_resExt.empty() )
                    NGenFunc::AddFileExt( file, filePath, m_resExt );

                rTexture = CTextureMgr::Instance().createTextureFor2D( group, filePath, m_compressed );
                m_textureIDVec.push_back( rTexture.getID() );
            }
        }
        else
        {
            std::string filePath = m_textureFilePath;

            // Add in the resource swap file extension if needed
            if( !m_resExt.empty() )
                NGenFunc::AddFileExt( m_textureFilePath, filePath, m_resExt );

            rTexture = CTextureMgr::Instance().createTextureFor2D( group, filePath, m_compressed );
            m_textureIDVec.push_back( rTexture.getID() );
        }

        // If the passed in size reference is empty, set it to the texture size
        if( rSize.isEmpty() )
            rSize = rTexture.getSize();
    }
}


/************************************************************************
*    DESC:  Generate a quad
************************************************************************/
void CObjectVisualData2D::generateQuad( const std::string & group )
{
    uint8_t indexData[] = {0, 1, 2, 3};

    // VBO data
    // The order of the verts is counter clockwise
    // 1----0
    // |   /|
    // |  / |
    // | /  |
    // 2----3
    std::vector<CVertex2D> vertVec =
    {
        {{ 0.5f,  0.5f, 0.0},  {1.0, 0.0}},
        {{-0.5f,  0.5f, 0.0},  {0.0, 0.0}},
        {{-0.5f, -0.5f, 0.0},  {0.0, 1.0}},
        {{ 0.5f, -0.5f, 0.0},  {1.0, 1.0}}
    };

    std::string horzStr = "";
    std::string vertStr = "";

    if( (m_mirror == NDefs::EM_HORIZONTAL) || (m_mirror == NDefs::EM_HORIZONTAL_VERTICAL) )
    {
        horzStr = "_horz";

        vertVec[0].uv.u = 0.0;
        vertVec[1].uv.u = 1.0;
        vertVec[2].uv.u = 1.0;
        vertVec[3].uv.u = 0.0;
    }

    if( (m_mirror == NDefs::EM_VERTICAL) || (m_mirror == NDefs::EM_HORIZONTAL_VERTICAL) )
    {
        vertStr = "_vert";

        vertVec[0].uv.v = 1.0;
        vertVec[1].uv.v = 1.0;
        vertVec[2].uv.v = 0.0;
        vertVec[3].uv.v = 0.0;
    }

    m_vbo = CVertBufMgr::Instance().createVBO( group, "quad_0011" + horzStr + vertStr, vertVec );
    m_ibo = CVertBufMgr::Instance().createIBO( group, "quad_0123", indexData, sizeof(indexData) );

    // A quad has 4 ibos
    m_iboCount = 4;
}


/************************************************************************
*    DESC:  Generate a scaled frame
************************************************************************/
void CObjectVisualData2D::generateScaledFrame(
    const std::string & group,
    const CSize<int> & textureSize,
    const CSize<int> & glyphSize,
    const CSize<int> & frameSize,
    const CRect<float> & textureOffset )
{
    std::string vboName = boost::str( boost::format("scaled_frame_%d_%d_%d_%d_%d_%d_%d_%d")
        % frameSize.w % frameSize.h % m_scaledFrame.m_frame.w % m_scaledFrame.m_frame.h % textureSize.w % textureSize.h % glyphSize.w % glyphSize.h );

    m_vbo = CVertBufMgr::Instance().createScaledFrame(
        group, vboName, m_scaledFrame, textureSize, glyphSize, frameSize, textureOffset, std::vector<CVertex2D>() );

    uint8_t indexData[] = {
        0,1,2,     0,3,1,
        2,4,5,     2,1,4,
        1,6,4,     1,7,6,
        7,8,6,     7,9,8,
        10,9,7,    10,11,9,
        12,11,10,  12,13,11,
        14,10,3,   14,12,10,
        15,3,0,    15,14,3,
        3,7,1,     3,10,7 };

    // Create the reusable IBO buffer
    m_ibo = CVertBufMgr::Instance().createIBO( group, "scaled_frame", indexData, sizeof(indexData) );

    // Set the ibo count depending on the number of quads being rendered
    // If the center quad is not used, just adjust the ibo count because
    // the center quad is just reused verts anyways and is that last 6 in the IBO
    // If the frame bottom is not being use, just subtract.
    // Center quad and no frame bottom can't co-exist.
	m_iboCount = 6 * 8;
    if( m_scaledFrame.m_centerQuad )
	m_iboCount += 6;

    else if( !m_scaledFrame.m_bottomFrame )
	m_iboCount -= 6 * 3;
}


/************************************************************************
*    DESC:  Generate a scaled frame with a mesh file
************************************************************************/
void CObjectVisualData2D::generateScaledFrameMeshFile(
    const std::string & group,
    const CSize<int> & textureSize,
    const CSize<int> & glyphSize,
    const CSize<int> & frameSize,
    const CRect<float> & textureOffset )
{
    // Construct the name used for vbo and ibo
    std::string name = "scaled_frame_mesh_" + m_meshFilePath;

    std::vector<uint8_t> iboVec = {
        0,1,2,     0,3,1,
        2,4,5,     2,1,4,
        1,6,4,     1,7,6,
        7,8,6,     7,9,8,
        10,9,7,    10,11,9,
        12,11,10,  12,13,11,
        14,10,3,   14,12,10,
        15,3,0,    15,14,3 };

    if( m_scaledFrame.m_centerQuad )
    {
        std::vector<uint8_t> exraVec = { 3,7,1, 3,10,7 };
        iboVec.insert( iboVec.end(), exraVec.begin(), exraVec.end() );
    }

    // See if it already exists before loading the mesh file
    m_vbo = CVertBufMgr::Instance().isVBO( group, name );
    if( m_vbo == 0 )
    {
        std::vector<CVertex2D> vertVec;

        // Load a mesh from XML file
        loadMeshFromXML( group, textureSize, frameSize, textureOffset, 16, vertVec, iboVec );

        // create the vbo
        m_vbo = CVertBufMgr::Instance().createScaledFrame(
            group, name, m_scaledFrame, textureSize, glyphSize, frameSize, textureOffset, vertVec );
    }

    // Create the unique IBO buffer
    m_ibo = CVertBufMgr::Instance().createIBO( group, name, iboVec.data(), sizeof(uint8_t)*iboVec.size() );
    m_iboCount = iboVec.size();
}


/************************************************************************
*    DESC:  Generate a mesh file
************************************************************************/
void CObjectVisualData2D::generateFromMeshFile(
    const std::string & group, const CSize<int> & textureSize, const CSize<int> & size )
{
    std::vector<uint8_t> iboVec;

    // Construct the name used for vbo and ibo
    std::string name = "mesh_file_" + m_meshFilePath;

    // See if it already exists before loading the mesh file
    m_vbo = CVertBufMgr::Instance().isVBO( group, name );
    if( m_vbo == 0 )
    {
        std::vector<CVertex2D> vertVec;

        // Load a mesh from XML file
        loadMeshFromXML( group, textureSize, size, CRect<float>(), 16, vertVec, iboVec );

        // create the vbo
        m_vbo = CVertBufMgr::Instance().createVBO( group, name, vertVec );
    }

    // Create the unique IBO buffer
    m_ibo = CVertBufMgr::Instance().createIBO( group, name, iboVec.data(), sizeof(uint8_t)*iboVec.size() );
    m_iboCount = iboVec.size();
}


/************************************************************************
*    DESC:  Load a mesh from XML file
************************************************************************/
void CObjectVisualData2D::loadMeshFromXML(
    const std::string & group,
    const CSize<int> & textureSize,
    const CSize<int> & size,
    const CRect<float> & textureOffset,
    int iboOffset,
    std::vector<CVertex2D> & rVertVec,
    std::vector<uint8_t> & rIboVec )
{
    float additionalOffsetX = 0;
    if( (int)size.getW() % 2 != 0 )
        additionalOffsetX = 0.5f;

    float additionalOffsetY = 0;
    if( (int)size.getH() % 2 != 0 )
        additionalOffsetY = 0.5f;

    // This converts the data to a center aligned vertex buffer
    const CSize<float> centerAlignSize(-(size.w / 2), (size.h / 2));

    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( m_meshFilePath.c_str(), "mesh" );
    const XMLNode vboNode = mainNode.getChildNode( "vbo" );
    if( !vboNode.isEmpty() )
    {
        CVertex2D vert;

        rVertVec.reserve( vboNode.nChildNode() );

        for( int i = 0; i < vboNode.nChildNode(); ++i )
        {
            // Load the 2D vert
            vert = NParseHelper::LoadVertex2d( vboNode.getChildNode( "vert", i ) );

            // This converts the data to a center aligned vertex buffer
            vert.vert.x = centerAlignSize.w + vert.vert.x + additionalOffsetX;
            vert.vert.y = centerAlignSize.h - vert.vert.y + additionalOffsetY;
            vert.uv.u = textureOffset.x1 + (vert.uv.u / textureSize.w);
            vert.uv.v = textureOffset.y1 + (vert.uv.v / textureSize.h);

            rVertVec.emplace_back( vert );
        }
    }

    const XMLNode iboNode = mainNode.getChildNode( "ibo" );
    if( !iboNode.isEmpty() )
    {
        for( int i = 0; i < iboNode.nChildNode(); ++i )
        {
            const XMLNode iNode = iboNode.getChildNode( "i", i );

            rIboVec.push_back( iboOffset + std::atoi(iNode.getText()) );
        }
    }
}


/************************************************************************
*    DESC:  Get the gne type
************************************************************************/
NDefs::EGenerationType CObjectVisualData2D::getGenerationType() const
{
    return m_genType;
}


/************************************************************************
*    DESC:  Get the texture ID
************************************************************************/
uint32_t CObjectVisualData2D::getTextureID( uint index ) const
{
    if( m_textureIDVec.empty() )
        return 0;
    else
        return m_textureIDVec[index];
}


/************************************************************************
*    DESC:  Get the name of the shader ID
************************************************************************/
const std::string & CObjectVisualData2D::getShaderID() const
{
    return m_shaderID;
}


/************************************************************************
*    DESC:  Get the color
************************************************************************/
const CColor & CObjectVisualData2D::getColor() const
{
    return m_color;
}


/************************************************************************
*    DESC:  Get the VBO
************************************************************************/
uint32_t CObjectVisualData2D::getVBO() const
{
    return m_vbo;
}


/************************************************************************
*    DESC:  Get the IBO
************************************************************************/
uint32_t CObjectVisualData2D::getIBO() const
{
    return m_ibo;
}


/************************************************************************
*    DESC:  Get the vertex count
************************************************************************/
int CObjectVisualData2D::getIBOCount() const
{
    return m_iboCount;
}


/************************************************************************
*    DESC:  Get the frame count
************************************************************************/
size_t CObjectVisualData2D::getFrameCount() const
{
    if( m_genType == NDefs::EGT_SPRITE_SHEET )
        return m_spriteSheet.getCount();

    return m_textureIDVec.size();
}


/************************************************************************
*    DESC:  Get the vertex scale
************************************************************************/
const CSize<float> & CObjectVisualData2D::getVertexScale() const
{
    return m_vertexScale;
}


/************************************************************************
*    DESC:  Whether or not the visual tag was specified
************************************************************************/
bool CObjectVisualData2D::isActive() const
{
    return (m_genType != NDefs::EGT_NULL);
}


/************************************************************************
*    DESC:  Get the sprite sheet
************************************************************************/
const CSpriteSheet & CObjectVisualData2D::getSpriteSheet() const
{
    return m_spriteSheet;
}


/************************************************************************
*    DESC:  Access functions for the default scale
************************************************************************/
float CObjectVisualData2D::getDefaultUniformScale() const
{
    return m_defaultUniformScale;
}
