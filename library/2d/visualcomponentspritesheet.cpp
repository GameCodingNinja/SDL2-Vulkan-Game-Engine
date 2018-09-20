
/************************************************************************
*    FILE NAME:       visualcomponentspritesheet.cpp
*
*    DESCRIPTION:     Class for handling the visual part of 2d quad
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentspritesheet.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <common/uniformbufferobject.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentSpriteSheet::CVisualComponentSpriteSheet( const CObjectData2D & objectData ) :
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
    const CObjectVisualData2D & rVisualData,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive_glyph ubo;
    ubo.model.setScale( rVisualData.getVertexScale() );
    ubo.model *= model;
    ubo.viewProj = viewProj;
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
