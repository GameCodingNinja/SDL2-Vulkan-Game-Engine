/************************************************************************
*    FILE NAME:       memorybuffer.h
*
*    DESCRIPTION:     VK Memory Buffer class
************************************************************************/  

#pragma once

// Vulkan lib dependencies
#include <system/vulkan.h>

class CMemoryBuffer
{
public:

    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;
    
    bool isEmpty()
    {
        if( m_buffer == VK_NULL_HANDLE && m_deviceMemory == VK_NULL_HANDLE )
            return true;
        
        return false;
    }

    /************************************************************************
    *    DESC:  Free the memory buffer
    ************************************************************************/
    void free( VkDevice logicalDevice )
    {
        if( m_buffer != VK_NULL_HANDLE )
        {
            vkDestroyBuffer( logicalDevice, m_buffer, nullptr );
            m_buffer = VK_NULL_HANDLE;
        }

        if( m_deviceMemory != VK_NULL_HANDLE )
        {
            vkFreeMemory( logicalDevice, m_deviceMemory, nullptr );
            m_deviceMemory = VK_NULL_HANDLE;
        }
    }
};
