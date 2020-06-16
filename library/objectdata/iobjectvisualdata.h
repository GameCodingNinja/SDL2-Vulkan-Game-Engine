
/************************************************************************
*    FILE NAME:       iobjectvisualdata.h
*
*    DESCRIPTION:     Object Visual Data interface class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/defs.h>
#include <common/color.h>
#include <common/texture.h>
#include <common/model.h>
#include <sprite/spritesheet.h>
#include <system/memorybuffer.h>

// Standard lib dependencies
#include <string>
#include <set>

// Forward Declarations
struct XMLNode;

class iObjectVisualData
{
public:

    // Constructor / Destructor
    iObjectVisualData();
    virtual ~iObjectVisualData();

    // Load thes object data from node
    virtual void loadFromNode( const XMLNode & objectNode, const std::string & name )
    {}

    // Create the object from data
    virtual void createFromData( const std::string & group, CSize<float> & rSize )
    {}

    // Get the gne type
    virtual EGenType getGenerationType() const
    { return EGenType::_NULL_; }

    // Get the texture
    virtual const CTexture & getTexture( uint index = 0 ) const
    { return m_null_texture; }

    // Get the name of the pipeline index
    virtual int getPipelineIndex() const
    { return 0; }

    // Get the color
    virtual const CColor & getColor() const
    { return m_null_color; }

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
    
    // Get the mesh3d vector
    virtual const CModel & getModel() const
    { return m_null_model; }
    
private:
    
    static CTexture m_null_texture;
    static CColor m_null_color;
    static CMemoryBuffer m_null_memoryBuffer;
    static CSpriteSheet m_null_spriteSheet;
    static CModel m_null_model;
};
