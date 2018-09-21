
/************************************************************************
*    FILE NAME:       pipelinedata.h
*
*    DESCRIPTION:     Set of classes for dynamic creration of pipelines
************************************************************************/

#ifndef __pipeline_data_h__
#define __pipeline_data_h__

// Vulkan lib dependencies
#include <system/vulkan.h>

// Standard lib dependencies
#include <string>
#include <vector>

class CUboData
{
public:
    
    CUboData(){}
    CUboData( const std::string & uboId, int size ) : m_uboId(uboId), uboSize(size)
    {}
    std::string m_uboId;
    int uboSize = 0;
};
    
class CDescriptorData
{
public:
    
    class CDescriptor
    {
    public:
        CUboData m_ubo;
        std::string m_descrId;
    };

    std::vector<CDescriptor> m_descriptorVec;
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
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    std::string m_descriptorId;
    VkVertexInputBindingDescription vertInputBindingDesc = {};
    std::vector<VkVertexInputAttributeDescription> vertInputAttrDescVec;
};

#endif  // __pipeline_data_h__