
/************************************************************************
*    FILE NAME:       devicevulkan.h
*
*    DESCRIPTION:     Class used for Vulkan API implementation
*
*    Based on:        https://vulkan-tutorial.com/Introduction
************************************************************************/

// Physical component dependency
#include <system/devicevulkan.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <common/texture.h>
#include <system/pipeline.h>
#include <soil/SOIL.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <bitset>

/************************************************************************
*    DESC:  Validation layer callback
************************************************************************/
VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayerCallback(
    VkFlags msgFlags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t srcObject,
    size_t location,
    int32_t msgCode,
    const char *pLayerPrefix,
    const char *pMsg,
    void *pUserData)
{
    NGenFunc::PostDebugMsg( pMsg );

    return VK_FALSE;
}

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CDeviceVulkan::CDeviceVulkan() :
    m_vulkanInstance(VK_NULL_HANDLE),
    m_vulkanSurface(VK_NULL_HANDLE),
    m_phyDevIndex(UINT32_MAX),
    m_logicalDevice(VK_NULL_HANDLE),
    m_graphicsQueueFamilyIndex(UINT32_MAX),
    m_presentQueueFamilyIndex(UINT32_MAX),
    m_transferQueueFamilyIndex(UINT32_MAX),
    m_graphicsQueue(VK_NULL_HANDLE),
    m_presentQueue(VK_NULL_HANDLE),
    m_transferQueue(VK_NULL_HANDLE),
    m_swapchain(VK_NULL_HANDLE),
    m_renderPass(VK_NULL_HANDLE),
    m_primaryCmdPool(VK_NULL_HANDLE),
    m_transferCmdPool(VK_NULL_HANDLE),
    m_depthImage(VK_NULL_HANDLE),
    m_depthImageMemory(VK_NULL_HANDLE),
    m_depthImageView(VK_NULL_HANDLE),
    vkDestroySwapchainKHR(VK_NULL_HANDLE),
    vkGetSwapchainImagesKHR(VK_NULL_HANDLE),
    vkDebugReportCallbackEXT(VK_NULL_HANDLE),
    vkDestroyDebugReportCallbackEXT(nullptr)
{
    m_vulkanErrorMap = {
        {VK_SUCCESS,                        "Vulkan Success!"},
        {VK_NOT_READY,                      "Vulkan Not Ready!"},
        {VK_TIMEOUT,                        "Vulkan Timeout!"},
        {VK_EVENT_SET,                      "Vulkan Event Set!"},
        {VK_EVENT_RESET,                    "Vulkan Event Reset!"},
        {VK_INCOMPLETE,                     "Vulkan Incomplete!"},
        {VK_ERROR_OUT_OF_HOST_MEMORY,       "Vulkan Out Of Host memory!"},
        {VK_ERROR_OUT_OF_DEVICE_MEMORY,     "Vulkan Out Of Device Memory!"},
        {VK_ERROR_INITIALIZATION_FAILED,    "Vulkan Initialization Failed!"},
        {VK_ERROR_DEVICE_LOST,              "Vulkan Device Lost!"},
        {VK_ERROR_MEMORY_MAP_FAILED,        "Vulkan Memory Map Failed!"},
        {VK_ERROR_LAYER_NOT_PRESENT,        "Vulkan Layer Not Present!"},
        {VK_ERROR_EXTENSION_NOT_PRESENT,    "Vulkan nExtension Not Present!"},
        {VK_ERROR_FEATURE_NOT_PRESENT,      "Vulkan Feature Not Present!"},
        {VK_ERROR_INCOMPATIBLE_DRIVER,      "Vulkan Incompatible Driver!"},
        {VK_ERROR_TOO_MANY_OBJECTS,         "Vulkan Too Many Objects!"},
        {VK_ERROR_FORMAT_NOT_SUPPORTED,     "Vulkan Format Not Supported!"},
        {VK_ERROR_FRAGMENTED_POOL,          "Vulkan Fragmented Pool!"},
        {VK_ERROR_OUT_OF_POOL_MEMORY,       "Vulkan Out Of Pool Memory!"},
        {VK_ERROR_INVALID_EXTERNAL_HANDLE,  "Vulkan Invalid External Handle!"},
        {VK_ERROR_SURFACE_LOST_KHR,         "Vulkan Surface Lost KHR!"},
        {VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "Vulkan Native Window In Use KHR!"},
        {VK_SUBOPTIMAL_KHR,                 "Vulkan Suboptimal KHR!"},
        {VK_ERROR_OUT_OF_DATE_KHR,          "Vulkan Out Of Date KHR!"},
        {VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "Vulkan Incompatible Display KHR!"},
        {VK_ERROR_VALIDATION_FAILED_EXT,    "Vulkan Validation Failed Ext!"},
        {VK_ERROR_INVALID_SHADER_NV,        "Vulkan Invalid Shader NV!"},
        {VK_ERROR_NOT_PERMITTED_EXT,        "Vulkan Not Permitted Ext!"} };
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CDeviceVulkan::~CDeviceVulkan()
{
}

/***************************************************************************
*   DESC:  Create Vulkan instance
****************************************************************************/
void CDeviceVulkan::create(
    const std::vector<const char*> & validationNameVec,
    const std::vector<const char*> & instanceExtensionNameVec,
    const std::vector<const char*> & physicalDeviceExtensionNameVec )
{
    #if defined(__ANDROID__)
    if( InitVulkan() == 0 )
        throw NExcept::CCriticalException( "Vulkan Error!", "Could not initialize Vulkan library!" );
    #endif

    // Create the vulkan instance
    createVulkanInstance( validationNameVec, instanceExtensionNameVec );

    // Create the Vulkan surface
    createSurface();

    // Select a physical device (GPU)
    selectPhysicalDevice();

    // Create the logical device
    createLogicalDevice( validationNameVec, physicalDeviceExtensionNameVec );

    // Setup the swap chain to be created
    setupSwapChain();

    // Create the swap chain
    createSwapChain();

    // Create the render pass
    createRenderPass();

    // Create the primary command pool
    createPrimaryCommandPool();

    // Create depth resources
    createDepthResources();

    // Create the frame buffer
    createFrameBuffer();

    // Create the Semaphores and fences
    createSyncObjects();

    // Create the primary command buffers
    createPrimaryCommandBuffers();
}

/***************************************************************************
*   DESC:  Destroy the window and Vulkan instance
****************************************************************************/
void CDeviceVulkan::destroy()
{
    if( m_logicalDevice != VK_NULL_HANDLE )
    {
        if( !m_frameFenceVec.empty() )
        {
            for( auto iter : m_frameFenceVec )
                vkDestroyFence( m_logicalDevice, iter, nullptr );

            m_frameFenceVec.clear();
        }

        if( !m_imageAvailableSemaphoreVec.empty() )
        {
            for( auto iter : m_imageAvailableSemaphoreVec )
                vkDestroySemaphore( m_logicalDevice, iter, nullptr );

            m_imageAvailableSemaphoreVec.clear();
        }

        if( !m_renderFinishedSemaphoreVec.empty()  )
        {
            for( auto iter : m_renderFinishedSemaphoreVec )
                vkDestroySemaphore( m_logicalDevice, iter, nullptr );

            m_renderFinishedSemaphoreVec.clear();
        }

        destroySwapChain();

        if( m_primaryCmdPool != VK_NULL_HANDLE )
        {
            vkDestroyCommandPool( m_logicalDevice, m_primaryCmdPool, nullptr );
            m_primaryCmdPool = VK_NULL_HANDLE;
        }

        if( m_transferCmdPool != VK_NULL_HANDLE )
        {
            vkDestroyCommandPool( m_logicalDevice, m_transferCmdPool, nullptr );
            m_transferCmdPool = VK_NULL_HANDLE;
        }

        destroyAssets();

        vkDestroyDevice( m_logicalDevice, nullptr );
        m_logicalDevice = VK_NULL_HANDLE;
    }

    if( m_vulkanInstance != VK_NULL_HANDLE )
    {
        if( m_vulkanSurface != VK_NULL_HANDLE )
            vkDestroySurfaceKHR( m_vulkanInstance, m_vulkanSurface, nullptr );

        if( CSettings::Instance().isValidationLayers() && (vkDestroyDebugReportCallbackEXT != nullptr) )
            vkDestroyDebugReportCallbackEXT( m_vulkanInstance, vkDebugReportCallbackEXT, nullptr );

        vkDestroyInstance( m_vulkanInstance, nullptr );
        m_vulkanInstance = VK_NULL_HANDLE;
    }
}

/***************************************************************************
*   DESC:  Destroy the swap chain
****************************************************************************/
void CDeviceVulkan::destroySwapChain()
{
    if( m_logicalDevice != VK_NULL_HANDLE )
    {
        if( !m_framebufferVec.empty() )
        {
            for( auto framebuffer : m_framebufferVec )
                vkDestroyFramebuffer( m_logicalDevice, framebuffer, nullptr );

            m_framebufferVec.clear();
        }

        if( m_renderPass != VK_NULL_HANDLE )
        {
            vkDestroyRenderPass( m_logicalDevice, m_renderPass, nullptr );
            m_renderPass = VK_NULL_HANDLE;
        }

        if( m_depthImageView != VK_NULL_HANDLE )
        {
            vkDestroyImageView( m_logicalDevice, m_depthImageView, nullptr );
            m_depthImageView = VK_NULL_HANDLE;
        }

        if( m_depthImage != VK_NULL_HANDLE )
        {
            vkDestroyImage( m_logicalDevice, m_depthImage, nullptr );
            m_depthImage = VK_NULL_HANDLE;
        }

        if( m_depthImageMemory != VK_NULL_HANDLE )
        {
            vkFreeMemory( m_logicalDevice, m_depthImageMemory, nullptr );
            m_depthImageMemory = VK_NULL_HANDLE;
        }

        if( m_swapchain != VK_NULL_HANDLE )
        {
            for( auto imageView : m_swapChainImageViewVec )
                vkDestroyImageView( m_logicalDevice, imageView, nullptr );

            m_swapChainImageViewVec.clear();

            vkDestroySwapchainKHR( m_logicalDevice, m_swapchain, nullptr );
            m_swapchain = VK_NULL_HANDLE;
        }
    }
}

/***************************************************************************
*   DESC:  Create the vulkan instance
****************************************************************************/
void CDeviceVulkan::createVulkanInstance(
    const std::vector<const char*> & validationNameVec,
    const std::vector<const char*> & instanceExtensionNameVec )
{
    VkResult vkResult(VK_SUCCESS);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = CSettings::Instance().getGameName().c_str();
    appInfo.applicationVersion = CSettings::Instance().getGameVersion();
    appInfo.pEngineName = CSettings::Instance().getEngineName().c_str();
    appInfo.engineVersion = CSettings::Instance().getEngineVersion();
    appInfo.apiVersion = VK_MAKE_VERSION(CSettings::Instance().getMajorVersion(), CSettings::Instance().getMinorVersion(), 0);

    printDebug( appInfo );

    VkInstanceCreateInfo instCreateInfo = {};
    instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pApplicationInfo = &appInfo;
    instCreateInfo.enabledLayerCount = validationNameVec.size();
    instCreateInfo.ppEnabledLayerNames = validationNameVec.data();
    instCreateInfo.enabledExtensionCount = instanceExtensionNameVec.size();
    instCreateInfo.ppEnabledExtensionNames = instanceExtensionNameVec.data();

    // Try to create the instance
    if( (vkResult = vkCreateInstance( &instCreateInfo, nullptr, &m_vulkanInstance )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create instance! %s") % getError(vkResult) ) );

    // Get a function pointer to the vulkan vkDestroySwapchainKHR
    if( !(vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkDestroySwapchainKHR" )) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkDestroySwapchainKHR!" );

    // Get a function pointer to the vulkan vkGetSwapchainImagesKHR
    if( !(vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkGetSwapchainImagesKHR" )) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkGetSwapchainImagesKHR!" );

    ///////////////////////////////////////////////////
    // Setup validation layers() call back
    //////////////////////////////////////////////////

    if( CSettings::Instance().isValidationLayers() )
    {
        PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = nullptr;

        // Get the function instances of the debug report callback messages
        if( !(CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr( m_vulkanInstance, "vkCreateDebugReportCallbackEXT" )) )
            throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkCreateDebugReportCallbackEXT!" );

        if( !(vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr( m_vulkanInstance, "vkDestroyDebugReportCallbackEXT" )) )
            throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkDestroyDebugReportCallbackEXT!" );

        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        dbgCreateInfo.pfnCallback = ValidationLayerCallback;
        dbgCreateInfo.pUserData = nullptr;

        if( (vkResult = CreateDebugReportCallback( m_vulkanInstance, &dbgCreateInfo, nullptr, &vkDebugReportCallbackEXT )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create debug report callback! %s") % getError(vkResult) ) );
    }
}

/***************************************************************************
*   DESC:  Select a physical device (GPU)
*          NOTE: This involves picking the GPU with the graphics bit
****************************************************************************/
void CDeviceVulkan::selectPhysicalDevice()
{
    VkResult vkResult(VK_SUCCESS);

    // Select a graphics device
    uint32_t gpuCount(0);
    if( (vkResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, nullptr )) || (gpuCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate physical device count! %s") % getError(vkResult) ) );

    std::vector<VkPhysicalDevice> physicalDeviceHandleVec(gpuCount);

    if( (vkResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, physicalDeviceHandleVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate physical device info! %s") % getError(vkResult) ) );

    // Get all the physical device info
    for( uint32_t i = 0; i < physicalDeviceHandleVec.size(); i++ )
    {
        m_phyDevVec.emplace_back( physicalDeviceHandleVec[i] );
        printDebug( m_phyDevVec.back(), i );
    }

    // Select the discrete GPU if one is available that supports the graphics bit
    m_graphicsQueueFamilyIndex = UINT32_MAX;
    m_phyDevIndex = UINT32_MAX;
    VkPhysicalDeviceType lastDeviceType = VK_PHYSICAL_DEVICE_TYPE_OTHER;
    for( uint32_t i = 0; i < m_phyDevVec.size(); i++ )
    {
        // Find the queue family on this graphics device
        m_graphicsQueueFamilyIndex = getQueueFamilyIndex( m_phyDevVec[i], VK_QUEUE_GRAPHICS_BIT );

        if( m_phyDevVec[i].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        {
            // Found a discrete GPU, our work here is done. Break out on this one.
            if( m_graphicsQueueFamilyIndex != UINT32_MAX )
            {
                m_phyDevIndex = i;
                break;
            }
        }
        else if( m_phyDevVec[i].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
        {
            // Found an integrated GPU. Good but keep looking
            if( m_graphicsQueueFamilyIndex != UINT32_MAX )
            {
                m_phyDevIndex = i;
                lastDeviceType = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
            }
        }
        else if( m_phyDevVec[i].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU )
        {
            // Found a Virtual GPU. Keep looking for something better
            if( m_graphicsQueueFamilyIndex != UINT32_MAX &&
                lastDeviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
            {
                m_phyDevIndex = i;
                lastDeviceType = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
            }
        }
        else if( m_phyDevVec[i].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU &&
                 lastDeviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
                 lastDeviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU  )
        {
            // Found a CPU. Keep looking for something better
            if( m_graphicsQueueFamilyIndex != UINT32_MAX )
            {
                m_phyDevIndex = i;
                lastDeviceType = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU;
            }
        }
        else if( m_phyDevVec[i].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER &&
                 lastDeviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
                 lastDeviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU &&
                 lastDeviceType != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU )
        {
            // Found Other. Keep looking for something better
            if( m_graphicsQueueFamilyIndex != UINT32_MAX )
            {
                m_phyDevIndex = i;
            }
        }
    }

    if( (m_phyDevIndex == UINT32_MAX) || (m_graphicsQueueFamilyIndex == UINT32_MAX) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Suitable GPU could not be found!" );

    // Make sure we have a swap chain
    if( !isDeviceExtension( m_phyDevVec[m_phyDevIndex].pDev, VK_KHR_SWAPCHAIN_EXTENSION_NAME ) )
        throw NExcept::CCriticalException( "Vulkan Error!", "No swap chain support!" );

    // Find the remaining queue families for present and transfer
    m_presentQueueFamilyIndex = getPresentQueueFamilyIndex();
    // If a generic transfer family queue index can't be found, use a graphics family queue index
    if( (m_transferQueueFamilyIndex = getQueueFamilyIndex( m_phyDevVec[m_phyDevIndex], VK_QUEUE_TRANSFER_BIT | VK_QUEUE_GRAPHICS_BIT )) == UINT32_MAX )
        m_transferQueueFamilyIndex = getQueueFamilyIndex( m_phyDevVec[m_phyDevIndex], VK_QUEUE_GRAPHICS_BIT );
    
    printDebugPhyDev();
}

/***************************************************************************
*   DESC:  Create the logical device
*          NOTE: Creating the logical device involves determining the queues
*                it will use based on queue families.
****************************************************************************/
void CDeviceVulkan::createLogicalDevice(
    const std::vector<const char*> & validationNameVec,
    const std::vector<const char*> & physicalDeviceExtensionNameVec )
{
    VkResult vkResult(VK_SUCCESS);

    // Need a structure to hold members for organizing
    struct queueFamilyIndexStruct
    {
        queueFamilyIndexStruct(uint32_t & queueFamilyIndex, uint32_t queueIndex, VkQueue * pQueue, const std::string &typeStr) :
            m_queueFamilyIndex(queueFamilyIndex),
            m_queueIndex(queueIndex),
            m_pQueue(pQueue),
            m_typeStr(typeStr)
        {}
        uint32_t & m_queueFamilyIndex;
        uint32_t m_queueIndex;
        VkQueue * m_pQueue;
        std::string m_typeStr;
    };

    // NOTE: A queue family is one or more queues that based on their queueFlags that defines what
    //       type of queue they are ie. graphics, present & transfer. A graphics queue can also
    //       be used and reused as a present or a transfer queue.

    // Pack the vector with all the queues we'll need
    std::vector<queueFamilyIndexStruct> queueFamilyIndexVec;
    queueFamilyIndexVec.emplace_back(m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue, "graphics");
    queueFamilyIndexVec.emplace_back(m_transferQueueFamilyIndex, 0, &m_transferQueue, "transfer");

    // Add in the present queue if it doen't match the graphics queue family index
    // Reusing the graphics queue appears to run faster then having it's own queue for present
    if( m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex )
        queueFamilyIndexVec.emplace_back(m_presentQueueFamilyIndex, 0, &m_presentQueue, "present");

    // This vector is for organizing unique families into separate vector indexes
    // to inform the logical device as to what families and queue counts are needed
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo deviceQueueInfo = {};
    deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    deviceQueueInfo.queueCount = 1;
    deviceQueueInfo.pQueuePriorities = &queuePriority;

    // This vector is for organizing unique queue families into separate vector indexes 
    // for incrememting their queue counts
    std::vector<VkDeviceQueueCreateInfo> devQueueCreateInfoVec = {deviceQueueInfo};

    // This fills a vector (devQueueCreateInfoVec) for counting how many queues are needed for 
    // each family when the logical device is created.
    for( size_t i = 1; i < queueFamilyIndexVec.size(); ++i )
    {
        bool found = false;

        for( auto & iter : devQueueCreateInfoVec )
        {
            // If this is the same family, get the family index and inc the queue count
            if( iter.queueFamilyIndex == queueFamilyIndexVec[i].m_queueFamilyIndex )
            {
                NGenFunc::PostDebugMsg( boost::str( boost::format("Queue family index increment count: %s") % queueFamilyIndexVec[i].m_typeStr ));
                queueFamilyIndexVec[i].m_queueIndex = iter.queueCount;
                iter.queueCount++;
                found = true;
                break;
            }
        }

        // If the family queue index has not been found, add a new entry for the new index
        // This also for queue families that were not found that can be setup to use the graphics queue
        if( !found )
        {
            if( queueFamilyIndexVec[i].m_queueFamilyIndex != UINT32_MAX )
            {
                NGenFunc::PostDebugMsg( boost::str( boost::format("Queue family index added to Create vector: %s") % queueFamilyIndexVec[i].m_typeStr ));
                devQueueCreateInfoVec.emplace_back( deviceQueueInfo );
                devQueueCreateInfoVec.back().queueFamilyIndex = queueFamilyIndexVec[i].m_queueFamilyIndex;
            }
            // If the queue family index was not found, reuse the graphics queue family index
            else
            {
                NGenFunc::PostDebugMsg( boost::str( boost::format("Queue family index using graphics family index: %s") % queueFamilyIndexVec[i].m_typeStr ));
                queueFamilyIndexVec[i].m_queueFamilyIndex = m_graphicsQueueFamilyIndex;
            }
        }
    }

    // Get all the features supported on this device
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures( m_phyDevVec[m_phyDevIndex].pDev, &physicalDeviceFeatures );

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = devQueueCreateInfoVec.size();
    createInfo.pQueueCreateInfos = devQueueCreateInfoVec.data();
    createInfo.enabledLayerCount = validationNameVec.size();
    createInfo.ppEnabledLayerNames = validationNameVec.data();
    createInfo.enabledExtensionCount = physicalDeviceExtensionNameVec.size();
    createInfo.ppEnabledExtensionNames = physicalDeviceExtensionNameVec.data();
    createInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if( CSettings::Instance().isValidationLayers() )
    {
        createInfo.enabledLayerCount = validationNameVec.size();
        createInfo.ppEnabledLayerNames = validationNameVec.data();
    }

    // Create the logical device
    if( (vkResult = vkCreateDevice( m_phyDevVec[m_phyDevIndex].pDev, &createInfo, nullptr, &m_logicalDevice )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create logical device! %s") % getError(vkResult) ) );

    //if( !(vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)vkGetDeviceProcAddr( m_logicalDevice, "vkCmdPushDescriptorSetKHR" )) )
    //    throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkCmdPushDescriptorSetKHR!" );

    // Get a handle to the queue family for graphics, present & transfer - Could be different but most likely in the same queue family
    for( auto & iter : queueFamilyIndexVec )
    {
        vkGetDeviceQueue( m_logicalDevice, iter.m_queueFamilyIndex, iter.m_queueIndex, iter.m_pQueue );
        if( iter.m_pQueue == nullptr )
            throw NExcept::CCriticalException(
                "Vulkan Error!", boost::str( boost::format("Could not get handle to the queue family for %s!") % iter.m_typeStr) );
    }

    // If the present queue was not set, use the graphics queue
    if( m_presentQueue == VK_NULL_HANDLE )
        m_presentQueue = m_graphicsQueue;

    printDebug( devQueueCreateInfoVec );
}

/***************************************************************************
*   DESC:  Setup the swap chain to be created
****************************************************************************/
void CDeviceVulkan::setupSwapChain()
{
    VkResult vkResult(VK_SUCCESS);
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormats = nullptr;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModes = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilities = nullptr;

    if( !(GetPhysicalDeviceSurfaceFormats = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkGetPhysicalDeviceSurfaceFormatsKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkGetPhysicalDeviceSurfaceFormatsKHR!" );

    if( !(GetPhysicalDeviceSurfacePresentModes = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkGetPhysicalDeviceSurfacePresentModesKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkGetPhysicalDeviceSurfacePresentModesKHR!" );

    if( !(GetPhysicalDeviceSurfaceCapabilities = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR!" );

    // Get the device surface capabilities
    VkSurfaceCapabilitiesKHR surfCapabilities = {};
    if( (vkResult = GetPhysicalDeviceSurfaceCapabilities( m_phyDevVec[m_phyDevIndex].pDev, m_vulkanSurface, &surfCapabilities )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface capabilities! %s") % getError(vkResult) ) );
    
    printDebug( surfCapabilities );

    // Get the best surface format
    VkSurfaceFormatKHR surfaceFormat;
    uint32_t surfaceFormatCount;
    if( (vkResult = GetPhysicalDeviceSurfaceFormats( m_phyDevVec[m_phyDevIndex].pDev, m_vulkanSurface, &surfaceFormatCount, nullptr)) || (surfaceFormatCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface format count! %s") % getError(vkResult) ) );

    std::vector<VkSurfaceFormatKHR> surfaceFormatVec(surfaceFormatCount);

    if( (vkResult = GetPhysicalDeviceSurfaceFormats( m_phyDevVec[m_phyDevIndex].pDev, m_vulkanSurface, &surfaceFormatCount, surfaceFormatVec.data())) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface formats! %s") % getError(vkResult) ) );

    // Init to the first format in the event the below two fail
    surfaceFormat = surfaceFormatVec.front();

    if( (surfaceFormatCount == 1) && surfaceFormatVec.back().format == VK_FORMAT_UNDEFINED)
    {
        surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
        surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }
    else
    {
        for( const auto & sFormat : surfaceFormatVec )
        {
            if( (sFormat.format == VK_FORMAT_B8G8R8A8_UNORM) && (sFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) )
            {
                surfaceFormat = sFormat;
                break;
            }
        }
    }

    printDebug( surfaceFormatVec, surfaceFormat );

    // Get the best presentation mode
    VkPresentModeKHR surfacePresMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
    uint32_t surfacePresModeCount;
    if( (vkResult = GetPhysicalDeviceSurfacePresentModes( m_phyDevVec[m_phyDevIndex].pDev, m_vulkanSurface, &surfacePresModeCount, nullptr)) || (surfacePresModeCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface presentation mode count! %s") % getError(vkResult) ) );

    std::vector<VkPresentModeKHR> surfacePresModeVec(surfacePresModeCount);

    if( (vkResult = GetPhysicalDeviceSurfacePresentModes( m_phyDevVec[m_phyDevIndex].pDev, m_vulkanSurface, &surfacePresModeCount, surfacePresModeVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface presentation modes! %s") % getError(vkResult) ) );

    NGenFunc::PostDebugMsg("Physical Device Surface Present Modes:");
    for( const auto & presentMode : surfacePresModeVec )
    {
        if( presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR )
            NGenFunc::PostDebugMsg("  VK_PRESENT_MODE_IMMEDIATE_KHR");
        else if( presentMode == VK_PRESENT_MODE_MAILBOX_KHR )
            NGenFunc::PostDebugMsg("  VK_PRESENT_MODE_MAILBOX_KHR");
        else if( presentMode == VK_PRESENT_MODE_FIFO_KHR )
            NGenFunc::PostDebugMsg("  VK_PRESENT_MODE_FIFO_KHR");
        else if( presentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR )
            NGenFunc::PostDebugMsg("  VK_PRESENT_MODE_FIFO_RELAXED_KHR");
        else if( presentMode == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR )
            NGenFunc::PostDebugMsg("  VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR");
        else if( presentMode == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR )
            NGenFunc::PostDebugMsg("  VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR");
    }

    // Choose based on vsync value
    if ( CSettings::Instance().getVSync() )
    {
        if( isSurfacePresMode( surfacePresModeVec, VK_PRESENT_MODE_FIFO_KHR) )
            surfacePresMode = VK_PRESENT_MODE_FIFO_KHR;

        else if( isSurfacePresMode( surfacePresModeVec, VK_PRESENT_MODE_MAILBOX_KHR) )
            surfacePresMode = VK_PRESENT_MODE_MAILBOX_KHR;
    }
    else
    {
        if( isSurfacePresMode( surfacePresModeVec, VK_PRESENT_MODE_IMMEDIATE_KHR) )
            surfacePresMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

        else if( isSurfacePresMode( surfacePresModeVec, VK_PRESENT_MODE_FIFO_RELAXED_KHR) )
            surfacePresMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    }

    // If one wasn't picked, choose the first one in the list
    if( surfacePresMode == VK_PRESENT_MODE_MAX_ENUM_KHR && surfacePresModeVec.size() )
    {
        surfacePresMode = surfacePresModeVec.at(0);
        NGenFunc::PostDebugMsg("  Surface not seletcted. First one chosen.");
    }
    else // All else fails, pick one
    {
        surfacePresMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

    if( surfacePresMode == VK_PRESENT_MODE_IMMEDIATE_KHR )
        NGenFunc::PostDebugMsg("  SELECTED: VK_PRESENT_MODE_IMMEDIATE_KHR\n");
    else if( surfacePresMode == VK_PRESENT_MODE_MAILBOX_KHR )
        NGenFunc::PostDebugMsg("  SELECTED: VK_PRESENT_MODE_MAILBOX_KHR\n");
    else if( surfacePresMode == VK_PRESENT_MODE_FIFO_KHR )
        NGenFunc::PostDebugMsg("  SELECTED: VK_PRESENT_MODE_FIFO_KHR\n");
    else if( surfacePresMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR )
        NGenFunc::PostDebugMsg("  SELECTED: VK_PRESENT_MODE_FIFO_RELAXED_KHR\n");
    else if( surfacePresMode == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR )
        NGenFunc::PostDebugMsg("  SELECTED: VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR\n");
    else if( surfacePresMode == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR )
        NGenFunc::PostDebugMsg("  SELECTED: VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR\n");

    // Init the pre-transform
    VkSurfaceTransformFlagBitsKHR preTransform = surfCapabilities.currentTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

    m_swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapchainInfo.surface = m_vulkanSurface;
    m_swapchainInfo.imageFormat = surfaceFormat.format;
    m_swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
    m_swapchainInfo.imageArrayLayers = 1;
    m_swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_swapchainInfo.preTransform = preTransform;
    m_swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_swapchainInfo.presentMode = surfacePresMode;
    m_swapchainInfo.clipped = true;

    // In the event the graphics and present queue family doesn't match
    if( m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex )
    {
        m_queueFamilySwapChainIndiceVec = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};

        m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        m_swapchainInfo.queueFamilyIndexCount = m_queueFamilySwapChainIndiceVec.size();
        m_swapchainInfo.pQueueFamilyIndices = m_queueFamilySwapChainIndiceVec.data();
    }

    // Determine the number of VkImage's to use in the swap chain.
    uint32_t minImageCount = surfCapabilities.minImageCount;

    if( CSettings::Instance().getTripleBuffering() && minImageCount < 3 )
        ++minImageCount;

    // Application might settle for fewer images than desired
    if ((surfCapabilities.maxImageCount > 0) && (minImageCount > surfCapabilities.maxImageCount))
        minImageCount = surfCapabilities.maxImageCount;

    m_swapchainInfo.minImageCount = minImageCount;

    // Set the extent of the render resolution
    VkExtent2D swapchainExtent;

    // Get the render size of the window
    const CSize<uint32_t> size( CSettings::Instance().getSize() );

    // width and height are either both -1, or both not -1.
    if (surfCapabilities.currentExtent.width == (uint32_t)-1)
    {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = size.getW();
        swapchainExtent.height = size.getH();
    }
    else
    {
        // If the sizes don't match, recalculate the ratio
        if( surfCapabilities.currentExtent.width != size.getW() ||
            surfCapabilities.currentExtent.height != size.getH() )
        {
            handleResolutionChange( surfCapabilities.currentExtent.width, surfCapabilities.currentExtent.height );
        }

        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
    }

    m_swapchainInfo.imageExtent = swapchainExtent;

    printDebug( m_swapchainInfo );
}

/***************************************************************************
*   DESC:  Create the swap chain
****************************************************************************/
void CDeviceVulkan::createSwapChain()
{
    VkResult vkResult(VK_SUCCESS);
    PFN_vkCreateSwapchainKHR CreateSwapchain = nullptr;
    if( !(CreateSwapchain = (PFN_vkCreateSwapchainKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkCreateSwapchainKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkCreateSwapchainKHR!" );

    // Create the swap chain
    if( (vkResult = CreateSwapchain( m_logicalDevice, &m_swapchainInfo, nullptr, &m_swapchain)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create swap chain! %s") % getError(vkResult) ) );

    // get the swap chain images. NOTE: Swap chain images are destroyed when the swap chain is destroyed.
    uint32_t swapChainImageCount;
    if( (vkResult = vkGetSwapchainImagesKHR( m_logicalDevice, m_swapchain, &swapChainImageCount, nullptr)) || (swapChainImageCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate swap chain image count! %s") % getError(vkResult) ) );

    std::vector<VkImage> swapChainImage(swapChainImageCount);

    if( (vkResult = vkGetSwapchainImagesKHR( m_logicalDevice, m_swapchain, &swapChainImageCount, swapChainImage.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not get swap chain images! %s") % getError(vkResult) ) );

    // Print out info if the swap images don't match
    if( m_swapchainInfo.minImageCount != swapChainImageCount )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Swap chain image count doesn't match! (%u / %u)") % m_swapchainInfo.minImageCount % swapChainImageCount ));
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Swap chain image count: %u") % swapChainImageCount ));

    m_swapChainImageViewVec.reserve( swapChainImageCount );

    for( uint32_t i = 0; i < swapChainImageCount; ++i )
        m_swapChainImageViewVec.push_back( createImageView( swapChainImage[i], m_swapchainInfo.imageFormat, 1, VK_IMAGE_ASPECT_COLOR_BIT ) );
}

/***************************************************************************
*   DESC:  Create the render pass
****************************************************************************/
void CDeviceVulkan::createRenderPass()
{
    VkResult vkResult(VK_SUCCESS);

    // Create the render pass
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_swapchainInfo.imageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::vector<VkAttachmentDescription> attachments = { colorAttachment };

    // Add the depth attachment if depth or stencil buffer is needed
    if( CSettings::Instance().activateDepthBuffer() )
    {
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        attachments.push_back( depthAttachment );
    }

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if( (vkResult = vkCreateRenderPass( m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create render pass! %s") % getError(vkResult) ) );
}

/***************************************************************************
*   DESC:  Create the primary command pool
****************************************************************************/
void CDeviceVulkan::createPrimaryCommandPool()
{
    m_primaryCmdPool = createCommandPool( m_graphicsQueueFamilyIndex );
    m_transferCmdPool = createCommandPool( m_transferQueueFamilyIndex );
}

/***************************************************************************
*   DESC:  Create depth resources
****************************************************************************/
void CDeviceVulkan::createDepthResources()
{
    if( CSettings::Instance().activateDepthBuffer() )
    {
        VkFormat depthFormat = findDepthFormat();

        if( depthFormat == VK_FORMAT_UNDEFINED )
            throw NExcept::CCriticalException( "Vulkan Error!", "Could not find depth format!");

        createImage(
            m_swapchainInfo.imageExtent.width,
            m_swapchainInfo.imageExtent.height,
            1,
            depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_depthImage,
            m_depthImageMemory );

        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;

        if( CSettings::Instance().activateStencilBuffer() )
            aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;

        m_depthImageView = createImageView( m_depthImage, depthFormat, 1, aspectFlags );

        transitionImageLayout( m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1 );
    }
}

/***************************************************************************
*   DESC:  Create the frame buffer
****************************************************************************/
void CDeviceVulkan::createFrameBuffer()
{
    VkResult vkResult(VK_SUCCESS);
    m_framebufferVec.resize( m_swapChainImageViewVec.size() );

    for( size_t i = 0; i < m_swapChainImageViewVec.size(); ++i )
    {
        std::vector<VkImageView> attachmentsAry = { m_swapChainImageViewVec[i] };

        // Add the depth image if depth or stencil buffer is needed
        if( CSettings::Instance().activateDepthBuffer() )
            attachmentsAry.push_back( m_depthImageView );

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = attachmentsAry.size();
        framebufferInfo.pAttachments = attachmentsAry.data();
        framebufferInfo.width = m_swapchainInfo.imageExtent.width;
        framebufferInfo.height = m_swapchainInfo.imageExtent.height;
        framebufferInfo.layers = 1;

        if( (vkResult = vkCreateFramebuffer( m_logicalDevice, &framebufferInfo, nullptr, &m_framebufferVec[i] )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create frame buffer! %s") % getError(vkResult) ) );
    }
}

/***************************************************************************
*   DESC:  Create the Semaphores and fences
****************************************************************************/
void CDeviceVulkan::createSyncObjects()
{
    VkResult vkResult(VK_SUCCESS);
    m_imageAvailableSemaphoreVec.resize( m_framebufferVec.size() );
    m_renderFinishedSemaphoreVec.resize( m_framebufferVec.size() );
    m_frameFenceVec.resize( m_framebufferVec.size() );

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for( size_t i = 0; i < m_framebufferVec.size(); ++i )
    {
        if( (vkResult = vkCreateSemaphore( m_logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphoreVec[i] )) ||
            (vkResult = vkCreateSemaphore( m_logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphoreVec[i] )) ||
            (vkResult = vkCreateFence( m_logicalDevice, &fenceInfo, nullptr, &m_frameFenceVec[i] )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create synchronization objects! %s") % getError(vkResult) ) );
    }
}

/***************************************************************************
*   DESC:  Create the command buffers
****************************************************************************/
void CDeviceVulkan::createPrimaryCommandBuffers()
{
    VkResult vkResult(VK_SUCCESS);
    m_primaryCmdBufVec.resize( m_framebufferVec.size() );

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = m_primaryCmdPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = (uint32_t) m_primaryCmdBufVec.size();

    if( (vkResult = vkAllocateCommandBuffers( m_logicalDevice, &commandBufferAllocateInfo, m_primaryCmdBufVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError(vkResult) ) );
}

/***************************************************************************
*   DESC:  Create the command buffers
****************************************************************************/
std::vector<VkCommandBuffer> CDeviceVulkan::createSecondaryCommandBuffers( VkCommandPool cmdPool )
{
    VkResult vkResult(VK_SUCCESS);
    std::vector<VkCommandBuffer>cmdBufVec( m_framebufferVec.size() );

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = cmdPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandBufferAllocateInfo.commandBufferCount = (uint32_t) cmdBufVec.size();

    if( (vkResult = vkAllocateCommandBuffers( m_logicalDevice, &commandBufferAllocateInfo, cmdBufVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError(vkResult) ) );

    return cmdBufVec;
}

/***************************************************************************
*   DESC:  Create the descriptor set layout
****************************************************************************/
VkDescriptorSetLayout CDeviceVulkan::createDescriptorSetLayout( SDescriptorData & descData )
{
    VkResult vkResult(VK_SUCCESS);
    int bindingOffset = 0;
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    for( auto & descIdIter : descData.m_descriptorVec )
    {
        // There can be multiple uniform buffers
        if( descIdIter.descrId == "UNIFORM_BUFFER" )
        {
            VkDescriptorSetLayoutBinding binding = {};
            binding.binding = bindingOffset++;
            binding.descriptorCount = 1;
            binding.pImmutableSamplers = nullptr;
            binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            bindings.push_back( binding );
        }
        else if( descIdIter.descrId == "COMBINED_IMAGE_SAMPLER" )
        {
            VkDescriptorSetLayoutBinding binding = {};
            binding.binding = bindingOffset++;
            binding.descriptorCount = 1;
            binding.pImmutableSamplers = nullptr;
            binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            bindings.push_back( binding );
        }
        else
        {
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Descriptor binding not defined! %s") % descIdIter.descrId ) );
        }
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    //layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR; // For enabling push descriptors
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    VkDescriptorSetLayout descriptorSetLayout;
    if( (vkResult = vkCreateDescriptorSetLayout( m_logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create descriptor set layout! %s") % getError(vkResult) ) );

    return descriptorSetLayout;
}

/***************************************************************************
*   DESC:  Create the pipeline layout
****************************************************************************/
VkPipelineLayout CDeviceVulkan::createPipelineLayout( VkDescriptorSetLayout descriptorSetLayout )
{
    VkResult vkResult(VK_SUCCESS);
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    VkPipelineLayout pipelineLayout;
    if( (vkResult = vkCreatePipelineLayout( m_logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create pipeline layout! %s") % getError(vkResult) ) );

    return pipelineLayout;
}

/***************************************************************************
*   DESC:  Create the pipeline
****************************************************************************/
void CDeviceVulkan::createPipeline( SPipelineData & pipelineData )
{
    VkResult vkResult(VK_SUCCESS);

    // Make sure our states are setup correctly
    if( pipelineData.depthTestEnable && !CSettings::Instance().activateDepthBuffer() )
        throw NExcept::CCriticalException(
            "Vulkan Error!", "Can't enable the depth buffer without activating it in the settings.cfg!" );

    if( pipelineData.depthTestEnable && !CSettings::Instance().activateStencilBuffer() )
        throw NExcept::CCriticalException(
            "Vulkan Error!", "Can't enable the stencil buffer without activating it in the settings.cfg!" );

    // Create the graphics pipeline
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = pipelineData.shader.vert;
    vertShaderStageInfo.pName = pipelineData.shader.vertFunc.c_str();

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = pipelineData.shader.frag;
    fragShaderStageInfo.pName = pipelineData.shader.fragFunc.c_str();

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = pipelineData.vertInputAttrDescVec.size();
    vertexInputInfo.pVertexBindingDescriptions = &pipelineData.vertInputBindingDesc;
    vertexInputInfo.pVertexAttributeDescriptions = pipelineData.vertInputAttrDescVec.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = m_swapchainInfo.imageExtent.width;
    viewport.height = m_swapchainInfo.imageExtent.height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    // Allow the viewport data to be changed by a registered slot
    m_deviceViewportSignal(viewport, pipelineData.id);

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = m_swapchainInfo.imageExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = pipelineData.depthClampEnable;                  // VK_FALSE
    rasterizer.rasterizerDiscardEnable = pipelineData.rasterizerDiscardEnable;    // VK_FALSE
    rasterizer.polygonMode = pipelineData.polygonMode;                            // VK_POLYGON_MODE_FILL
    rasterizer.lineWidth = pipelineData.lineWidth;                                // 1.0f
    rasterizer.cullMode = pipelineData.cullMode;                                  // VK_CULL_MODE_BACK_BIT
    rasterizer.frontFace = pipelineData.frontFace;                                // VK_FRONT_FACE_COUNTER_CLOCKWISE; // Counter clockwise for righthanded rule
    rasterizer.depthBiasEnable = pipelineData.depthBiasEnable;                    // VK_FALSE
    rasterizer.depthBiasConstantFactor = pipelineData.depthBiasConstantFactor;    // 0.f
    rasterizer.depthBiasClamp = pipelineData.depthBiasClamp;                      // 0.f
    rasterizer.depthBiasSlopeFactor = pipelineData.depthBiasSlopeFactor;          // 0.f

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = pipelineData.colorWriteMask; // VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    colorBlendAttachment.blendEnable = pipelineData.blendEnable;                        // VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = pipelineData.depthTestEnable;       // VK_FALSE
    depthStencil.depthWriteEnable = pipelineData.depthWriteEnable;     // VK_FALSE
    depthStencil.stencilTestEnable = pipelineData.stencilTestEnable;   // VK_FALSE
    depthStencil.depthCompareOp = pipelineData.depthCompareOp;         // VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = pipelineData.depthBoundsTestEnable; // VK_FALSE; // This is not a toggle to turn on depth testing
    // Stencil buffer masks
    depthStencil.back.compareOp = pipelineData.compareOp;              // VK_COMPARE_OP_EQUAL
    depthStencil.back.failOp = pipelineData.failOp;                    // VK_STENCIL_OP_KEEP
    depthStencil.back.depthFailOp = pipelineData.depthFailOp;          // VK_STENCIL_OP_KEEP
    depthStencil.back.passOp = pipelineData.passOp;                    // VK_STENCIL_OP_REPLACE
    depthStencil.back.compareMask = pipelineData.compareMask;          // 0xff
    depthStencil.back.writeMask = pipelineData.writeMask;              // 0xff
    depthStencil.back.reference = pipelineData.reference;              // 1
    depthStencil.front = depthStencil.back;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = VK_NULL_HANDLE;
    pipelineInfo.layout = pipelineData.pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    // Add the depthStencil if depth or stencil buffer is needed
    if( CSettings::Instance().activateDepthBuffer() )
        pipelineInfo.pDepthStencilState = &depthStencil;

    if( (vkResult = vkCreateGraphicsPipelines( m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipelineData.pipeline )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create graphics pipeline! %s") % getError(vkResult) ) );
}

/***************************************************************************
*   DESC:  Create the shader
****************************************************************************/
VkShaderModule CDeviceVulkan::createShader( const std::string & filePath )
{
    VkResult vkResult(VK_SUCCESS);
    std::vector<char> shaderVec = NGenFunc::FileToVec( filePath );

    VkShaderModuleCreateInfo shaderInfo = {};
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = shaderVec.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t*>(shaderVec.data());

    VkShaderModule shaderModule;
    if( (vkResult = vkCreateShaderModule( m_logicalDevice, &shaderInfo, nullptr, &shaderModule )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create shader! %s-%s") % filePath % getError(vkResult) ) );

    return shaderModule;
}

/***************************************************************************
*   DESC:  Recreate swap chain
****************************************************************************/
void CDeviceVulkan::recreateSwapChain()
{
    // Wait for the logical device to be idle before doing the clean up
    vkDeviceWaitIdle( m_logicalDevice );

    // Destroy the current swap chain
    destroySwapChain();

    // Setup the swap chain to be created
    setupSwapChain();

    // Create the swap chain
    createSwapChain();

    // Create the render pass
    createRenderPass();

    // Recreate the graphics pipeline
    recreatePipelines();

    // Create depth resources
    createDepthResources();

    // Create the frame buffer
    createFrameBuffer();
}

/***************************************************************************
*   DESC:  Find the GPU memory type
****************************************************************************/
uint32_t CDeviceVulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties( m_phyDevVec[m_phyDevIndex].pDev, &memProperties );

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw NExcept::CCriticalException( "Vulkan Error!", "Failed to find suitable memory type!" );
}

/***************************************************************************
*   DESC:  Check if the device extension is supported
****************************************************************************/
bool CDeviceVulkan::isDeviceExtension( VkPhysicalDevice physicalDevice, const char* extenName )
{
    VkResult vkResult(VK_SUCCESS);
    uint32_t deviceExtensionCount = 0;
    if( (vkResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, nullptr )) || (deviceExtensionCount == 0 ) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate device extension count! %s") % getError(vkResult) ) );

    std::vector<VkExtensionProperties> availableExtensions(deviceExtensionCount);

    if( (vkResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, availableExtensions.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate device extension info! %s") % getError(vkResult) ) );

    for( auto iter : availableExtensions )
        if( std::strcmp( iter.extensionName, extenName ) == 0 )
            return true;

    return false;
}

/***************************************************************************
*   DESC:  get the queue family present index and decrement the queue count
*          NOTE: If the queue is in the same family as the graphic
*                then the graphics queue will be used
****************************************************************************/
uint32_t CDeviceVulkan::getPresentQueueFamilyIndex()
{
    if( m_phyDevIndex == UINT32_MAX )
        throw NExcept::CCriticalException( "Vulkan Error!", "Need to select physical device before query for queue families!" );

    // Find family present index
    for( uint32_t i = 0; i < m_phyDevVec[m_phyDevIndex].queueFamilyPropVec.size(); ++i )
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_phyDevVec[m_phyDevIndex].pDev, i, m_vulkanSurface, &presentSupport);

        if( presentSupport && (m_phyDevVec[m_phyDevIndex].queueFamilyPropVec[i].queueCount > 0) )
        {
            // Decrement the queue count for this family if it's not the same family index as the graphic queue
            // This vector is used for handing out available queues from each family
            if( i != m_graphicsQueueFamilyIndex )
                m_phyDevVec[m_phyDevIndex].queueFamilyPropVec[i].queueCount--;

            return i;
        }
    }

    return UINT32_MAX;
}

/***************************************************************************
*   DESC:  Get the queue family index
****************************************************************************/
uint32_t CDeviceVulkan::getQueueFamilyIndex( CPhysicalDevice & phyDev, uint32_t queueMask )
{
    // Find family based on bit flag
    for( uint32_t i = 0; i < phyDev.queueFamilyPropVec.size(); i++ )
    {
        if( (phyDev.queueFamilyPropVec[i].queueCount > 0) &&
            (phyDev.queueFamilyPropVec[i].queueFlags & queueMask) )
        {
            // Decrement the queue count for this family
            // This vector is used for handing out available queues from each family
            phyDev.queueFamilyPropVec[i].queueCount--;

            return i;
        }
    }

    return UINT32_MAX;
}

/***************************************************************************
*   DESC:  Find supported format
****************************************************************************/
VkFormat CDeviceVulkan::findSupportedFormat( const std::vector<VkFormat> & candidates, VkImageTiling tiling, VkFormatFeatureFlags features )
{
    for( VkFormat format : candidates )
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties( m_phyDevVec[m_phyDevIndex].pDev, format, &props );

        if( tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features )
            return format;

        else if( tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features )
            return format;
    }

    return VK_FORMAT_UNDEFINED;
}

/***************************************************************************
*   DESC:  Find the depth format
****************************************************************************/
VkFormat CDeviceVulkan::findDepthFormat()
{
    std::vector<VkFormat> depthStencilFormatChoices;

    // Check only the formats that include the stencil buffer if stencil is needed
    if( CSettings::Instance().activateStencilBuffer() )
        depthStencilFormatChoices = {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    else
        depthStencilFormatChoices = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

    VkFormat format = findSupportedFormat( depthStencilFormatChoices, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );

    // If an optimal one is not found, try for a linear one
    if( format == VK_FORMAT_UNDEFINED )
        format = findSupportedFormat( depthStencilFormatChoices, VK_IMAGE_TILING_LINEAR, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );

    return format;
}

/***************************************************************************
*   DESC:  Create image
****************************************************************************/
void CDeviceVulkan::createImage(
    uint32_t width,
    uint32_t height,
    uint32_t mipLevels,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkImage & image,
    VkDeviceMemory & imageMemory )
{
    VkResult vkResult(VK_SUCCESS);
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if( (vkResult = vkCreateImage( m_logicalDevice, &imageInfo, nullptr, &image )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create image! %s") % getError(vkResult) ) );

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements( m_logicalDevice, image, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if( (vkResult = vkAllocateMemory( m_logicalDevice, &allocInfo, nullptr, &imageMemory )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate image memory! %s") % getError(vkResult) ) );

    if( (vkResult = vkBindImageMemory( m_logicalDevice, image, imageMemory, 0 ) ) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not bind image memory! %s") % getError(vkResult) ) );
}

/***************************************************************************
*   DESC:  Create a buffer
****************************************************************************/
void CDeviceVulkan::createBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer & buffer,
    VkDeviceMemory & bufferMemory )
{
    VkResult vkResult(VK_SUCCESS);
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if( (vkResult = vkCreateBuffer( m_logicalDevice, &bufferInfo, nullptr, &buffer )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create buffer! %s") % getError(vkResult) ) );

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( m_logicalDevice, buffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if( (vkResult = vkAllocateMemory( m_logicalDevice, &allocInfo, nullptr, &bufferMemory )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate buffer memory! %s") % getError(vkResult) ) );

    if( (vkResult = vkBindBufferMemory( m_logicalDevice, buffer, bufferMemory, 0) ) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not bind buffer memory! %s") % getError(vkResult) ) );
}

/***************************************************************************
*   DESC:  Copy a buffer
****************************************************************************/
void CDeviceVulkan::copyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size )
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion );

    endSingleTimeCommands( commandBuffer );
}

/***************************************************************************
*   DESC:  Find the queue family index
****************************************************************************/
VkCommandBuffer CDeviceVulkan::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_transferCmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( m_logicalDevice, &allocInfo, &commandBuffer );

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void CDeviceVulkan::endSingleTimeCommands( VkCommandBuffer commandBuffer )
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit( m_transferQueue, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( m_transferQueue );

    vkFreeCommandBuffers( m_logicalDevice, m_transferCmdPool, 1, &commandBuffer);
}

/***************************************************************************
*   DESC:  Transition image layout
****************************************************************************/
void CDeviceVulkan::transitionImageLayout( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels )
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if( newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL )
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if( format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT )
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    else
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL )
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
    {
        throw NExcept::CCriticalException( "Vulkan Error!", "unsupported layout transition!" );
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    endSingleTimeCommands( commandBuffer );
}

/***************************************************************************
*   DESC:  Copy a buffer to an image
****************************************************************************/
void CDeviceVulkan::copyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height )
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage( commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );
    endSingleTimeCommands( commandBuffer );
}

/***************************************************************************
*   DESC:  Create the command pool
****************************************************************************/
VkCommandPool CDeviceVulkan::createCommandPool( uint32_t queueFamilyIndex )
{
    VkResult vkResult(VK_SUCCESS);
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkCommandPool commandPool;
    if( (vkResult = vkCreateCommandPool( m_logicalDevice, &poolInfo, nullptr, &commandPool )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create command pool! %s") % getError(vkResult) ) );

    return commandPool;
}

/***************************************************************************
*   DESC:  Create texture
****************************************************************************/
void CDeviceVulkan::createTexture( CTexture & texture )
{
    int channels(0);
    unsigned char * pixels = SOIL_load_image(
        texture.textFilePath.c_str(),
        &texture.size.w,
        &texture.size.h,
        &channels,
        SOIL_LOAD_RGBA );

    if( pixels == nullptr )
        throw NExcept::CCriticalException(
            "SOIL Error!", 
            boost::str( boost::format("Error loading image! %s") % texture.textFilePath ));

    VkDeviceSize imageSize = texture.size.w * texture.size.h * SOIL_LOAD_RGBA;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(
        imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory );

    void* data;
    vkMapMemory( m_logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data );
    std::memcpy( data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory( m_logicalDevice, stagingBufferMemory );

    SOIL_free_image_data( pixels );

    uint32_t imageUsageFlags( VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT );

    // Calculate the mip levels based on the size of the texture
    if( texture.genMipLevels )
    {
        imageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        texture.mipLevels = std::floor(std::log2(std::max(texture.size.w, texture.size.h))) + 1;
    }

    createImage(
        texture.size.w,
        texture.size.h,
        texture.mipLevels,
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_IMAGE_TILING_OPTIMAL,
        imageUsageFlags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        texture.textureImage,
        texture.textureImageMemory );

    transitionImageLayout( texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture.mipLevels );
    copyBufferToImage( stagingBuffer, texture.textureImage, static_cast<uint32_t>(texture.size.w), static_cast<uint32_t>(texture.size.h) );

    if( texture.genMipLevels )
        generateMipmaps( texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, texture.size.w, texture.size.h, texture.mipLevels );
    else
        transitionImageLayout( texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, texture.mipLevels );

    vkDestroyBuffer( m_logicalDevice, stagingBuffer, nullptr );
    vkFreeMemory( m_logicalDevice, stagingBufferMemory, nullptr );
    
    // create the image view
    texture.textureImageView = createImageView( texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, texture.mipLevels, VK_IMAGE_ASPECT_COLOR_BIT );

    // Create the texture sampler
    texture.textureSampler = createTextureSampler( texture );
}

/***************************************************************************
*   DESC:  Generate Mipmaps
****************************************************************************/
void CDeviceVulkan::generateMipmaps( VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels )
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties( m_phyDevVec[m_phyDevIndex].pDev, imageFormat, &formatProperties );

    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        throw NExcept::CCriticalException( "Vulkan Error!", "texture image format does not support linear blitting!" );

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = width;
    int32_t mipHeight = height;

    for (uint32_t i = 1; i < mipLevels; i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        VkImageBlit blit = {};
        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(commandBuffer,
            image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    endSingleTimeCommands(commandBuffer);
}

/***************************************************************************
*   DESC:  Create texture sampler
****************************************************************************/
VkSampler CDeviceVulkan::createTextureSampler( CTexture & texture )
{
    VkResult vkResult(VK_SUCCESS);
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = texture.magFilter;  // Default: VK_FILTER_LINEAR
    samplerInfo.minFilter = texture.minFilter;  // Default: VK_FILTER_LINEAR
    // Using repeat mode because clamp mode will return a solid color if sampling beyond the image
    // Have seen clamp mode cause some flickering on Android with fonts
    samplerInfo.addressModeU = texture.samplerAddressModeU;  // Default: VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
    samplerInfo.addressModeV = texture.samplerAddressModeV;  // Default: VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
    samplerInfo.addressModeW = texture.samplerAddressModeW;  // Default: VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
    samplerInfo.anisotropyEnable = ((CSettings::Instance().getAnisotropicLevel() > 0) ? VK_TRUE : VK_FALSE);
    samplerInfo.maxAnisotropy = CSettings::Instance().getAnisotropicLevel();
    samplerInfo.borderColor = texture.borderColor;
    samplerInfo.unnormalizedCoordinates = texture.unnormalizedCoordinates;  // Default: VK_FALSE
    samplerInfo.compareEnable = texture.compareEnable;                      // Default: VK_FALSE
    samplerInfo.compareOp = texture.compareOp;    // Default: VK_COMPARE_OP_ALWAYS
    samplerInfo.mipmapMode = texture.mipmapMode;  // Default: VK_SAMPLER_MIPMAP_MODE_LINEAR
    samplerInfo.mipLodBias = texture.mipLodBias;  // Default: 0.0f
    samplerInfo.minLod = texture.minLod;          // Default: 0.0f
    samplerInfo.maxLod = ((texture.mipLevels > 1) ? texture.mipLevels : 0.f);

    VkSampler textureSampler;

    if( (vkResult = vkCreateSampler( m_logicalDevice, &samplerInfo, nullptr, &textureSampler )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create texture sampler! %s") % getError(vkResult) ) );

    return textureSampler;
}

/***************************************************************************
*   DESC:  Create the image view
****************************************************************************/
VkImageView CDeviceVulkan::createImageView( VkImage image, VkFormat format, uint32_t mipLevels, VkImageAspectFlags aspectFlags )
{
    VkResult vkResult(VK_SUCCESS);
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    // imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    // imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    // imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    // imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if( (vkResult = vkCreateImageView( m_logicalDevice, &viewInfo, nullptr, &imageView )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create texture image view! %s") % getError(vkResult) ) );

    return imageView;
}

/***************************************************************************
*   DESC:  Create descriptor pool
*          NOTE: Each discriptor count must equal the total number needed in the pool
****************************************************************************/
VkDescriptorPool CDeviceVulkan::createDescriptorPool( const SDescriptorData & descData )
{
    VkResult vkResult(VK_SUCCESS);
    std::vector<VkDescriptorPoolSize> descriptorPoolVec;
    descriptorPoolVec.reserve( descData.m_descriptorVec.size() );
    const uint32_t MAX_POOL_SIZE( m_framebufferVec.size() * descData.descPoolMax );

    for( auto & descIdIter : descData.m_descriptorVec )
    {
        // There can be multiple uniform buffers
        if( descIdIter.descrId == "UNIFORM_BUFFER" )
        {
            VkDescriptorPoolSize uniformBufferPoolSize = {};
            uniformBufferPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uniformBufferPoolSize.descriptorCount = MAX_POOL_SIZE;

            descriptorPoolVec.push_back( uniformBufferPoolSize );
        }
        else if( descIdIter.descrId == "COMBINED_IMAGE_SAMPLER" )
        {
            VkDescriptorPoolSize combinedImageSamplerPoolSize = {};
            combinedImageSamplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            combinedImageSamplerPoolSize.descriptorCount = MAX_POOL_SIZE;

            descriptorPoolVec.push_back( combinedImageSamplerPoolSize );
        }
        else
        {
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Descriptor pool binding not defined! %s") % descIdIter.descrId ) );
        }
    }

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = descriptorPoolVec.size();
    poolInfo.pPoolSizes = descriptorPoolVec.data();
    poolInfo.maxSets = MAX_POOL_SIZE;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    VkDescriptorPool descriptorPool( VK_NULL_HANDLE );

    if( (vkResult = vkCreateDescriptorPool( m_logicalDevice, &poolInfo, nullptr, &descriptorPool )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError(vkResult) ) );

    return descriptorPool;
}

/***************************************************************************
*   DESC:  Allocate the descriptor sets
****************************************************************************/
std::vector<VkDescriptorSet> CDeviceVulkan::allocateDescriptorSetVec(
    const SPipelineData & pipelineData,
    VkDescriptorPool descriptorPool )
{
    VkResult vkResult(VK_SUCCESS);
    std::vector<VkDescriptorSetLayout> layouts( m_framebufferVec.size(), pipelineData.descriptorSetLayout );

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = layouts.size();
    allocInfo.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> descriptorSetVec( m_framebufferVec.size() );

    if( (vkResult = vkAllocateDescriptorSets( m_logicalDevice, &allocInfo, descriptorSetVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate descriptor sets! %s") % getError(vkResult) ) );

    return descriptorSetVec;
}

/***************************************************************************
*   DESC:  update descriptor sets
****************************************************************************/
void CDeviceVulkan::updateDescriptorSetVec(
    std::vector<VkDescriptorSet> & descriptorSetVec,
    const CTexture & texture,
    const SDescriptorData & descData,
    const std::vector<CMemoryBuffer> & uniformBufVec )
{
    for( size_t i = 0; i < descriptorSetVec.size(); ++i )
    {
        std::vector<VkWriteDescriptorSet> writeDescriptorSetVec;

        // Keep the data alive until the call to vkUpdateDescriptorSets
        std::vector<VkDescriptorBufferInfo> descriptorBufferInfoVec;
        std::vector<VkDescriptorImageInfo> descriptorImageInfoVec;

        int bindingOffset = 0;

        for( auto & descIdIter : descData.m_descriptorVec )
        {
            if( descIdIter.descrId == "UNIFORM_BUFFER" )
            {
                // Make sure this UBO has a size
                if( descIdIter.ubo.uboSize == 0 )
                    throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Uniform Buffer UBO size is 0! %s") % descIdIter.descrId ) );

                VkDescriptorBufferInfo bufferInfo = {};
                bufferInfo.buffer = uniformBufVec[i].m_buffer;
                bufferInfo.offset = 0;
                bufferInfo.range = descIdIter.ubo.uboSize;

                descriptorBufferInfoVec.emplace_back( bufferInfo );

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstSet = descriptorSetVec[i];
                writeDescriptorSet.dstBinding = bindingOffset++;
                writeDescriptorSet.dstArrayElement = 0;
                writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                writeDescriptorSet.descriptorCount = 1;
                writeDescriptorSet.pBufferInfo = &descriptorBufferInfoVec.back();

                writeDescriptorSetVec.push_back( writeDescriptorSet );
            }
            else if( descIdIter.descrId == "COMBINED_IMAGE_SAMPLER" )
            {
                VkDescriptorImageInfo imageInfo = {};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = texture.textureImageView;
                imageInfo.sampler = texture.textureSampler;

                descriptorImageInfoVec.emplace_back( imageInfo );

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstSet = descriptorSetVec[i];
                writeDescriptorSet.dstBinding = bindingOffset++;
                writeDescriptorSet.dstArrayElement = 0;
                writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                writeDescriptorSet.descriptorCount = 1;
                writeDescriptorSet.pImageInfo = &descriptorImageInfoVec.back();

                writeDescriptorSetVec.push_back( writeDescriptorSet );
            }
            else
            {
                throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Create Descriptor Set binding not defined! %s") % descIdIter.descrId ) );
            }
        }

        vkUpdateDescriptorSets( m_logicalDevice, writeDescriptorSetVec.size(), writeDescriptorSetVec.data(), 0, nullptr );
    }
}

/***************************************************************************
*   DESC:  Create the uniform buffer Vec for ubo buffer writes
****************************************************************************/
std::vector<CMemoryBuffer> CDeviceVulkan::createUniformBufferVec( VkDeviceSize sizeOfUniformBuf )
{
    std::vector<CMemoryBuffer> uniformBufVec( m_framebufferVec.size() );

    for( size_t i = 0; i < m_framebufferVec.size(); ++i )
        CDeviceVulkan::createBuffer(
            sizeOfUniformBuf,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBufVec[i].m_buffer,
            uniformBufVec[i].m_deviceMemory );

    return uniformBufVec;
}

/***************************************************************************
*   DESC:  Get Vulkan error
****************************************************************************/
const char * CDeviceVulkan::getError( VkResult result )
{
    std::unique_lock<std::mutex> lock( m_mutex );

    auto iter = m_vulkanErrorMap.find( result );
    if( iter != m_vulkanErrorMap.end() )
        return iter->second;

    return "Vulkan Unknown Error";
}

/***************************************************************************
*   DESC:  Get the GPU type
****************************************************************************/
const char * CDeviceVulkan::getDeviceType( VkPhysicalDeviceType deviceType )
{
    if( deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
        return "Integrated GPU";

    else if( deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        return "Descrete GPU";

    else if( deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU )
        return "Virtual GPU";

    else if( deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU )
        return "CPU";

    return "Unknown";
}

/***************************************************************************
*   DESC:  Indicates if surface presentation mode is available
****************************************************************************/
bool CDeviceVulkan::isSurfacePresMode(std::vector<VkPresentModeKHR> & surfacePresModeVec, VkPresentModeKHR mode)
{
    bool found = false;
    for( auto & presentMode : surfacePresModeVec )
    {
        if(mode == presentMode )
        {
            found = true;
            break;
        }
    }

    return found;
}

/***************************************************************************
*   DESC:  Print debug info
****************************************************************************/
void CDeviceVulkan::printDebug( const VkApplicationInfo & appInfo )
{
    NGenFunc::PostDebugMsg( 
        boost::str( boost::format(
            "Application Info...\n"
            "  Structure Type: %u\n"
            "  App Name: %s\n"
            "  App Version: %u\n"
            "  Engine Name: %s\n"
            "  Engine Version: %u\n"
            "  API Version: %u\n" )
            % appInfo.sType
            % appInfo.pApplicationName
            % appInfo.applicationVersion
            % appInfo.pEngineName
            % appInfo.engineVersion
            % appInfo.apiVersion ));
}

void CDeviceVulkan::printDebug( const CPhysicalDevice & phyDev, uint32_t index )
{
    NGenFunc::PostDebugMsg( 
        boost::str( boost::format(
            "Physical Device Properties (%u)...\n"
            "  GPU Name: %s\n"
            "  GPU Type: %s\n"
            "  GPU Driver Version: %u\n"
            "  GPU Vender Id: %u\n"
            "  GPU Device Id: %u\n"
            "  Max Image 1D: %u\n"
            "  Max Image 2D: %u\n"
            "  Max Image 3D: %u\n"
            "  Max Viewports: %u\n")
            % index
            % phyDev.prop.deviceName
            % getDeviceType(phyDev.prop.deviceType)
            % phyDev.prop.driverVersion
            % phyDev.prop.vendorID
            % phyDev.prop.deviceID
            % phyDev.prop.limits.maxImageDimension1D
            % phyDev.prop.limits.maxImageDimension2D
            % phyDev.prop.limits.maxImageDimension3D
            % phyDev.prop.limits.maxViewports ));

    for( uint32_t i = 0; i < phyDev.queueFamilyPropVec.size(); ++i )
    {
        NGenFunc::PostDebugMsg( 
            boost::str( boost::format(
                "  Queue Family Properties (%u)...\n"
                "    Queue Flags: b%s\n"
                "    Queue Count: %u\n"
                "    Timestamp Valid Bits: 0x%04x\n"
                "    Min Image Transfer Granularity: %u x %u x %u\n"
                "    Graphics Bit: %s\n"
                "    Compute Bit: %s\n"
                "    Transfer Bit: %s\n"
                "    Sparse Binding Bit: %s\n"
                "    Protected Bit: %s\n" )
                % i
                % std::bitset<32>(phyDev.queueFamilyPropVec[i].queueFlags).to_string()
                % phyDev.queueFamilyPropVec[i].queueCount
                % phyDev.queueFamilyPropVec[i].timestampValidBits
                % phyDev.queueFamilyPropVec[i].minImageTransferGranularity.width
                % phyDev.queueFamilyPropVec[i].minImageTransferGranularity.height
                % phyDev.queueFamilyPropVec[i].minImageTransferGranularity.depth
                % (phyDev.queueFamilyPropVec[i].queueFlags & VK_QUEUE_GRAPHICS_BIT ? "true" : "false")
                % (phyDev.queueFamilyPropVec[i].queueFlags & VK_QUEUE_COMPUTE_BIT ? "true" : "false")
                % (phyDev.queueFamilyPropVec[i].queueFlags & VK_QUEUE_TRANSFER_BIT ? "true" : "false")
                % (phyDev.queueFamilyPropVec[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT ? "true" : "false")
                % (phyDev.queueFamilyPropVec[i].queueFlags & VK_QUEUE_PROTECTED_BIT ? "true" : "false") ));
    }
}

void CDeviceVulkan::printDebugPhyDev()
{
    std::string devTypeStr = "Other";
    if( m_phyDevVec[m_phyDevIndex].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
        devTypeStr = "Integrated GPU";

    else if( m_phyDevVec[m_phyDevIndex].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        devTypeStr = "Discrete GPU";

    else if( m_phyDevVec[m_phyDevIndex].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU )
        devTypeStr = "Virtual GPU";
    
    else if( m_phyDevVec[m_phyDevIndex].prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU )
        devTypeStr = "CPU";

    NGenFunc::PostDebugMsg( 
        boost::str( boost::format(
            "Selected Physical Device Type: %s\n"
            "  %s Physical Device Index: %u\n"
            "  %s Graphics Queue Family Index: %u\n"
            "  %s Transfer Queue Family Index: %u\n"
            "  %s Presentation Queue Family Index: %u\n" )
            % devTypeStr
            % (m_phyDevIndex < UINT32_MAX ? "SELECTED" : "UNAVAILABLE")
            % m_phyDevIndex
            % (m_graphicsQueueFamilyIndex < UINT32_MAX ? "SELECTED" : "UNAVAILABLE")
            % m_graphicsQueueFamilyIndex
            % (m_transferQueueFamilyIndex < UINT32_MAX ? "SELECTED" : "UNAVAILABLE")
            % m_transferQueueFamilyIndex
            % (m_presentQueueFamilyIndex < UINT32_MAX ? "SELECTED" : "UNAVAILABLE")
            % m_presentQueueFamilyIndex ));
}

void CDeviceVulkan::printDebug( const std::string & title, const std::vector<const char*> & dataVec )
{
    if( !dataVec.empty() )
    {
        std::string buf;
        for( auto iter : dataVec )
            buf += "  " + std::string(iter) + "\n";

        NGenFunc::PostDebugMsg( 
            boost::str( boost::format("%s...\n%s")
                % title
                % buf ));
    }
}

void CDeviceVulkan::printDebug( const VkSurfaceCapabilitiesKHR & surfCap )
{
    NGenFunc::PostDebugMsg( 
        boost::str( boost::format(
            "Surface Capabilities...\n"
            "  Min Image Count: %u\n"
            "  Max Image Count: %u\n"
            "  Current Extent: %u x %u\n"
            "  Min Image Extent: %u x %u\n"
            "  Max Image Extent: %u x %u\n"
            "  Max Image Array Layers: %u\n"
            "  Supported Transform Flags: b%s\n"
            "  Current Transform Flags: 0x%04x\n"
            "  Supported Composite Alpha Flags: b%s\n"
            "  Supported Usage Flags: b%s\n" )
            % surfCap.minImageCount
            % surfCap.maxImageCount
            % surfCap.currentExtent.width
            % surfCap.currentExtent.height
            % surfCap.minImageExtent.width
            % surfCap.minImageExtent.height
            % surfCap.maxImageExtent.width
            % surfCap.maxImageExtent.height
            % surfCap.maxImageArrayLayers
            % std::bitset<32>(surfCap.supportedTransforms).to_string()
            % std::bitset<32>(surfCap.currentTransform).to_string()
            % std::bitset<32>(surfCap.supportedCompositeAlpha).to_string()
            % std::bitset<32>(surfCap.supportedUsageFlags).to_string() ));
}

void CDeviceVulkan::printDebug( const std::vector<VkSurfaceFormatKHR> & surfFormatVec, const VkSurfaceFormatKHR & surfFormat )
{
    NGenFunc::PostDebugMsg( "Surface Format(s)...\n" );

    for( auto & iter : surfFormatVec)
    {
        NGenFunc::PostDebugMsg( 
            boost::str( boost::format(
                "%s"
                "  Format: %u\n"
                "  Color Space: %u\n" )
                % (surfFormat.format == iter.format && surfFormat.colorSpace == iter.colorSpace ? "  SELECTED\n" : "")
                % iter.format
                % iter.colorSpace ));
    }
}

void CDeviceVulkan::printDebug( const VkSwapchainCreateInfoKHR & swapchainInfo )
{
    NGenFunc::PostDebugMsg( 
        boost::str( boost::format(
            "Swapchain Create Info...\n"
            "  sType: %u\n"
            "  flags: b%s\n"
            "  Min Image Count: %u\n"
            "  Image Extent: %u x %u\n"
            "  Image Array Layers: %u\n"
            "  Image Sharing Mode: 0x%04x\n"
            "  Pretransform: 0x%04x\n"
            "  Composite Alpha: 0x%04x\n"
            "  Present Mode: b%s\n"
            "  Clipped: %s\n" )
            % swapchainInfo.sType
            % std::bitset<32>(swapchainInfo.flags).to_string()
            % swapchainInfo.minImageCount
            % swapchainInfo.imageExtent.width
            % swapchainInfo.imageExtent.height
            % swapchainInfo.imageArrayLayers
            % swapchainInfo.imageSharingMode
            % swapchainInfo.preTransform
            % swapchainInfo.compositeAlpha
            % std::bitset<32>(swapchainInfo.presentMode).to_string()
            % (swapchainInfo.clipped ? "true" : "false") ));
}

void CDeviceVulkan::printDebug( const std::vector<VkDeviceQueueCreateInfo> & queueInfoVec )
{
    NGenFunc::PostDebugMsg( "Device Queue Create Info...\n" );

    for( auto & iter : queueInfoVec)
    {
        NGenFunc::PostDebugMsg( 
            boost::str( boost::format(
                "  sType: %u\n"
                "  flags: b%s\n"
                "  Queue Family Index: %u\n"
                "  Queue Count: %u\n" )
                % iter.sType
                % std::bitset<32>(iter.flags).to_string()
                % iter.queueFamilyIndex
                % iter.queueCount ));
    }
}