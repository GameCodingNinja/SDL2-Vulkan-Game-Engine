
/************************************************************************
*    FILE NAME:       visualcomponent3d.h
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#pragma once

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <system/descriptorset.h>
//#include <system/pushdescriptorset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class iObjectData;
class CModel;
class CDevice;
class iObjectVisualData;
class CMemoryBuffer;

class CVisualComponent3D : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponent3D( const iObjectData & objectData );

    // Destructor
    virtual ~CVisualComponent3D();

    // Record the command buffers
    void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CObject * const pObject,
        const CCamera & camera ) final;

    // Get the rotation matrix
    const CMatrix & getRotMatrix() const final;
    CMatrix & getRotMatrix() final;

    // Use a point to set a column - used for 3d physics
    void setRotMatrixColumn( const int col, const float x, const float y, const float z ) final;
    
private:
    
    // Update the UBO buffer
    void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CObject * const pObject,
        const CCamera & camera );

private:

    // Reference to object visual data
    const iObjectData & m_rObjectData;

    // Copy of model data
    const CModel & m_rModel;
    
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;

    // Descriptor Set for this image
    std::vector<CDescriptorSet *> m_pDescriptorSetVec;

    // Push Descriptor set
    //std::vector<CPushDescriptorSet> m_pushDescSetVec;
    
    // Is the active
    const bool m_active;

    // Matrix for rotations only
    // Basically used for normal calculations
    CMatrix m_rotMatrix;
};
