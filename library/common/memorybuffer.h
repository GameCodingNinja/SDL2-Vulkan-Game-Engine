/************************************************************************
*    FILE NAME:       memorybuffer.h
*
*    DESCRIPTION:     VK Memory Buffer class
************************************************************************/  

#ifndef __memory_buffer_h__
#define __memory_buffer_h__

// Vulkan lib dependencies
#include <system/vulkan.h>

class CMemoryBuffer
{
public:

    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;
};

#endif  // __memory_buffer_h__
