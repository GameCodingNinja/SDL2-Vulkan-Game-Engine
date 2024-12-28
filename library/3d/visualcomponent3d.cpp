
/************************************************************************
*    FILE NAME:       visualcomponent3d.cpp
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

// Physical component dependency
#include <3d/visualcomponent3d.h>

// Game lib dependencies
#include <objectdata/iobjectdata.h>
#include <objectdata/iobjectvisualdata.h>
#include <system/device.h>
#include <system/descriptorset.h>
#include <system/pipeline.h>
#include <system/uniformbufferobject.h>
#include <utilities/statcounter.h>
#include <common/camera.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CVisualComponent3D::CVisualComponent3D( const iObjectData & objectData ) :
    iVisualComponent( objectData ),
    m_rObjectData( objectData ),
    m_rModel( objectData.getVisualData().getModel() ),
    //m_pushDescSetVec( objectData.getVisualData().getModel().m_meshVec.size() ),
    m_active( objectData.getVisualData().isActive() )
{
    auto & device( CDevice::Instance() );
    const uint32_t pipelineIndex( objectData.getVisualData().getPipelineIndex() );

    // Create the uniform buffer
    m_uniformBufVec = device.createUniformBufferVec( pipelineIndex );
    
    // Create the push descriptor set
    for( auto & iter : m_rModel.m_meshVec )
        m_pDescriptorSetVec.push_back( 
            device.getDescriptorSet(
                pipelineIndex,
                iter.m_textureVec.back(),
                m_uniformBufVec ));

    /*device.createPushDescriptorSet(
        pipelineIndex,
        m_rModel.m_meshVec[i].m_textureVec.back(),
        m_uniformBufVec,
        m_pushDescSetVec[i] );*/
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CVisualComponent3D::~CVisualComponent3D()
{
    CDevice::Instance().AddToDeleteQueue( m_uniformBufVec );

    for( auto iter : m_pDescriptorSetVec )
        CDevice::Instance().recycleDescriptorSet( iter );
}

/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponent3D::recordCommandBuffer(
    uint32_t index,
    VkCommandBuffer cmdBuffer,
    const CObject * const pObject,
    const CCamera & camera )
{
    if( m_active )
    {
        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().incDisplayCounter();
        
        const auto & rVisualData( m_rObjectData.getVisualData() );
        auto & device( CDevice::Instance() );

        // Get the pipeline data
        const SPipelineData & rPipelineData = device.getPipelineData( rVisualData.getPipelineIndex() );

        // Update the UBO buffer
        updateUBO( index, device, rVisualData, pObject, camera );

        // Bind the pipeline
        vkCmdBindPipeline( cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.pipeline );
        
        for( size_t i = 0; i < m_rModel.m_meshVec.size(); ++i )
        {
            // Bind vertex buffer
            VkBuffer vertexBuffers[] = {m_rModel.m_meshVec[i].m_vboBuffer.m_buffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers( cmdBuffer, 0, 1, vertexBuffers, offsets );

            // Bind the index buffer
            vkCmdBindIndexBuffer( cmdBuffer, m_rModel.m_meshVec[i].m_iboBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT16 );

            // With the regular descriptor set implementation, objects that use the same texture and UBO can't share the same
            // descriptor set because the translation matrix is part of the UBO the objects sharing this will render ontop of each other
            vkCmdBindDescriptorSets( 
                cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.pipelineLayout, 0, 1, &m_pDescriptorSetVec[i]->m_descriptorVec[index], 0, nullptr );

            // Use the push descriptors
            //m_pushDescSetVec[i].cmdPushDescriptorSet( index, cmdBuffer, rPipelineData.pipelineLayout );

            // Do the draw
            vkCmdDrawIndexed( cmdBuffer, m_rModel.m_meshVec[i].m_iboCount, 1, 0, 0, 0 );
        }
    }
}

/************************************************************************
*    DESC:  Update the UBO buffer
************************************************************************/
void CVisualComponent3D::updateUBO(
    uint32_t index,
    CDevice & device,
    const iObjectVisualData & rVisualData,
    const CObject * const pObject,
    const CCamera & camera )
{
    // Setup the uniform buffer object
    NUBO::model_rotate_viewProj_color_additive ubo;
    ubo.model = pObject->getMatrix();
    ubo.rotate = m_rotMatrix * camera.getRotMatrix();
    ubo.viewProj = camera.getFinalMatrix();
    ubo.color = m_color;
    ubo.additive = m_additive;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
}

/************************************************************************
*    DESC:  Get the rotation matrix
************************************************************************/
const CMatrix & CVisualComponent3D::getRotMatrix() const
{
    return m_rotMatrix;
}

CMatrix & CVisualComponent3D::getRotMatrix()
{
    return m_rotMatrix;
}

/************************************************************************
*    DESC:  Use a point to set a column - used for 3d physics
************************************************************************/
void CVisualComponent3D::setRotMatrixColumn( const int col, const float x, const float y, const float z )
{
    m_rotMatrix.setColumn( col, x, y, z );
}
