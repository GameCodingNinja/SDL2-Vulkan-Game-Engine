
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_2d_h__
#define __object_visual_data_2d_h__

// Physical component dependency
#include <objectdata/iobjectvisualdata.h>

// Game lib dependencies
#include <common/size.h>
#include <common/color.h>
#include <common/rect.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/scaledframe.h>
#include <common/texture.h>
#include <common/memorybuffer.h>
#include <common/vertex.h>
#include <sprite/spritesheet.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <set>

// Forward Declarations
struct XMLNode;
class CQuad2D;

class CObjectVisualData2D : public iObjectVisualData
{
public:

    // Constructor/Destructor
    CObjectVisualData2D();
    ~CObjectVisualData2D();

    // Load thes object data from node
    void loadFromNode( const XMLNode & objectNode, const std::string & name ) override;

    // Create the object from data
    void createFromData( const std::string & group, CSize<float> & rSize ) override;

    // Get the gne type
    NDefs::EGenerationType getGenerationType() const override;

    // Get the texture
    const CTexture & getTexture( uint index = 0 ) const override;

    // Get the name of the pipeline index
    int getPipelineIndex() const override;

    // Get the color
    const CColor & getColor() const override;

    // Get the VBO
    const CMemoryBuffer & getVBO() const override;

    // Get the IBO
    const CMemoryBuffer & getIBO() const override;

    // Get the ibo count
    int getIBOCount() const override;

    // Get the frame count
    size_t getFrameCount() const override;

    // Whether or not the visual tag was specified
    bool isActive() const override;
    
    // Get the sprite sheet
    const CSpriteSheet & getSpriteSheet() const override;
    
    // Access functions for the default uniform scale
    float getDefaultUniformScale() const override;
    
    // Create a unique descriptor texture id to add to a list
    void addToDescSet( const std::string & descriptorId, std::set<std::string> & descUniqueLst ) const override;

private:
    
    // Create the texture from loaded image data
    void createTexture( const std::string & group, CTexture & rTexture, CSize<float> & rSize );
    
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
        std::vector<NVertex::vert_uv> & rVertVec,
        std::vector<uint16_t> & rIboVec );
    
    // Create a scaled frame
    void createScaledFrame( 
        const std::string & group,
        const std::string & id,
        const CScaledFrame & scaledFrame,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & spriteSheetOffset,
        const std::vector<NVertex::vert_uv> & extraVertVec );

    void createQuad(
        const CPoint<float> & vert,
        const CSize<float> & vSize,
        const CUV & uv,
        const CSize<float> & uvSize,
        const CSize<float> & textureSize,
        const CSize<float> & frameSize,
        const CRect<float> & spriteSheetOffset,
        CQuad2D & quadBuf );
    
private:
    
    // Name of this object data
    std::string m_name;

    // Loaded texture data
    std::vector<CTexture> m_textureVec;
    static CTexture m_null_texture;

    // VBO buffer
    CMemoryBuffer m_vboBuffer;

    // IBO
    CMemoryBuffer m_iboBuffer;

    // VBO/IBO generation type
    NDefs::EGenerationType m_genType;

    // Pipeline index
    int m_pipelineIndex;

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
