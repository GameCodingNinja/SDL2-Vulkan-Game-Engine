
/************************************************************************
*    FILE NAME:       physicaldevice.cpp
*
*    DESCRIPTION:     Physical device class
************************************************************************/

// Physical component dependency
#include <system/physicaldevice.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPhysicalDevice::CPhysicalDevice( VkPhysicalDevice physicalDevice ) : pDev(physicalDevice)
{
    vkGetPhysicalDeviceProperties( physicalDevice, &prop );

    // Get the queue family count for this physical device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyCount, nullptr );

    if( queueFamilyCount > 0 )
    {
        queueFamilyPropVec.resize( queueFamilyCount );
        vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyCount, queueFamilyPropVec.data() );
    }
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CPhysicalDevice::~CPhysicalDevice()
{
}

