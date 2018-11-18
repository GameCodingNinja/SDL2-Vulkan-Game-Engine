
/************************************************************************
*    FILE NAME:       visualcomponent3d.h
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#ifndef __visual_component_3d_h__
#define __visual_component_3d_h__

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/pushdescriptorset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CObjectData3D;
class CModel;
class CDevice;
class iObjectVisualData;
class CMemoryBuffer;

class CVisualComponent3D : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponent3D( const CObjectData3D & objectData );

    // Destructor
    ~CVisualComponent3D();

    // Record the command buffers
    virtual void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CMatrix & model,
        const CMatrix & viewProj ) override;
    
private:
    
    // Update the UBO buffer
    virtual void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CMatrix & model,
        const CMatrix & viewProj );

private:

    // Reference to object visual data
    const CObjectData3D & m_rObjectData;

    // Copy of model data
    const CModel & m_rModel;
    
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;

    // Push Descriptor set
    std::vector<CPushDescriptorSet> m_pushDescSetVec;
    
    // Is the active
    const bool m_active;
};

#endif
