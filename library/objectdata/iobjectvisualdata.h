
/************************************************************************
*    FILE NAME:       iobjectvisualdata.h
*
*    DESCRIPTION:     Object Visual Data interface class
************************************************************************/

#ifndef __iobjectvisualdata_h__
#define __iobjectvisualdata_h__

// Game lib dependencies
#include <common/defs.h>
#include <common/size.h>
#include <common/color.h>
#include <common/memorybuffer.h>
#include <common/spritesheet.h>
#include <common/texture.h>

// Standard lib dependencies
#include <string>
#include <set>

// Forward Declarations
struct XMLNode;

class iObjectVisualData
{
public:

    // Load thes object data from node
    virtual void loadFromNode( const XMLNode & objectNode, const std::string & name )
    {}

    // Create the object from data
    virtual void createFromData( const std::string & group, CSize<int> & rSize )
    {}

    // Get the gne type
    virtual NDefs::EGenerationType getGenerationType() const
    { return NDefs::EGT_NULL; }

    // Get the texture
    virtual const CTexture & getTexture( uint index = 0 ) const
    { return m_null_texture; }

    // Get the name of the pipeline index
    virtual int getPipelineIndex() const
    { return 0; }

    // Get the color
    virtual const CColor & getColor() const
    { return m_null_color; }

    // Get the vertex scale
    virtual const CSize<float> & getVertexScale() const
    { return m_null_size; }

    // Get the VBO
    virtual const CMemoryBuffer & getVBO() const
    { return m_null_memoryBuffer; }

    // Get the IBO
    virtual const CMemoryBuffer & getIBO() const
    { return m_null_memoryBuffer; }

    // Get the ibo count
    virtual int getIBOCount() const
    { return 0; }

    // Get the frame count
    virtual size_t getFrameCount() const
    { return 0; }

    // Whether or not the visual tag was specified
    virtual bool isActive() const
    { return false; }
    
    // Get the sprite sheet
    virtual const CSpriteSheet & getSpriteSheet() const
    { return m_null_spriteSheet; }
    
    // Access functions for the default uniform scale
    virtual float getDefaultUniformScale() const
    { return 0.f; }
    
    // Create a unique descriptor texture id to add to a list
    virtual void addToDescSet( const std::string & descriptorId, std::set<std::string> & descUniqueLst ) const
    {}
    
private:
    
    static CTexture m_null_texture;
    static CColor m_null_color;
    static CSize<float> m_null_size;
    static CMemoryBuffer m_null_memoryBuffer;
    static CSpriteSheet m_null_spriteSheet;
};

#endif  // __iobjectdata_h__


