
/************************************************************************
*    FILE NAME:       visualcomponentscaledframe.cpp
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentscaledframe.h>

// Game lib dependencies
#include <objectdata/iobjectdata.h>
#include <objectdata/iobjectvisualdata.h>
#include <common/camera.h>
#include <system/device.h>
#include <system/uniformbufferobject.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentScaledFrame::CVisualComponentScaledFrame( const iObjectData & objectData ) :
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
    const iObjectVisualData & rVisualData,
    const CObject * const pObject,
    const CCamera & camera )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive ubo;
    ubo.model = pObject->getMatrix();
    ubo.viewProj = camera.getFinalMatrix();
    ubo.color = m_color;
    ubo.additive = m_additive;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
}

/************************************************************************
*    DESC:  Get the size
************************************************************************/
const CSize<float> & CVisualComponentScaledFrame::getSize() const
{
    return m_rObjectData.getSize();
}
