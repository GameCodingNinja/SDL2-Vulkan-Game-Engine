/************************************************************************
*    FILE NAME:       vertex.h
*
*    DESCRIPTION:     Vertex class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/point.h>
#include <common/uv.h>
#include <common/normal.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

namespace NVertex
{
    class vert
    {
    public:

        // Verts
        CPoint<float> vert;
    };
    
    class vert_uv
    {
    public:

        // Verts
        CPoint<float> vert;

        // uv
        CUV uv;
    };
    
    class vert_uv_normal
    {
    public:

        // Verts
        CPoint<float> vert;

        // uv
        CUV uv;

        // Vertex normal
        CNormal<float> norm;
    };

    // Get the vertex input binding binding description
    VkVertexInputBindingDescription getBindingDesc( const std::string & bindingDes );

    // Get the vertex input attribute description
    std::vector<VkVertexInputAttributeDescription> getAttributeDesc( const std::string & vertAttrDes );
}
