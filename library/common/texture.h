
/************************************************************************
*    FILE NAME:       texture.h
*
*    DESCRIPTION:     Class for holding texture data
************************************************************************/

#ifndef __texture_h__
#define __texture_h__

// Game lib dependencies
#include <common/size.h>

// Standard lib dependencies
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Texture type
enum ETextureType
{
    ETT_NULL = -1,
    ETT_DIFFUSE,
    ETT_NORMAL,
    ETT_SPECULAR,
    ETT_DISPLACEMENT,
    ETT_MAX_TEXT_TYPES,
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
    ETextureType type = ETT_DIFFUSE;
    
    // Texture file path
    std::string textFilePath;
};

#endif
