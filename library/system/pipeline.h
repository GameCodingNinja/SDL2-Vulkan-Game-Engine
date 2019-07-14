
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
    
    // UBO string Id
    std::string m_uboId;
    
    // UBO size
    int uboSize = 0;
};
    
class CDescriptorData
{
public:
    
    class CDescriptor
    {
    public:
        
        // UBO data needed for the descriptor
        CUboData m_ubo;
        
        // Descriptor name id
        std::string m_descrId;
    };

    CDescriptorData( const size_t descPoolMax ) : m_descPoolMax( descPoolMax )
    {}

    std::vector<CDescriptor> m_descriptorVec;

    // Descriptor pool max
    const size_t m_descPoolMax;
};

class CShader
{
public:
    
    // Handle to the vertex shader
    VkShaderModule m_vert = VK_NULL_HANDLE;
    
    // handle to the fragment shader
    VkShaderModule m_frag = VK_NULL_HANDLE;
};

class CPipelineData
{
public:
    
    // Shader for this pipeline
    CShader m_shader;
    
    // Handle to the descriptor set layout
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
    
    // Handle to the pipeline layout
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    
    // Handle to the pipeline
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    
    // Name of the descriptor id
    std::string m_descriptorId;
    
    // Vertex input binding description
    VkVertexInputBindingDescription vertInputBindingDesc = {};
    
    // Vertex input attribute description
    std::vector<VkVertexInputAttributeDescription> vertInputAttrDescVec;
    
    // Do we enable the depth test
    bool m_enableDepthTest = false;

    // Do we create the depth stencil test
    bool m_enableStencilTest = false;
    
    // Flag if this is the stencil pipeline
    bool m_stencilPipeline = false;
};

#endif  // __pipeline_data_h__
