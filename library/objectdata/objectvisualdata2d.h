
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_2d_h__
#define __object_visual_data_2d_h__

// Game lib dependencies
#include <common/size.h>
#include <common/color.h>
#include <common/rect.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/face2d.h>
#include <common/scaledframe.h>
#include <common/texture.h>
#include <common/spritesheet.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
struct XMLNode;

class CObjectVisualData2D
{
public:

    // Constructor/Destructor
    CObjectVisualData2D();
    ~CObjectVisualData2D();

    // Load thes object data from node
    void loadFromNode( const XMLNode & objectNode );

    // Create the object from data
    void createFromData( const std::string & group, CSize<int> & rSize );

    // Get the gne type
    NDefs::EGenerationType getGenerationType() const;

    // Get the texture ID
    uint32_t getTextureID( uint index = 0 ) const;

    // Get the name of the shader ID
    const std::string & getShaderID() const;

    // Get the color
    const CColor & getColor() const;

    // Get the vertex scale
    const CSize<float> & getVertexScale() const;

    // Get the VBO
    uint32_t getVBO() const;

    // Get the IBO
    uint32_t getIBO() const;

    // Get the ibo count
    int getIBOCount() const;

    // Get the frame count
    size_t getFrameCount() const;

    // Whether or not the visual tag was specified
    bool isActive() const;
    
    // Get the sprite sheet
    const CSpriteSheet & getSpriteSheet() const;
    
    // Access functions for the default uniform scale
    float getDefaultUniformScale() const;

private:
    
    // Create the texture from loaded image data
    void createTexture( const std::string & group, CTexture & rTexture, CSize<int> & rSize );
    
    // Generate a quad
    void generateQuad( const std::string & group );
    
    // Generate a scaled frame
    void generateScaledFrame(
        const std::string & group,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & textureOffset );
    
    // Generate a scaled frame with a mesh file
    void generateScaledFrameMeshFile( 
        const std::string & group,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & textureOffset );
    
    // Generate a mesh file
    void generateFromMeshFile( 
        const std::string & group, const CSize<int> & textureSize, const CSize<int> & size );
    
    // Load a mesh from XML file
    void loadMeshFromXML(
        const std::string & group,
        const CSize<int> & textureSize,
        const CSize<int> & size,
        const CRect<float> & textureOffset,
        int iboOffset,
        std::vector<CVertex2D> & rVertVec,
        std::vector<uint8_t> & rIboVec );
    
private:

    // Loaded texture data
    std::vector<CTexture> m_textureVec;

    // VBO
    uint32_t m_vbo;

    // IBO
    uint32_t m_ibo;

    // VBO/IBO generation type
    NDefs::EGenerationType m_genType;

    // Name of the shader
    std::string m_shaderID;

    // Initial color of the object
    CColor m_color;

    // texture file path
    std::string m_textureFilePath;
    
    // Texture Sequence count
    int m_textureSequenceCount;

    // Compressed flag
    bool m_compressed;

    // mesh file path
    std::string m_meshFilePath;

    // ibo count
    int m_iboCount;

    // The vertex scale of the object
    CSize<float> m_vertexScale;

    // Scaled frame
    CScaledFrame m_scaledFrame;
    
    // Sprite Sheet
    CSpriteSheet m_spriteSheet;
    
    // Sprite sheet file path
    std::string m_spriteSheetFilePath;
    
    // String of glyph ids
    std::vector<std::string> m_glyphIDs;
    
    // Load all glyphs flag
    bool m_loadAllGlyphs;
    
    // Default scale
    float m_defaultUniformScale;
    
    // Mirror enum
    NDefs::EMirror m_mirror;
    
    // File extension for resolution swap
    std::string m_resExt;
};

#endif  // __object_visual_data_2d_h__
