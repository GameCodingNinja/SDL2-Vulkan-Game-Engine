/************************************************************************
*    FILE NAME:       memorybuffer.h
*
*    DESCRIPTION:     VK Memory Buffer class
************************************************************************/  

#ifndef __memory_buffer_h__
#define __memory_buffer_h__

// Vulkan lib dependencies
#if defined(__ANDROID__)
#include <system/vulkan_wrapper.h>
#else
#include <vulkan/vulkan.h>
#endif

class CMemoryBuffer
{
public:

    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;
};

#endif  // __memory_buffer_h__
