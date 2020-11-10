
/************************************************************************
*    FILE NAME:       visualcomponentspritesheet.cpp
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentspritesheet.h>

// Game lib dependencies
#include <objectdata/iobjectdata.h>
#include <objectdata/objectvisualdata2d.h>
#include <common/camera.h>
#include <system/device.h>
#include <system/uniformbufferobject.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentSpriteSheet::CVisualComponentSpriteSheet( const iObjectData & objectData ) :
    CVisualComponentQuad( objectData )
{
    m_glyphUV = objectData.getVisualData().getSpriteSheet().getGlyph().getUV();
    m_frameIndex = objectData.getVisualData().getSpriteSheet().getDefaultIndex();
}

/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentSpriteSheet::~CVisualComponentSpriteSheet()
{
}

/************************************************************************
*    DESC:  Update the UBO buffer
************************************************************************/
void CVisualComponentSpriteSheet::updateUBO(
    uint32_t index,
    CDevice & device,
    const iObjectVisualData & rVisualData,
    const CObject * const pObject,
    const CCamera & camera )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive_glyph ubo;
    ubo.model.setScale( m_quadVertScale );
    ubo.model *= pObject->getMatrix();
    ubo.viewProj = camera.getFinalMatrix();
    ubo.color = m_color;
    ubo.additive = m_additive;
    ubo.glyph = m_glyphUV;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
}

/************************************************************************
*    DESC:  Get the crop offset
************************************************************************/
const CSize<int> & CVisualComponentSpriteSheet::getCropOffset( uint index ) const
{
    return m_rObjectData.getVisualData().getSpriteSheet().getGlyph( index ).getCropOffset();
}

/************************************************************************
*    DESC:  Set the frame ID from index
************************************************************************/
void CVisualComponentSpriteSheet::setFrame( uint index )
{
    iVisualComponent::setFrame( index );
    
    const auto & rVisualData( m_rObjectData.getVisualData() );
    
    auto & rGlyph = rVisualData.getSpriteSheet().getGlyph( index );
    m_glyphUV = rGlyph.getUV();
    m_quadVertScale = rGlyph.getSize() * rVisualData.getDefaultUniformScale();
}