
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

    VkShaderModule m_shaderVert;
    VkShaderModule m_shaderFrag;
    VkPipeline m_pipeline;
};

#endif  // __pipeline_data_h__
