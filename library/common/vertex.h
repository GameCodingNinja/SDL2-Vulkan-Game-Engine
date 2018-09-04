/************************************************************************
*    FILE NAME:       vertex.h
*
*    DESCRIPTION:     Vertex class
************************************************************************/  

#ifndef __vertex_h__
#define __vertex_h__

// Game lib dependencies
#include <common/point.h>
#include <common/uv.h>
#include <common/normal.h>

// Standard lib dependencies
#include <array>

// Vulkan lib dependencies
#include <system/vulkan.h>

class CVertex
{
public:

    // Verts
    CPoint<float> vert;

    // uv
    CUV uv;
    
    // Vertex normal
    CNormal<float> norm;
    
    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(CVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(CVertex, vert);
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(CVertex, uv);
        
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(CVertex, norm);

        return attributeDescriptions;
    }
};

#endif  // __vertex_2d_h__
