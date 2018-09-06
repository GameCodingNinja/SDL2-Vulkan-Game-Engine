
/************************************************************************
*    FILE NAME:       pipelinedata.h
*
*    DESCRIPTION:     pipeline data class
************************************************************************/

#ifndef __pipeline_data_h__
#define __pipeline_data_h__

// Vulkan lib dependencies
#include <system/vulkan.h>

class CPipelineData
{
public:

    VkShaderModule m_shaderVert = VK_NULL_HANDLE;
    VkShaderModule m_shaderFrag = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
};

#endif  // __pipeline_data_h__
