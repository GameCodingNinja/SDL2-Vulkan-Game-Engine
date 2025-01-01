
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

    std::string vertFunc;
    std::string fragFunc;
};

struct SPipelineData
{
    // Id name of this pipeline data
    std::string id;

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

    // Color blend mode members
    VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    bool blendEnable = true;
    
    // Depth stencil mode members
    bool depthTestEnable = false;
    bool depthWriteEnable = false;
    bool stencilTestEnable = false;
    VkCompareOp compareOp = VK_COMPARE_OP_EQUAL;
    VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS;
    bool depthBoundsTestEnable = false;
    VkStencilOp failOp = VK_STENCIL_OP_KEEP;
    VkStencilOp depthFailOp = VK_STENCIL_OP_KEEP;
    VkStencilOp passOp = VK_STENCIL_OP_REPLACE;
    uint32_t compareMask = 0xff;
    uint32_t writeMask = 0xff;
    uint32_t reference = 1;

    // Rasterizer mode members
    bool depthClampEnable = false;
    bool rasterizerDiscardEnable = false;
    float lineWidth = 1.0f;
    bool depthBiasEnable = false;
    float depthBiasConstantFactor = 0.f;
    float depthBiasClamp = 0.f;
    float depthBiasSlopeFactor = 0.f;
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlagBits cullMode = VK_CULL_MODE_BACK_BIT;
    VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

};
