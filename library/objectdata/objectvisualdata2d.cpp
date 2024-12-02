
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.cpp
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

// Physical component dependency
#include <objectdata/objectvisualdata2d.h>

// Game lib dependencies
#include <system/device.h>
#include <managers/spritesheetmanager.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <common/defs.h>
#include <common/quad2d.h>
#include <common/scaledframe.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

// Declare some static
CTexture CObjectVisualData2D::m_null_texture;

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectVisualData2D::CObjectVisualData2D() :
    m_genType(EGenType::_NULL_),
    m_pipelineIndex(-1),
    m_textureSequenceCount(0),
    // The below defaults need to match the defaults in texture.h
    m_genMipLevels(false),
    m_magFilter(VK_FILTER_LINEAR),
    m_minFilter(VK_FILTER_LINEAR),
    m_samplerAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE),
    m_samplerAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE),
    m_samplerAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE),
    m_borderColor(VK_BORDER_COLOR_INT_OPAQUE_BLACK),
    m_compareOp(VK_COMPARE_OP_ALWAYS),
    m_mipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR),
    m_unnormalizedCoordinates(VK_FALSE),
    m_compareEnable(VK_FALSE),
    m_mipLodBias(0.0f),
    m_minLod(0.0f),
    m_iboCount(0),
    m_defaultUniformScale(1),
    m_mirror(EMirror::_NULL_)
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
void CObjectVisualData2D::loadFromNode( const XMLNode & objectNode, const std::string & name )
{
    m_name = name;
    
    const XMLNode visualNode = objectNode.getChildNode( "visual" );
    if( !visualNode.isEmpty() )
    {
        if( visualNode.isAttributeSet("defaultUniformScale") )
            m_defaultUniformScale = std::atof( visualNode.getAttribute( "defaultUniformScale" ) );

        // See if we have a texture to load
        const XMLNode textureNode = visualNode.getChildNode("texture");
        if( !textureNode.isEmpty() )
        {
            if( textureNode.isAttributeSet("count") )
                m_textureSequenceCount = std::atoi( textureNode.getAttribute( "count" ) );

            if( textureNode.isAttributeSet("file") )
                m_textureFilePath = textureNode.getAttribute( "file" );

            // Enable mip map generation
            if( textureNode.isAttributeSet("mip_levels") )
                m_genMipLevels = (std::strcmp(textureNode.getAttribute( "mip_levels" ), "true") == 0);

            // Select the filtering
            if( textureNode.isAttributeSet("mag_filter") )
            {
                std::string filterStr = textureNode.getAttribute( "mag_filter" );

                if( filterStr == "nearest" )
                    m_magFilter = VK_FILTER_NEAREST;
                
                else if( filterStr == "linear" )
                    m_magFilter = VK_FILTER_LINEAR;

                else if( filterStr == "cubic_ext" )
                    m_magFilter = VK_FILTER_CUBIC_EXT;

                else if( filterStr == "cubic_img" )
                    m_magFilter = VK_FILTER_CUBIC_IMG;
            }

            if( textureNode.isAttributeSet("min_filter") )
            {
                std::string filterStr = textureNode.getAttribute( "min_filter" );

                if( filterStr == "nearest" )
                    m_minFilter = VK_FILTER_NEAREST;
                
                else if( filterStr == "linear" )
                    m_minFilter = VK_FILTER_LINEAR;

                else if( filterStr == "cubic_ext" )
                    m_minFilter = VK_FILTER_CUBIC_EXT;

                else if( filterStr == "cubic_img" )
                    m_minFilter = VK_FILTER_CUBIC_IMG;
            }

            if( textureNode.isAttributeSet("samp_mode_u") )
            {
                std::string sampAddrModeStr = textureNode.getAttribute( "samp_mode_u" );

                if( sampAddrModeStr == "repeat" )
                    m_samplerAddressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                
                else if( sampAddrModeStr == "mirrored_repeat" )
                    m_samplerAddressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

                else if( sampAddrModeStr == "clamp_to_edge" )
                    m_samplerAddressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

                else if( sampAddrModeStr == "clamp_to_border" )
                    m_samplerAddressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
                
                else if( sampAddrModeStr == "mirror_clamp_to_edge" )
                    m_samplerAddressModeU = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            }

            if( textureNode.isAttributeSet("samp_mode_v") )
            {
                std::string sampAddrModeStr = textureNode.getAttribute( "samp_mode_v" );

                if( sampAddrModeStr == "repeat" )
                    m_samplerAddressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                
                else if( sampAddrModeStr == "mirrored_repeat" )
                    m_samplerAddressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

                else if( sampAddrModeStr == "clamp_to_edge" )
                    m_samplerAddressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

                else if( sampAddrModeStr == "clamp_to_border" )
                    m_samplerAddressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
                
                else if( sampAddrModeStr == "mirror_clamp_to_edge" )
                    m_samplerAddressModeV = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            }

            if( textureNode.isAttributeSet("samp_mode_w") )
            {
                std::string sampAddrModeStr = textureNode.getAttribute( "samp_mode_w" );

                if( sampAddrModeStr == "repeat" )
                    m_samplerAddressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                
                else if( sampAddrModeStr == "mirrored_repeat" )
                    m_samplerAddressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

                else if( sampAddrModeStr == "clamp_to_edge" )
                    m_samplerAddressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

                else if( sampAddrModeStr == "clamp_to_border" )
                    m_samplerAddressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
                
                else if( sampAddrModeStr == "mirror_clamp_to_edge" )
                    m_samplerAddressModeW = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            }

            if( textureNode.isAttributeSet("border_color") )
            {
                std::string borderColorStr = textureNode.getAttribute( "border_color" );

                if( borderColorStr == "float_trans_black" )
                    m_borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
                
                else if( borderColorStr == "int_trans_black" )
                    m_borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;

                else if( borderColorStr == "float_opaque_black" )
                    m_borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

                else if( borderColorStr == "int_opaque_black" )
                    m_borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
                
                else if( borderColorStr == "float_opaque_white" )
                    m_borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
                
                else if( borderColorStr == "int_opaque_white" )
                    m_borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;

                else if( borderColorStr == "float_custom_ext" )
                    m_borderColor = VK_BORDER_COLOR_FLOAT_CUSTOM_EXT;

                else if( borderColorStr == "int_custom_ext" )
                    m_borderColor = VK_BORDER_COLOR_INT_CUSTOM_EXT;
            }

            if( textureNode.isAttributeSet("compare_op") )
            {
                std::string borderColorStr = textureNode.getAttribute( "compare_op" );

                if( borderColorStr == "never" )
                    m_compareOp = VK_COMPARE_OP_NEVER;
                
                else if( borderColorStr == "less" )
                    m_compareOp = VK_COMPARE_OP_LESS;

                else if( borderColorStr == "equal" )
                    m_compareOp = VK_COMPARE_OP_EQUAL;

                else if( borderColorStr == "less_or_equal" )
                    m_compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                
                else if( borderColorStr == "greater" )
                    m_compareOp = VK_COMPARE_OP_GREATER;
                
                else if( borderColorStr == "not_equal" )
                    m_compareOp = VK_COMPARE_OP_NOT_EQUAL;

                else if( borderColorStr == "greater_or_equal" )
                    m_compareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;

                else if( borderColorStr == "always" )
                    m_compareOp = VK_COMPARE_OP_ALWAYS;
            }

            if( textureNode.isAttributeSet("mipmap_mode") )
            {
                std::string borderColorStr = textureNode.getAttribute( "mipmap_mode" );

                if( borderColorStr == "nearest" )
                    m_mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
                
                else if( borderColorStr == "linear" )
                    m_mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            }

            if( textureNode.isAttributeSet("unnorm_coord") )
                m_unnormalizedCoordinates = (std::strcmp(textureNode.getAttribute( "unnorm_coord" ), "true") == 0);

            if( textureNode.isAttributeSet("compare_enable") )
                m_compareEnable = (std::strcmp(textureNode.getAttribute( "compare_enable" ), "true") == 0);

            if( textureNode.isAttributeSet("mip_lod_bias") )
                m_mipLodBias = std::atof(textureNode.getAttribute( "mip_lod_bias" ));
            
            if( textureNode.isAttributeSet("min_lod") )
                m_minLod = std::atof(textureNode.getAttribute( "min_lod" ));
        }

        // Get the mesh node
        const XMLNode meshNode = visualNode.getChildNode( "mesh" );
        if( !meshNode.isEmpty() )
        {
            if( meshNode.isAttributeSet("genType") )
            {
                std::string genTypeStr = meshNode.getAttribute( "genType" );

                if( genTypeStr == "quad" )
                    m_genType = EGenType::QUAD;

                else if( genTypeStr == "sprite_sheet" )
                    m_genType = EGenType::SPRITE_SHEET;

                else if( genTypeStr == "scaled_frame" )
                    m_genType = EGenType::SCALED_FRAME;

                else if( genTypeStr == "mesh_file" )
                    m_genType = EGenType::MESH_FILE;

                else if( genTypeStr == "font" )
                    m_genType = EGenType::FONT;
                
                else if( genTypeStr == "null" )
                    m_genType = EGenType::_NULL_;
            }

            if( meshNode.isAttributeSet("file") )
                m_meshFilePath = meshNode.getAttribute( "file" );

            if( meshNode.isAttributeSet("mirror") )
            {
                std::string mirrorTypeStr = meshNode.getAttribute( "mirror" );

                if( mirrorTypeStr == "horizontal" )
                    m_mirror = EMirror::HORIZONTAL;

                else if( mirrorTypeStr == "vertical" )
                    m_mirror = EMirror::VERTICAL;

                else if( mirrorTypeStr == "horizontal_vertical" )
                    m_mirror = EMirror::HORIZONTAL_VERTICAL;
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
                    // This will return the sprite sheet
                    auto rSpriteSheet = CSpriteSheetMgr::Instance().load( m_spriteSheetFilePath );

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

        // The pipeline node determines which pipeline to use
        const XMLNode pipelineNode = visualNode.getChildNode( "pipeline" );
        if( !pipelineNode.isEmpty() )
        {
            std::string pipelineId = pipelineNode.getAttribute( "id" );
            m_pipelineIndex = CDevice::Instance().getPipelineIndex( pipelineId );
        }

        // Raise an exception if there's a genType but no valid pipeline index
        if( (m_genType != EGenType::_NULL_) && (m_pipelineIndex == -1) )
        {
            throw NExcept::CCriticalException("Shader effect or techique not set!",
                boost::str( boost::format("Shader object data missing.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Create the object from data
************************************************************************/
void CObjectVisualData2D::createFromData( const std::string & group, CSize<float> & rSize )
{
    CTexture texture;

    // Create the texture from loaded image data
    createTexture( group, texture, rSize );
    
    if( m_genType == EGenType::QUAD )
    {
        // Generate a quad
        generateQuad( group );
    }
    // Load object data defined as a sprite sheet
    else if( m_genType == EGenType::SPRITE_SHEET )
    {
        // Build the simple (grid) sprite sheet from XML data
        if( m_spriteSheetFilePath.empty() )
            m_spriteSheet.build( rSize );

        // Generate a quad
        generateQuad( group );

        // For this generation type, the glyph size is the default scale
        rSize = m_spriteSheet.getGlyph().getSize();
    }
    else if( m_genType == EGenType::SCALED_FRAME )
    {
        if( !m_glyphIDs.empty() && !m_spriteSheetFilePath.empty() )
        {
            // Get the glyph to make the frame with
            auto rGlyph = m_spriteSheet.findGlyph( m_glyphIDs.back() );

            // Create the scaled frame using glyph info
            if( m_meshFilePath.empty() )
                generateScaledFrame( group, texture.size, rGlyph.getSize(), rSize, rGlyph.getUV() );
            else
                generateScaledFrameMeshFile( group, texture.size, rGlyph.getSize(), rSize, rGlyph.getUV() );
        }
        // Generate a scaled frame
        else if( m_meshFilePath.empty() )
            generateScaledFrame( group, texture.size, texture.size, rSize, CRect<float>() );

        else
            generateScaledFrameMeshFile( group, texture.size, texture.size, rSize, CRect<float>() );
    }
}


/************************************************************************
*    DESC:  Generate a quad
************************************************************************/
void CObjectVisualData2D::generateQuad( const std::string & group )
{
    const std::vector<uint16_t> iboVec = { 0, 1, 2, 2, 3, 0 };
    
    // The order of the verts is counter clockwise
    // 1----0
    // |   /|
    // |  / |
    // | /  |
    // 2----3
    std::vector<NVertex::vert_uv> vert_uv_vec =
    {
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}
    };
    
    std::vector<NVertex::vert> vert_vec =
    {
        {{ 0.5f, -0.5f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}}
    };


    std::string horzStr = "";
    std::string vertStr = "";

    if( (m_mirror == EMirror::HORIZONTAL) || (m_mirror == EMirror::HORIZONTAL_VERTICAL) )
    {
        horzStr = "_horz";

        vert_uv_vec[0].uv.u = 0.0;
        vert_uv_vec[1].uv.u = 1.0;
        vert_uv_vec[2].uv.u = 1.0;
        vert_uv_vec[3].uv.u = 0.0;
    }

    if( (m_mirror == EMirror::VERTICAL) || (m_mirror == EMirror::HORIZONTAL_VERTICAL) )
    {
        vertStr = "_vert";

        vert_uv_vec[0].uv.v = 1.0;
        vert_uv_vec[1].uv.v = 1.0;
        vert_uv_vec[2].uv.v = 0.0;
        vert_uv_vec[3].uv.v = 0.0;
    }

    if( m_textureFilePath.empty() )
        m_vboBuffer = CDevice::Instance().creatMemoryBuffer( group, "quad_solid_vbo" + horzStr + vertStr, vert_vec, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
    else
        m_vboBuffer = CDevice::Instance().creatMemoryBuffer( group, "quad_uv_vbo" + horzStr + vertStr, vert_uv_vec, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
    
    m_iboBuffer = CDevice::Instance().creatMemoryBuffer( group, "quad_ibo", iboVec, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );

    // Set the ibo count
    m_iboCount = iboVec.size();
}


/************************************************************************
*    DESC:  Create the texture from loaded image data
************************************************************************/
void CObjectVisualData2D::createTexture( const std::string & group, CTexture & rTexture, CSize<float> & rSize )
{
    if( !m_textureFilePath.empty() )
    {
        // Init texture class
        rTexture.magFilter = m_magFilter;
        rTexture.minFilter = m_minFilter;
        rTexture.genMipLevels = m_genMipLevels;
        rTexture.textFilePath = m_textureFilePath;
        rTexture.samplerAddressModeU = m_samplerAddressModeU;
        rTexture.samplerAddressModeV = m_samplerAddressModeV;
        rTexture.samplerAddressModeW = m_samplerAddressModeW;

        if( m_textureSequenceCount > 0 )
        {
            m_textureVec.reserve( m_textureSequenceCount );

            for( int i = 0; i < m_textureSequenceCount; ++i )
            {
                rTexture.textFilePath = boost::str( boost::format(m_textureFilePath) % i );

                rTexture = CDevice::Instance().createTexture( group, rTexture );
                m_textureVec.emplace_back( rTexture );
            }
        }
        else
        {
            rTexture = CDevice::Instance().createTexture( group, rTexture );
            m_textureVec.emplace_back( rTexture );
        }

        // If the passed in size reference is empty, set it to the texture size
        if( rSize.isEmpty() )
            rSize = rTexture.size;
    }
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
    if( m_scaledFrame.m_frame.w == 0 || m_scaledFrame.m_frame.h == 0 )
        throw NExcept::CCriticalException("Scaled frame value not set!",
            boost::str( boost::format("Frame does not have a value (%s-%s).\n\n%s\nLine: %s")
                % group % m_name % __FUNCTION__ % __LINE__ ));
    
    std::string vboName = boost::str( boost::format("scaled_frame_%d_%d_%d_%d_%d_%d_%d_%d")
        % frameSize.w
        % frameSize.h
        % m_scaledFrame.m_frame.w
        % m_scaledFrame.m_frame.h
        % textureSize.w
        % textureSize.h
        % glyphSize.w
        % glyphSize.h );

    createScaledFrame(
        group,
        vboName,
        m_scaledFrame,
        textureSize,
        glyphSize,
        frameSize,
        textureOffset,
        std::vector<NVertex::vert_uv>() );

    const std::vector<uint16_t> iboVec = {
        0,1,2,     2,3,0,
        4,5,1,     1,0,4,
        6,4,0,     0,7,6,
        8,6,7,     7,9,8,
        10,9,7,    10,11,9,
        11,10,12,  12,13,11,
        10,3,14,   14,12,10,
        3,2,15,    15,14,3,
        7,0,3,     3,10,7 };

    // Create the reusable IBO buffer
    m_iboBuffer = CDevice::Instance().creatMemoryBuffer( group, "scaled_frame", iboVec, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );

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
*    DESC:  Create a scaled frame
*    NOTE: This is a bit of a brute force implementation but writing an
*          algorithm that takes into account an index buffer is difficult
************************************************************************/
void CObjectVisualData2D::createScaledFrame(
    const std::string & group,
    const std::string & id,
    const CScaledFrame & scaledFrame,
    const CSize<int> & textureSize,
    const CSize<int> & glyphSize,
    const CSize<int> & frameSize,
    const CRect<float> & spriteSheetOffset,
    const std::vector<NVertex::vert_uv> & extraVertVec )
{
    // See if it already exists before loading the mesh file
    m_vboBuffer = CDevice::Instance().getMemoryBuffer( group, id );

    // If it's not found, create the vertex buffer and add it to the list
    if( m_vboBuffer.isEmpty() )
    {
        std::vector<NVertex::vert_uv> vertVec;

        // Offsets to center the mesh
        const CPoint<float> center((frameSize.w / 2.f), (frameSize.h / 2.f));
        const CSize<float> frameLgth( (float)frameSize.w - ((float)scaledFrame.m_frame.w * 2.f), (float)frameSize.h - ((float)scaledFrame.m_frame.h * 2.f) );
        const CSize<float> uvLgth( glyphSize.w - ((float)scaledFrame.m_frame.w * 2.f), glyphSize.h - ((float)scaledFrame.m_frame.h * 2.f) );

        CQuad2D quadBuf[8];

        // Left frame
        createQuad( CPoint<float>(-center.x, center.y-(float)scaledFrame.m_frame.h),
                    CSize<float>((float)scaledFrame.m_frame.w, frameLgth.h),
                    CUV(0, (float)scaledFrame.m_frame.h),
                    CSize<float>((float)scaledFrame.m_frame.w, uvLgth.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[0] );

        // top left
        createQuad( CPoint<float>(-center.x, center.y),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    CUV(0, 0),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[1] );

        // top
        createQuad( CPoint<float>(-(center.x-(float)scaledFrame.m_frame.w), center.y),
                    CSize<float>(frameLgth.w, (float)scaledFrame.m_frame.h),
                    CUV((float)scaledFrame.m_frame.w, 0),
                    CSize<float>(uvLgth.w, (float)scaledFrame.m_frame.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[2] );

        // top right
        createQuad( CPoint<float>(center.x-(float)scaledFrame.m_frame.w, center.y),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    CUV((float)scaledFrame.m_frame.w + uvLgth.w,0),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[3] );

        // right frame
        createQuad( CPoint<float>(center.x-scaledFrame.m_frame.w, center.y-scaledFrame.m_frame.h),
                    CSize<float>(scaledFrame.m_frame.w, frameLgth.h),
                    CUV(scaledFrame.m_frame.w + uvLgth.w, scaledFrame.m_frame.h),
                    CSize<float>(scaledFrame.m_frame.w, uvLgth.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[4] );

        // bottom right
        createQuad( CPoint<float>(center.x-(float)scaledFrame.m_frame.w, -(center.y-(float)scaledFrame.m_frame.h)),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    CUV((float)scaledFrame.m_frame.w + uvLgth.w, (float)scaledFrame.m_frame.h + uvLgth.h),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[5] );

        // bottom frame
        createQuad( CPoint<float>(-(center.x-(float)scaledFrame.m_frame.w), -(center.y-(float)scaledFrame.m_frame.h)),
                    CSize<float>(frameLgth.w, (float)scaledFrame.m_frame.h),
                    CUV((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h + uvLgth.h),
                    CSize<float>(uvLgth.w, (float)scaledFrame.m_frame.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[6] );

        // bottom left
        createQuad( CPoint<float>(-center.x, -(center.y-(float)scaledFrame.m_frame.h)),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    CUV(0, (float)scaledFrame.m_frame.h + uvLgth.h),
                    CSize<float>((float)scaledFrame.m_frame.w, (float)scaledFrame.m_frame.h),
                    textureSize,
                    frameSize,
                    spriteSheetOffset,
                    quadBuf[7] );

        // 5----4----6----8
        // |   /|   /|   /|
        // |  / |  / |  / |
        // | /  | /  | /  |
        // 1----0----7----9
        // |   /|   /|   /|
        // |  / |  / |  / |
        // | /  | /  | /  |
        // 2----3---10---11
        // |   /|   /|   /|
        // |  / |  / |  / |
        // | /  | /  | /  |
        // 15--14---12---13
        
        // Piece together the needed unique verts
        vertVec.push_back( quadBuf[0].vert[0] );
        vertVec.push_back( quadBuf[0].vert[1] );
        vertVec.push_back( quadBuf[0].vert[2] );
        vertVec.push_back( quadBuf[0].vert[3] );
        vertVec.push_back( quadBuf[1].vert[0] );
        vertVec.push_back( quadBuf[1].vert[1] );
        vertVec.push_back( quadBuf[2].vert[0] );
        vertVec.push_back( quadBuf[2].vert[3] );
        vertVec.push_back( quadBuf[3].vert[0] );
        vertVec.push_back( quadBuf[3].vert[3] );
        vertVec.push_back( quadBuf[4].vert[2] );
        vertVec.push_back( quadBuf[4].vert[3] );
        vertVec.push_back( quadBuf[5].vert[2] );
        vertVec.push_back( quadBuf[5].vert[3] );
        vertVec.push_back( quadBuf[6].vert[2] );
        vertVec.push_back( quadBuf[7].vert[2] );
        
        for( auto & iter : extraVertVec )
            vertVec.push_back( iter );
        
        m_vboBuffer = CDevice::Instance().creatMemoryBuffer( group, id, vertVec, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
    }
}


/************************************************************************
*    DESC:  Create a quad
************************************************************************/
void CObjectVisualData2D::createQuad(
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
    
    // The order of the verts is counter clockwise
    // 1----0
    // |   /|
    // |  / |
    // | /  |
    // 2----3
    //{{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
    //{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
    //{{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}},
    //{{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}
    
    // Calculate the second vertex of the first face
    quadBuf.vert[1].vert.x = vert.x + additionalOffsetX;
    quadBuf.vert[1].vert.y = -(vert.y + additionalOffsetY);
    quadBuf.vert[1].uv.u = spriteSheetOffset.x1 + (uv.u / textureSize.w);
    quadBuf.vert[1].uv.v = spriteSheetOffset.y1 + (uv.v / textureSize.h);
    
    // Calculate the second vertex of the second face
    quadBuf.vert[3].vert.x = vert.x + additionalOffsetX + vSize.w;
    quadBuf.vert[3].vert.y = vSize.h - vert.y + additionalOffsetY;
    quadBuf.vert[3].uv.u = spriteSheetOffset.x1 + ((uv.u + uvSize.w) / textureSize.w);
    quadBuf.vert[3].uv.v = spriteSheetOffset.y1 + ((uv.v + uvSize.h) / textureSize.h);
    
    // Calculate the first vertex of the first face
    quadBuf.vert[0].vert.x = quadBuf.vert[3].vert.x;
    quadBuf.vert[0].vert.y = quadBuf.vert[1].vert.y;
    quadBuf.vert[0].uv.u = quadBuf.vert[3].uv.u;
    quadBuf.vert[0].uv.v = quadBuf.vert[1].uv.v;

    // Calculate the third vertex of the first face
    quadBuf.vert[2].vert.x = quadBuf.vert[1].vert.x;
    quadBuf.vert[2].vert.y = quadBuf.vert[3].vert.y;
    quadBuf.vert[2].uv.u = quadBuf.vert[1].uv.u;
    quadBuf.vert[2].uv.v = quadBuf.vert[3].uv.v;
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
    // Construct the name used for vbo
    std::string id = "scaled_frame_mesh_" + m_meshFilePath;

    std::vector<uint16_t> iboVec = {
        0,1,2,     2,3,0,
        4,5,1,     1,0,4,
        6,4,0,     0,7,6,
        8,6,7,     7,9,8,
        10,9,7,    10,11,9,
        11,10,12,  12,13,11,
        10,3,14,   14,12,10,
        3,2,15,    15,14,3 };

    if( m_scaledFrame.m_centerQuad )
    {
        std::vector<uint16_t> exraVec = { 7,0,3,  3,10,7 };
        iboVec.insert( iboVec.end(), exraVec.begin(), exraVec.end() );
    }

    // See if it already exists before loading the mesh file
    m_vboBuffer = CDevice::Instance().getMemoryBuffer( group, id );
    if( m_vboBuffer.isEmpty() )
    {
        std::vector<NVertex::vert_uv> loadedVertVec;

        // Load a mesh from XML file
        loadMeshFromXML( group, textureSize, frameSize, textureOffset, 16, loadedVertVec, iboVec );

        // create the vbo
        createScaledFrame(
            group, id, m_scaledFrame, textureSize, glyphSize, frameSize, textureOffset, loadedVertVec );
    }
    
    // Construct the name used for ibo
    id = "scaled_frame_ibo_" + m_meshFilePath;

    // Create the unique IBO buffer
    m_iboBuffer = CDevice::Instance().creatMemoryBuffer( group, id, iboVec, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
    m_iboCount = iboVec.size();
}


/************************************************************************
*    DESC:  Generate a mesh file
************************************************************************/
void CObjectVisualData2D::generateFromMeshFile(
    const std::string & group, const CSize<int> & textureSize, const CSize<int> & size )
{
    std::vector<uint16_t> iboVec;

    // Construct the name used for vbo and ibo
    std::string id = "mesh_file_" + m_meshFilePath;

    // See if it already exists before loading the mesh file
    m_vboBuffer = CDevice::Instance().getMemoryBuffer( group, id );
    if( m_vboBuffer.isEmpty() )
    {
        std::vector<NVertex::vert_uv> loadedVertVec;

        // Load a mesh from XML file
        loadMeshFromXML( group, textureSize, size, CRect<float>(), 16, loadedVertVec, iboVec );

        // create the vbo
        m_vboBuffer = CDevice::Instance().creatMemoryBuffer( group, id, loadedVertVec, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
    }

    // Create the unique IBO buffer
    m_iboBuffer = CDevice::Instance().creatMemoryBuffer( group, id, iboVec, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
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
    std::vector<NVertex::vert_uv> & rVertVec,
    std::vector<uint16_t> & rIboVec )
{
    float additionalOffsetX = 0;
    if( (int)size.getW() % 2 != 0 )
        additionalOffsetX = 0.5f;

    float additionalOffsetY = 0;
    if( (int)size.getH() % 2 != 0 )
        additionalOffsetY = 0.5f;

    // This converts the data to a center aligned vertex buffer
    const CSize<float> centerAlignSize(-(size.w / 2), -(size.h / 2));

    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( m_meshFilePath.c_str(), "mesh" );
    const XMLNode vboNode = mainNode.getChildNode( "vbo" );
    if( !vboNode.isEmpty() )
    {
        NVertex::vert_uv vert;

        rVertVec.reserve( vboNode.nChildNode() );

        for( int i = 0; i < vboNode.nChildNode(); ++i )
        {
            // Load the 2D vert
            vert = NParseHelper::Load_vert_uv( vboNode.getChildNode( "vert", i ) );

            // This converts the data to a center aligned vertex buffer
            vert.vert.x = centerAlignSize.w + vert.vert.x + additionalOffsetX;
            vert.vert.y = centerAlignSize.h + vert.vert.y + additionalOffsetY;
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
*    DESC:  Get the generation type
************************************************************************/
EGenType CObjectVisualData2D::getGenerationType() const
{
    return m_genType;
}


/************************************************************************
*    DESC:  Get the texture
************************************************************************/
const CTexture & CObjectVisualData2D::getTexture( uint index ) const
{
    if( m_textureVec.empty() )
        return m_null_texture;

    return m_textureVec[index];
}


/************************************************************************
*    DESC:  Get the name of the pipeline index
************************************************************************/
int CObjectVisualData2D::getPipelineIndex() const
{
    return m_pipelineIndex;
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
const CMemoryBuffer & CObjectVisualData2D::getVBO() const
{
    return m_vboBuffer;
}


/************************************************************************
*    DESC:  Get the IBO
************************************************************************/
const CMemoryBuffer & CObjectVisualData2D::getIBO() const
{
    return m_iboBuffer;
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
    if( m_genType == EGenType::SPRITE_SHEET )
        return m_spriteSheet.getCount();

    return m_textureVec.size();
}


/************************************************************************
*    DESC:  Whether or not the visual tag was specified
************************************************************************/
bool CObjectVisualData2D::isActive() const
{
    return (m_genType != EGenType::_NULL_);
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
