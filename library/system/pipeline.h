
/************************************************************************
*    FILE NAME:       pipelinedata.h
*
*    DESCRIPTION:     Set of classes for dynamic creration of pipelines
************************************************************************/

#pragma once

// Vulkan lib dependencies
#include <system/vulkan.h>

// Standard lib dependencies
#include <string>
#include <vector>

struct SUboData
{
    SUboData(){}
    SUboData( const std::string & uboId, int size ) :
        uboId(uboId), uboSize(size)
    {}
    
    // UBO string Id
    std::string uboId;
    
    // UBO size
    int uboSize = 0;
};
    
struct SDescriptorData
{
public:
    
    struct SDescriptor
    {
        // UBO data needed for the descriptor
        SUboData ubo;
        
        // Descriptor name id
        std::string descrId;
    };

    SDescriptorData( const size_t descPoolMax ) : descPoolMax( descPoolMax )
    {}

    std::vector<SDescriptor> m_descriptorVec;

    // Descriptor pool max
    const size_t descPoolMax;
};

struct SShader
{
    // Handle to the vertex shader
    VkShaderModule vert = VK_NULL_HANDLE;
    
    // handle to the fragment shader
    VkShaderModule frag = VK_NULL_HANDLE;
};

struct SPipelineData
{
    // Shader for this pipeline
    SShader shader;
    
    // Handle to the descriptor set layout
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    
    // Handle to the pipeline layout
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    
    // Handle to the pipeline
    VkPipeline pipeline = VK_NULL_HANDLE;
    
    // Name of the descriptor id
    std::string descriptorId;
    
    // Vertex input binding description
    VkVertexInputBindingDescription vertInputBindingDesc = {};
    
    // Vertex input attribute description
    std::vector<VkVertexInputAttributeDescription> vertInputAttrDescVec;
    
    // Do we enable the depth test
    bool depthTestEnable = false;
    bool depthWriteEnable = false;

    // Do we create the depth stencil test
    bool stencilTestEnable = false;

    // Flag if this is the stencil pipeline
    bool stencilPipeline = false;

    // Raster mode members
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlagBits cullMode = VK_CULL_MODE_BACK_BIT;
    VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

};
