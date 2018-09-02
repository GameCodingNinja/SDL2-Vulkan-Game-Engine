
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
#if defined(__ANDROID__)
#include <system/vulkan_wrapper.h>
#else
#include <vulkan/vulkan.h>
#endif

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
    uint32_t m_mipLevels = 1;

    // Texture image handle
    VkImage m_textureImage = VK_NULL_HANDLE;

    // Texture memory handle
    VkDeviceMemory m_textureImageMemory = VK_NULL_HANDLE;

    // Texture Image View
    VkImageView m_textureImageView = VK_NULL_HANDLE;

    // Texture shader sampler
    VkSampler m_textureSampler = VK_NULL_HANDLE;

    // Texture size - mostly needed for 2D
    CSize<int32_t> m_size;
    
    // Texture type
    ETextureType m_type = ETT_DIFFUSE;
    
    // Texture file path
    std::string m_textFilePath;
};

#endif  // __texture_h__


