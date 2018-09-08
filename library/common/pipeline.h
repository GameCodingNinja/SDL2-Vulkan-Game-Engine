
/************************************************************************
*    FILE NAME:       pipelinedata.h
*
*    DESCRIPTION:     pipeline data class
************************************************************************/

#ifndef __pipeline_data_h__
#define __pipeline_data_h__

// Vulkan lib dependencies
#include <system/vulkan.h>

// Standard lib dependencies
#include <string>
#include <vector>

class CUBO
{
public:
    
    CUBO( int size ) : uboSize(size)
    {}
    int uboSize;
};

class CDescriptor
{
public:
    
    std::vector<CUBO> m_uboVec;
    std::vector<std::string> m_descriptorIdVec;
};

class CShader
{
public:
    
    VkShaderModule m_vert = VK_NULL_HANDLE;
    VkShaderModule m_frag = VK_NULL_HANDLE;
};

class CPipelineData
{
public:
    
    CShader m_shader;
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    std::string m_vertexDescription;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkVertexInputBindingDescription vertInputBindingDesc = {};
    std::vector<VkVertexInputAttributeDescription> vertInputAttrDescVec;
};

#endif  // __pipeline_data_h__
