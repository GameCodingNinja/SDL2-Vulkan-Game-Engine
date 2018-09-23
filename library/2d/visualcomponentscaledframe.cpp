
/************************************************************************
*    FILE NAME:       visualcomponentscaledframe.cpp
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentscaledframe.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/objectvisualdata2d.h>
#include <common/uniformbufferobject.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentScaledFrame::CVisualComponentScaledFrame( const CObjectData2D & objectData ) :
    CVisualComponentQuad( objectData )
{
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentScaledFrame::~CVisualComponentScaledFrame()
{
}

/************************************************************************
*    DESC:  Update the UBO buffer
************************************************************************/
void CVisualComponentScaledFrame::updateUBO(
    uint32_t index,
    CDevice & device,
    const CObjectVisualData2D & rVisualData,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive ubo;
    ubo.model = model;
    ubo.viewProj = viewProj;
    ubo.color = m_color;
    ubo.additive = m_additive;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
}
