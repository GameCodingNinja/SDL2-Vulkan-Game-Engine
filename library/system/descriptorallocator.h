/************************************************************************
*    FILE NAME:       descriptorallocator.h
*
*    DESCRIPTION:     Decsriptor allocator class
************************************************************************/

#pragma once

// Game lib dependencies
#include <system/descriptorset.h>

// Standard lib dependencies
#include <vector>
#include <deque>

// Vulkan lib dependencies
#include <system/vulkan.h>

class CDescriptorAllocator
{
public:

    std::vector<VkDescriptorPool> m_descriptorPoolVec;
    std::deque<std::vector<CDescriptorSet>> m_descriptorSetDeqVec;
};
