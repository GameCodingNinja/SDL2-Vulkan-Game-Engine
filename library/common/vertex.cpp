
/************************************************************************
*    FILE NAME:       vertex.cpp
*
*    DESCRIPTION:     Vertex class
************************************************************************/

// Physical component dependency
#include <common/vertex.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

namespace NVertex
{
    /************************************************************************
    *    DESC:  Get the vertex input binding binding description
    ************************************************************************/ 
    VkVertexInputBindingDescription getBindingDesc( const std::string & bindingDes )
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;

        if( bindingDes == "vert_uv_norm" )
        {
            bindingDescription.stride = sizeof(vert_uv_normal);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        }
        else if( bindingDes == "vert_uv" )
        {
            bindingDescription.stride = sizeof(vert_uv);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        }
        else if( bindingDes == "vert" )
        {
            bindingDescription.stride = sizeof(vert);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        }
        else
        {
            throw NExcept::CCriticalException(
                "Vulkan Error!",
                boost::str( boost::format("Binding Description not defined! %s") % bindingDes ) );
        }

        return bindingDescription;
    }

    /************************************************************************
    *    DESC:  Get the vertex input attribute description
    ************************************************************************/ 
    std::vector<VkVertexInputAttributeDescription> getAttributeDesc( const std::string & vertAttrDes )
    {
        std::vector<VkVertexInputAttributeDescription> attrDescVec;

        if( vertAttrDes == "vert_uv_norm" )
        {
            {
                VkVertexInputAttributeDescription attrDesc = {};
                attrDesc.binding = 0;
                attrDesc.location = 0;
                attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
                attrDesc.offset = offsetof(vert_uv_normal, vert);
                attrDescVec.push_back( attrDesc );
            }

            {
                VkVertexInputAttributeDescription attrDesc = {};
                attrDesc.binding = 0;
                attrDesc.location = 1;
                attrDesc.format = VK_FORMAT_R32G32_SFLOAT;
                attrDesc.offset = offsetof(vert_uv_normal, uv);
                attrDescVec.push_back( attrDesc );
            }

            {
                VkVertexInputAttributeDescription attrDesc = {};
                attrDesc.binding = 0;
                attrDesc.location = 2;
                attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
                attrDesc.offset = offsetof(vert_uv_normal, norm);
                attrDescVec.push_back( attrDesc );
            }
        }
        else if( vertAttrDes == "vert_uv" )
        {
            {
                VkVertexInputAttributeDescription attrDesc = {};
                attrDesc.binding = 0;
                attrDesc.location = 0;
                attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
                attrDesc.offset = offsetof(vert_uv, vert);
                attrDescVec.push_back( attrDesc );
            }

            {
                VkVertexInputAttributeDescription attrDesc = {};
                attrDesc.binding = 0;
                attrDesc.location = 1;
                attrDesc.format = VK_FORMAT_R32G32_SFLOAT;
                attrDesc.offset = offsetof(vert_uv, uv);
                attrDescVec.push_back( attrDesc );
            }
        }
        else if( vertAttrDes == "vert" )
        {
            VkVertexInputAttributeDescription attrDesc = {};
            attrDesc.binding = 0;
            attrDesc.location = 0;
            attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
            attrDesc.offset = offsetof(vert, vert);
            attrDescVec.push_back( attrDesc );
        }
        else
        {
            throw NExcept::CCriticalException(
                "Vulkan Error!",
                boost::str( boost::format("Attribute Description not defined! %s") % vertAttrDes ) );
        }

        return attrDescVec;
    }
}
