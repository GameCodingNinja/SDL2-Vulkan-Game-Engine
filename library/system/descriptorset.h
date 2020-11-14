/************************************************************************
*    FILE NAME:       descriptorset.h
*
*    DESCRIPTION:     Desrriptor set class
************************************************************************/

#pragma once

// Vulkan lib dependencies
#include <system/vulkan.h>

class CDescriptorSet
{
public:

    CDescriptorSet(){}
    CDescriptorSet( std::vector<VkDescriptorSet> & descriptorVec ) :
        m_descriptorVec(descriptorVec)
    {}

    std::vector<VkDescriptorSet> m_descriptorVec;

    // Frame this descriptor set was recycled on plus
    // the frame buffer count as an offset to determine
    // if it was in the queue lond enough to be reused
    uint32_t m_frameRecycleOffset = 0;

    // Flag to indicate this descriptor set is actively being used so as to not hand it out
    // NOTE: Defaulted to true because it will be active when first allocated
    bool m_active = true;
};
