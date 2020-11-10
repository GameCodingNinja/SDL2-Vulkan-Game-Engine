
/************************************************************************
*    FILE NAME:       texture.h
*
*    DESCRIPTION:     Class for holding texture data
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/size.h>

// Standard lib dependencies
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Texture type
enum class ETextureType
{
    _NULL = -1,
    DIFFUSE,
    NORMAL,
    SPECULAR,
    DISPLACEMENT
};

class CTexture
{
public:
    
    // Mip levels
    uint32_t mipLevels = 1;

    // Texture image handle
    VkImage textureImage = VK_NULL_HANDLE;

    // Texture memory handle
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;

    // Texture Image View
    VkImageView textureImageView = VK_NULL_HANDLE;

    // Texture shader sampler
    VkSampler textureSampler = VK_NULL_HANDLE;

    // Texture size - mostly needed for 2D
    CSize<int32_t> size;
    
    // Texture type
    ETextureType type = ETextureType::DIFFUSE;
    
    // Texture file path
    std::string textFilePath;

    /************************************************************************
    *    DESC:  Free the texture memory
    ************************************************************************/
    void free( VkDevice logicalDevice )
    {
        textFilePath.clear();
        mipLevels = 1;
        size.clear();
        type = ETextureType::DIFFUSE;

        if( textureImage != VK_NULL_HANDLE )
        {
            vkDestroyImage( logicalDevice, textureImage, nullptr );
            textureImage = VK_NULL_HANDLE;
        }

        if( textureImageMemory != VK_NULL_HANDLE )
        {
            vkFreeMemory( logicalDevice, textureImageMemory, nullptr );
            textureImageMemory = VK_NULL_HANDLE;
        }

        if( textureImageView != VK_NULL_HANDLE )
        {
            vkDestroyImageView( logicalDevice, textureImageView, nullptr );
            textureImageView = VK_NULL_HANDLE;
        }

        if( textureSampler != VK_NULL_HANDLE )
        {
            vkDestroySampler( logicalDevice, textureSampler, nullptr );
            textureSampler = VK_NULL_HANDLE;
        }
    }
};
