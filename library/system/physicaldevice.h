/************************************************************************
*    FILE NAME:       physicaldevice.h
*
*    DESCRIPTION:     Physical device class
************************************************************************/

#ifndef __physical_device_h__
#define __physical_device_h__

// Vulkan lib dependencies
#include <system/vulkan.h>

// Standard lib dependencies
#include <vector>

class CPhysicalDevice
{
public:

    CPhysicalDevice( VkPhysicalDevice phyDev );
    ~CPhysicalDevice();

    // Pointer to the physical device
    VkPhysicalDevice pDev = VK_NULL_HANDLE;

    // Physical device properties
    VkPhysicalDeviceProperties prop {};

    // Vector of queue family properties
    std::vector<VkQueueFamilyProperties> queueFamilyPropVec;
};

#endif