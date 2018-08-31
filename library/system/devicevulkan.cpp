
/************************************************************************
*    FILE NAME:       devicevulkan.h
*
*    DESCRIPTION:     Class used for Vulkan API implementation
************************************************************************/

// Physical component dependency
#include <system/devicevulkan.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <utilities/matrix.h>
#include <common/size.h>
#include <common/vertex.h>
#include <soil/SOIL.h>

// Standard lib dependencies
#include <fstream>
#include <array>

// Boost lib dependencies
#include <boost/format.hpp>


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

struct UniformBufferObject {
    CMatrix model;
    CMatrix view;
    CMatrix proj;
};

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CDeviceVulkan::CDeviceVulkan() :
    m_vulkanInstance(VK_NULL_HANDLE),
    m_vulkanSurface(VK_NULL_HANDLE),
    m_physicalDevice(VK_NULL_HANDLE),
    m_logicalDevice(VK_NULL_HANDLE),
    m_graphicsQueueFamilyIndex(VK_NULL_HANDLE),
    m_presentQueueFamilyIndex(VK_NULL_HANDLE),
    m_graphicsQueue(VK_NULL_HANDLE),
    m_presentQueue(VK_NULL_HANDLE),
    m_swapchain(VK_NULL_HANDLE),
    m_pipelineLayout(VK_NULL_HANDLE),
    m_descriptorSetLayout(VK_NULL_HANDLE),
    m_renderPass(VK_NULL_HANDLE),
    m_graphicsPipeline(VK_NULL_HANDLE),
    m_primaryCmdPool(VK_NULL_HANDLE),
    m_currentFrame(0),
    m_maxConcurrentFrames(0),
    m_lastResult(VK_SUCCESS),
    m_depthImage(VK_NULL_HANDLE),
    m_depthImageMemory(VK_NULL_HANDLE),
    m_depthImageView(VK_NULL_HANDLE),
    vkDestroySwapchainKHR(VK_NULL_HANDLE),
    vkGetSwapchainImagesKHR(VK_NULL_HANDLE),
    vkDebugReportCallbackEXT(VK_NULL_HANDLE),
    vkDestroyDebugReportCallbackEXT(nullptr)
{
    m_vulkanErrorMap.emplace( VK_SUCCESS,                        "Vulkan Success!" );
    m_vulkanErrorMap.emplace( VK_NOT_READY,                      "Vulkan Not Ready!" );
    m_vulkanErrorMap.emplace( VK_TIMEOUT,                        "Vulkan Timeout!" );
    m_vulkanErrorMap.emplace( VK_EVENT_SET,                      "Vulkan Event Set!" );
    m_vulkanErrorMap.emplace( VK_EVENT_RESET,                    "Vulkan Event Reset!" );
    m_vulkanErrorMap.emplace( VK_INCOMPLETE,                     "Vulkan Incomplete!" );
    m_vulkanErrorMap.emplace( VK_ERROR_OUT_OF_HOST_MEMORY,       "Vulkan Out Of Host memory!" );
    m_vulkanErrorMap.emplace( VK_ERROR_OUT_OF_DEVICE_MEMORY,     "Vulkan Out Of Device Memory!" );
    m_vulkanErrorMap.emplace( VK_ERROR_INITIALIZATION_FAILED,    "Vulkan Initialization Failed!" );
    m_vulkanErrorMap.emplace( VK_ERROR_DEVICE_LOST,              "Vulkan Device Lost!" );
    m_vulkanErrorMap.emplace( VK_ERROR_MEMORY_MAP_FAILED,        "Vulkan Memory Map Failed!" );
    m_vulkanErrorMap.emplace( VK_ERROR_LAYER_NOT_PRESENT,        "Vulkan Layer Not Present!" );
    m_vulkanErrorMap.emplace( VK_ERROR_EXTENSION_NOT_PRESENT,    "Vulkan nExtension Not Present!" );
    m_vulkanErrorMap.emplace( VK_ERROR_FEATURE_NOT_PRESENT,      "Vulkan Feature Not Present!" );
    m_vulkanErrorMap.emplace( VK_ERROR_INCOMPATIBLE_DRIVER,      "Vulkan Incompatible Driver!" );
    m_vulkanErrorMap.emplace( VK_ERROR_TOO_MANY_OBJECTS,         "Vulkan Too Many Objects!" );
    m_vulkanErrorMap.emplace( VK_ERROR_FORMAT_NOT_SUPPORTED,     "Vulkan Format Not Supported!" );
    m_vulkanErrorMap.emplace( VK_ERROR_FRAGMENTED_POOL,          "Vulkan Fragmented Pool!" );
    m_vulkanErrorMap.emplace( VK_ERROR_OUT_OF_POOL_MEMORY,       "Vulkan Out Of Pool Memory!" );
    m_vulkanErrorMap.emplace( VK_ERROR_INVALID_EXTERNAL_HANDLE,  "Vulkan Invalid External Handle!" );
    m_vulkanErrorMap.emplace( VK_ERROR_SURFACE_LOST_KHR,         "Vulkan Surface Lost KHR!" );
    m_vulkanErrorMap.emplace( VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "Vulkan Native Window In Use KHR!" );
    m_vulkanErrorMap.emplace( VK_SUBOPTIMAL_KHR,                 "Vulkan Suboptimal KHR!" );
    m_vulkanErrorMap.emplace( VK_ERROR_OUT_OF_DATE_KHR,          "Vulkan Out Of Date KHR!" );
    m_vulkanErrorMap.emplace( VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "Vulkan Incompatible Display KHR!" );
    m_vulkanErrorMap.emplace( VK_ERROR_VALIDATION_FAILED_EXT,    "Vulkan Validation Failed Ext!" );
    m_vulkanErrorMap.emplace( VK_ERROR_INVALID_SHADER_NV,        "Vulkan Invalid Shader NV!" );
    m_vulkanErrorMap.emplace( VK_ERROR_NOT_PERMITTED_EXT,        "Vulkan Not Permitted Ext!" );
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
    const std::string & vertShader,
    const std::string & fragShader )
{
    #if defined(__ANDROID__)
    if( InitVulkan() == 0 )
        throw NExcept::CCriticalException( "Vulkan Error!", "Could not initialize Vulkan library!" );
    #endif

    // Determines how many frames are going through the pipeline simultaneously. Unrelated to buffering.
    m_maxConcurrentFrames = CSettings::Instance().getMaxConcurrentFrameRender() + 1;

    // Sanity check. This should never happen
    if( m_maxConcurrentFrames == 0 )
        throw NExcept::CCriticalException( "Vulkan Error!", "Max concurrent frames can't be zero!" );

    // Create the vulkan instance
    createVulkanInstance( validationNameVec, instanceExtensionNameVec );
    
    // Create the Vulkan surface
    createSurface();
    
    // Select a physical device (GPU)
    selectPhysicalDevice();
    
    // Create the logical device
    createLogicalDevice( validationNameVec );
    
    // Load the shaders
    loadShaders( vertShader, fragShader );
    
    // Setup the swap chain to be created
    setupSwapChain();
    
    // Create the descriptor set layout
    createDescriptorSetLayout();
    
    // Create the swap chain
    createSwapChain();
    
    // Create the render pass
    createRenderPass();
    
    // Create the graphics pipeline
    createGraphicsPipeline();
    
    // Create the primary command pool
    createPrimaryCommandPool();
    
    // Create depth resources
    createDepthResources();
    
    // Create the frame buffer
    createFrameBuffer();
    
    // Create uniform buffers
    createUniformBuffer();
    
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
        // Wait for the logical device to be idle before doing the clean up
        vkDeviceWaitIdle( m_logicalDevice );
        
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

        if( !m_uniformBufVec.empty()  )
        {
            for( auto iter : m_uniformBufVec )
                vkDestroyBuffer( m_logicalDevice, iter, nullptr );
            
            m_uniformBufVec.clear();
        }
        
        if( !m_uniformBufMemVec.empty()  )
        {
            for( auto iter : m_uniformBufMemVec )
                vkFreeMemory( m_logicalDevice, iter, nullptr );
            
            m_uniformBufMemVec.clear();
        }

        if( m_primaryCmdPool != VK_NULL_HANDLE )
        {
            vkDestroyCommandPool( m_logicalDevice, m_primaryCmdPool, nullptr );
            m_primaryCmdPool = VK_NULL_HANDLE;
        }
        
        if( m_descriptorSetLayout != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorSetLayout( m_logicalDevice, m_descriptorSetLayout, nullptr );
            m_descriptorSetLayout = VK_NULL_HANDLE;
        }
        
        destroyAssets();

        if( m_shaderModuleVert != VK_NULL_HANDLE )
        {
            vkDestroyShaderModule( m_logicalDevice, m_shaderModuleVert, nullptr );
            m_shaderModuleVert = VK_NULL_HANDLE;
        }
        
        if( m_shaderModuleFrag != VK_NULL_HANDLE )
        {
            vkDestroyShaderModule( m_logicalDevice, m_shaderModuleFrag, nullptr );
            m_shaderModuleFrag = VK_NULL_HANDLE;
        }

        vkDestroyDevice( m_logicalDevice, nullptr );
        m_logicalDevice = VK_NULL_HANDLE;
    }
    
    if( m_vulkanInstance != VK_NULL_HANDLE )
    {
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
        
        if( m_graphicsPipeline != VK_NULL_HANDLE )
        {
            vkDestroyPipeline( m_logicalDevice, m_graphicsPipeline, nullptr );
            m_graphicsPipeline = VK_NULL_HANDLE;
        }
        
        if( m_renderPass != VK_NULL_HANDLE )
        {
            vkDestroyRenderPass( m_logicalDevice, m_renderPass, nullptr );
            m_renderPass = VK_NULL_HANDLE;
        }
        
        if( m_pipelineLayout != VK_NULL_HANDLE )
        {
            vkDestroyPipelineLayout( m_logicalDevice, m_pipelineLayout, nullptr );
            m_pipelineLayout = VK_NULL_HANDLE;
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
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = CSettings::Instance().getGameName().c_str();
    appInfo.applicationVersion = CSettings::Instance().getGameVersion();
    appInfo.pEngineName = CSettings::Instance().getEngineName().c_str();
    appInfo.engineVersion = CSettings::Instance().getEngineVersion();
    appInfo.apiVersion = VK_MAKE_VERSION(CSettings::Instance().getMajorVersion(), CSettings::Instance().getMinorVersion(), 0);

    VkInstanceCreateInfo instCreateInfo = {};
    instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pApplicationInfo = &appInfo;
    instCreateInfo.enabledLayerCount = validationNameVec.size();
    instCreateInfo.ppEnabledLayerNames = validationNameVec.data();
    instCreateInfo.enabledExtensionCount = instanceExtensionNameVec.size();
    instCreateInfo.ppEnabledExtensionNames = instanceExtensionNameVec.data();

    // Try to create the instance
    if( (m_lastResult = vkCreateInstance( &instCreateInfo, nullptr, &m_vulkanInstance )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create instance! %s") % getError() ) );
    
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

        if( (m_lastResult = CreateDebugReportCallback( m_vulkanInstance, &dbgCreateInfo, nullptr, &vkDebugReportCallbackEXT )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create debug report callback! %s") % getError() ) );
    }
}


/***************************************************************************
*   DESC:  Select a physical device (GPU)
****************************************************************************/
void CDeviceVulkan::selectPhysicalDevice()
{
    // Select a graphics device
    uint32_t gpuCount(0);
    if( (m_lastResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, nullptr )) || (gpuCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate physical device count! %s") % getError() ) );

    std::vector<VkPhysicalDevice> physicalDeviceHandleVec(gpuCount);

    if( (m_lastResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, physicalDeviceHandleVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate physical device info! %s") % getError() ) );

    // Select the discrete GPU if one is available that supports the graphics bit
    m_graphicsQueueFamilyIndex = UINT32_MAX;
    for( auto iter : physicalDeviceHandleVec )
    {
        auto props = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(iter, &props);

        // Print out the GPU if validation layers is enabled
        if( CSettings::Instance().isValidationLayers() )
            NGenFunc::PostDebugMsg( "GPU: " + std::string(props.deviceName));
        
        m_graphicsQueueFamilyIndex = findQueueFamilyIndex( iter, VK_QUEUE_GRAPHICS_BIT );

        // If we found a discrete GPU, our work here is done
        if( props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        {
            if( m_graphicsQueueFamilyIndex != UINT32_MAX )
            {
                m_physicalDevice = iter;
                break;
            }
        }

        if( m_graphicsQueueFamilyIndex != UINT32_MAX )
            m_physicalDevice = iter;
    }

    if( (m_physicalDevice == nullptr) || (m_graphicsQueueFamilyIndex == UINT32_MAX) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Suitable GPU could not be found!" );
}


/***************************************************************************
*   DESC:  Create the logical device
****************************************************************************/
void CDeviceVulkan::createLogicalDevice( const std::vector<const char*> & validationNameVec )
{
    // Make sure we have a swap chain
    if( !isDeviceExtension( m_physicalDevice, VK_KHR_SWAPCHAIN_EXTENSION_NAME ) )
        throw NExcept::CCriticalException( "Vulkan Error!", "No swap chain support!" );
    
    std::vector<const char*> physicalDeviceExtensionNameVec = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo deviceQueueInfo = {};
    deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    deviceQueueInfo.queueCount = 1;
    deviceQueueInfo.pQueuePriorities = &queuePriority;
    
    // Get all the features supported on this device
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures( m_physicalDevice, &physicalDeviceFeatures );
    
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &deviceQueueInfo;
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
    if( (m_lastResult = vkCreateDevice( m_physicalDevice, &createInfo, nullptr, &m_logicalDevice )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create logical device! %s") % getError() ) );
    
    // Get a handle to the graphics and present queue family - Could be different but most likely in the same queue family
    // Get a handle to the graphics queue
    vkGetDeviceQueue( m_logicalDevice, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue );
    if( m_graphicsQueue == nullptr )
        throw NExcept::CCriticalException( "Vulkan Error!", "Could not get handle to graphics queue family!" );
    
    // Get a handle to the present queue
    m_presentQueueFamilyIndex = findQueueFamilyIndex( m_physicalDevice );
    vkGetDeviceQueue( m_logicalDevice, m_presentQueueFamilyIndex, 0, &m_presentQueue );
    if( m_presentQueue == nullptr )
        throw NExcept::CCriticalException( "Vulkan Error!", "Could not get handle to present queue family!" );
}


/***************************************************************************
*   DESC:  Load the shaders
****************************************************************************/
void CDeviceVulkan::loadShaders( const std::string & vertShader, const std::string & fragShader )
{
    // Load shaders  **** temporary code ****
    std::vector<char> shaderVert = NGenFunc::FileToVec(vertShader);
    std::vector<char> shaderFrag = NGenFunc::FileToVec(fragShader);
    
    VkShaderModuleCreateInfo shaderInfo = {};
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = shaderVert.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t*>(shaderVert.data());
    
    if( (m_lastResult = vkCreateShaderModule( m_logicalDevice, &shaderInfo, nullptr, &m_shaderModuleVert )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create vertex shader! %s") % getError() ) );
    
    shaderInfo.codeSize = shaderFrag.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t*>(shaderFrag.data());
    
    if( (m_lastResult = vkCreateShaderModule( m_logicalDevice, &shaderInfo, nullptr, &m_shaderModuleFrag )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create fragment shader! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Setup the swap chain to be created
****************************************************************************/
void CDeviceVulkan::setupSwapChain()
{
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
    if( (m_lastResult = GetPhysicalDeviceSurfaceCapabilities( m_physicalDevice, m_vulkanSurface, &surfCapabilities )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface capabilities! %s") % getError() ) );
    
    // Get the best surface format
    VkSurfaceFormatKHR surfaceFormat;
    uint32_t surfaceFormatCount;
    if( (m_lastResult = GetPhysicalDeviceSurfaceFormats( m_physicalDevice, m_vulkanSurface, &surfaceFormatCount, nullptr)) || (surfaceFormatCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface format count! %s") % getError() ) );
    
    std::vector<VkSurfaceFormatKHR> surfaceFormatVec(surfaceFormatCount);
    
    if( (m_lastResult = GetPhysicalDeviceSurfaceFormats( m_physicalDevice, m_vulkanSurface, &surfaceFormatCount, surfaceFormatVec.data())) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface formats! %s") % getError() ) );
    
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
    
    // Get the best presentation mode
    VkPresentModeKHR surfacePresMode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t surfacePresModeCount;
    if( (m_lastResult = GetPhysicalDeviceSurfacePresentModes( m_physicalDevice, m_vulkanSurface, &surfacePresModeCount, nullptr)) || (surfacePresModeCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface presentation mode count! %s") % getError() ) );
    
    std::vector<VkPresentModeKHR> surfacePresModeVec(surfacePresModeCount);
    
    if( (m_lastResult = GetPhysicalDeviceSurfacePresentModes( m_physicalDevice, m_vulkanSurface, &surfacePresModeCount, surfacePresModeVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to get physical device surface presentation modes! %s") % getError() ) );
    
    for( const auto & presentMode : surfacePresModeVec )
    {
        if( presentMode == VK_PRESENT_MODE_MAILBOX_KHR )
        {
            surfacePresMode = presentMode;
            break;
        }
        else if( presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR )
            surfacePresMode = presentMode;
    }

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
    m_swapchainInfo.queueFamilyIndexCount = 0;
    m_swapchainInfo.preTransform = preTransform;
    m_swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_swapchainInfo.presentMode = surfacePresMode;
    m_swapchainInfo.clipped = true;
    
    // Determine the number of VkImage's to use in the swap chain.
    uint32_t minImageCount = surfCapabilities.minImageCount;
    
    if( CSettings::Instance().getTripleBuffering() )
        ++minImageCount;
    
    // Application must settle for fewer images than desired
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
            CSettings::Instance().setSize(
                CSize<float>(
                    surfCapabilities.currentExtent.width,
                    surfCapabilities.currentExtent.height) );
            
            CSettings::Instance().calcRatio();
        }
        
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
    }
    
    m_swapchainInfo.imageExtent = swapchainExtent;
    
    // In the event the graphics and present queue family doesn't match
    std::vector<uint32_t> queueFamilyIndiceVec = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};
    if( m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex )
    {
        m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        m_swapchainInfo.queueFamilyIndexCount = queueFamilyIndiceVec.size();
        m_swapchainInfo.pQueueFamilyIndices = queueFamilyIndiceVec.data();
    }
}


/***************************************************************************
*   DESC:  Create the swap chain
****************************************************************************/
void CDeviceVulkan::createSwapChain()
{
    PFN_vkCreateSwapchainKHR CreateSwapchain = nullptr;
    if( !(CreateSwapchain = (PFN_vkCreateSwapchainKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkCreateSwapchainKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkCreateSwapchainKHR!" );
    
    // Create the swap chain
    if( (m_lastResult = CreateSwapchain( m_logicalDevice, &m_swapchainInfo, nullptr, &m_swapchain)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create swap chain! %s") % getError() ) );
    
    // get the swap chain images. NOTE: Swap chain images are destroyed when the swap chain is destroyed.
    uint32_t swapChainImageCount;
    if( (m_lastResult = vkGetSwapchainImagesKHR( m_logicalDevice, m_swapchain, &swapChainImageCount, nullptr)) || (swapChainImageCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate swap chain image count! %s") % getError() ) );
    
    std::vector<VkImage> swapChainImage(swapChainImageCount);
    
    if( (m_lastResult = vkGetSwapchainImagesKHR( m_logicalDevice, m_swapchain, &swapChainImageCount, swapChainImage.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not get swap chain images! %s") % getError() ) );
    
    // Print out info if the swap images don't match
    if( m_swapchainInfo.minImageCount != swapChainImageCount )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Swap chain image don't match! (%d / %d)") % m_swapchainInfo.minImageCount % swapChainImageCount ));
    
    m_swapChainImageViewVec.reserve( swapChainImageCount );
    
    for( uint32_t i = 0; i < swapChainImageCount; ++i )
        m_swapChainImageViewVec.push_back( createImageView( swapChainImage[i], m_swapchainInfo.imageFormat, 1, VK_IMAGE_ASPECT_COLOR_BIT ) );
}


/***************************************************************************
*   DESC:  Create the descriptor set layout
****************************************************************************/
void CDeviceVulkan::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();
    
    if( (m_lastResult = vkCreateDescriptorSetLayout( m_logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create descriptor set layout! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the render pass
****************************************************************************/
void CDeviceVulkan::createRenderPass()
{
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
    if( CSettings::Instance().getEnableDepthBuffer() || CSettings::Instance().getEnableStencilBuffer() )
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
    
    if( (m_lastResult = vkCreateRenderPass( m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create render pass! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the graphics pipeline
****************************************************************************/
void CDeviceVulkan::createGraphicsPipeline()
{
    // Create the pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;

    if( (m_lastResult = vkCreatePipelineLayout( m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create pipeline layout! %s") % getError() ) );
    
    // Create the graphics pipeline
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = m_shaderModuleVert;
    vertShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = m_shaderModuleFrag;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    
    // Bind the vertex buffer
    auto bindingDescription = CVertex::getBindingDescription();
    auto attributeDescriptions = CVertex::getAttributeDescriptions();
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = m_swapchainInfo.imageExtent.width;
    viewport.height = m_swapchainInfo.imageExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
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
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // Counter clockwise for righthanded rule
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = CSettings::Instance().getEnableDepthBuffer();
    depthStencil.depthWriteEnable = CSettings::Instance().getEnableDepthBuffer();
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = CSettings::Instance().getEnableStencilBuffer();
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
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

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    // Add the depthStencil if depth or stencil buffer is needed
    if( CSettings::Instance().getEnableDepthBuffer() || CSettings::Instance().getEnableStencilBuffer() )
        pipelineInfo.pDepthStencilState = &depthStencil;

    if( (m_lastResult = vkCreateGraphicsPipelines( m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create graphics pipeline! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the primary command pool
****************************************************************************/
void CDeviceVulkan::createPrimaryCommandPool()
{
    m_primaryCmdPool = createCommandPool();
}


/***************************************************************************
*   DESC:  Create depth resources
****************************************************************************/
void CDeviceVulkan::createDepthResources()
{
    if( CSettings::Instance().getEnableDepthBuffer() || CSettings::Instance().getEnableStencilBuffer() )
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
    
    m_depthImageView = createImageView( m_depthImage, depthFormat, 1, VK_IMAGE_ASPECT_DEPTH_BIT );

    transitionImageLayout( m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1 );
}
}


/***************************************************************************
*   DESC:  Create the frame buffer
****************************************************************************/
void CDeviceVulkan::createFrameBuffer()
{
    m_framebufferVec.resize( m_swapChainImageViewVec.size() );
    
    for( size_t i = 0; i < m_swapChainImageViewVec.size(); ++i )
    {
        std::vector<VkImageView> attachmentsAry = { m_swapChainImageViewVec[i] };

        // Add the depth image if depth or stencil buffer is needed
        if( CSettings::Instance().getEnableDepthBuffer() || CSettings::Instance().getEnableStencilBuffer() )
            attachmentsAry.push_back( m_depthImageView );

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = attachmentsAry.size();
        framebufferInfo.pAttachments = attachmentsAry.data();
        framebufferInfo.width = m_swapchainInfo.imageExtent.width;
        framebufferInfo.height = m_swapchainInfo.imageExtent.height;
        framebufferInfo.layers = 1;

        if( (m_lastResult = vkCreateFramebuffer( m_logicalDevice, &framebufferInfo, nullptr, &m_framebufferVec[i] )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create frame buffer! %s") % getError() ) );
    }
}


/***************************************************************************
*   DESC:  Create the uniform buffer
****************************************************************************/
void CDeviceVulkan::createUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBufVec.resize(m_framebufferVec.size());
    m_uniformBufMemVec.resize(m_framebufferVec.size());

    for( size_t i = 0; i < m_framebufferVec.size(); ++i )
        createBuffer( 
            bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_uniformBufVec[i],
            m_uniformBufMemVec[i]);
}


/***************************************************************************
*   DESC:  Create the Semaphores and fences
****************************************************************************/
void CDeviceVulkan::createSyncObjects()
{    
    m_imageAvailableSemaphoreVec.resize( m_maxConcurrentFrames );
    m_renderFinishedSemaphoreVec.resize( m_maxConcurrentFrames );
    m_frameFenceVec.resize( m_maxConcurrentFrames );
            
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for( int i = 0; i < m_maxConcurrentFrames; ++i )
    {
        if( (m_lastResult = vkCreateSemaphore( m_logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphoreVec[i] )) ||
            (m_lastResult = vkCreateSemaphore( m_logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphoreVec[i] )) ||
            (m_lastResult = vkCreateFence( m_logicalDevice, &fenceInfo, nullptr, &m_frameFenceVec[i] )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create synchronization objects! %s") % getError() ) );
    }
}


/***************************************************************************
*   DESC:  Create the command buffers
****************************************************************************/
void CDeviceVulkan::createPrimaryCommandBuffers()
{
    m_primaryCmdBufVec.resize( m_framebufferVec.size() );
    
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = m_primaryCmdPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = (uint32_t) m_primaryCmdBufVec.size();
    
    if( (m_lastResult = vkAllocateCommandBuffers( m_logicalDevice, &commandBufferAllocateInfo, m_primaryCmdBufVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the command buffers
****************************************************************************/
std::vector<VkCommandBuffer> CDeviceVulkan::createSecondaryCommandBuffers( VkCommandPool cmdPool )
{
    std::vector<VkCommandBuffer>cmdBufVec( m_framebufferVec.size() );
    
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = cmdPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandBufferAllocateInfo.commandBufferCount = (uint32_t) cmdBufVec.size();
    
    if( (m_lastResult = vkAllocateCommandBuffers( m_logicalDevice, &commandBufferAllocateInfo, cmdBufVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError() ) );
    
    return cmdBufVec;
}


/***************************************************************************
*   DESC:  Update the uniform buffer
****************************************************************************/
void CDeviceVulkan::updateUniformBuffer( uint32_t unfBufIndex )
{
    UniformBufferObject ubo;
    
    ubo.proj.orthographicRH(
        CSettings::Instance().getDefaultSize().w,
        CSettings::Instance().getDefaultSize().h,
        CSettings::Instance().getMinZdist(),
        CSettings::Instance().getMaxZdist() );
    
    ubo.model.setScale( 1344.f, 756.f );
    
    ubo.view.translate( CPoint<float>(0.f, 0.f, -10.0f) );
    
    void* data;
    vkMapMemory( m_logicalDevice, m_uniformBufMemVec[unfBufIndex], 0, sizeof(ubo), 0, &data );
    std::memcpy( data, &ubo, sizeof(ubo));
    vkUnmapMemory( m_logicalDevice, m_uniformBufMemVec[unfBufIndex] );
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
    
    // Create the graphics pipeline
    createGraphicsPipeline();
    
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
    vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, &memProperties );

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
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
    uint32_t deviceExtensionCount = 0;
    if( (m_lastResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, nullptr )) || (deviceExtensionCount == 0 ) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate device extension count! %s") % getError() ) );
    
    std::vector<VkExtensionProperties> availableExtensions(deviceExtensionCount);
    
    if( (m_lastResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, availableExtensions.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate device extension info! %s") % getError() ) );
    
    for( auto iter : availableExtensions )
        if( std::strcmp( iter.extensionName, extenName ) == 0 )
            return true;
 
    return false;
}


/***************************************************************************
*   DESC:  Find the queue family index
****************************************************************************/
uint32_t CDeviceVulkan::findQueueFamilyIndex( VkPhysicalDevice physicalDevice, uint32_t queueMask )
{
    // Get the queue family information - Check that graphics bit is available
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    if( queueFamilyCount > 0 )
    {
        std::vector<VkQueueFamilyProperties> queueFamiliesVec(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamiliesVec.data());

        for( uint32_t i = 0; i < queueFamiliesVec.size(); ++i )
            if( (queueFamiliesVec[i].queueCount > 0) && (queueFamiliesVec[i].queueFlags & queueMask) )
                return i;
    }
    
    return UINT32_MAX;
}

uint32_t CDeviceVulkan::findQueueFamilyIndex( VkPhysicalDevice physicalDevice )
{
    // Get the queue family information - Check that graphics bit is available
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    if( queueFamilyCount > 0 )
    {
        std::vector<VkQueueFamilyProperties> queueFamiliesVec(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamiliesVec.data());

        for( uint32_t i = 0; i < queueFamiliesVec.size(); ++i )
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_vulkanSurface, &presentSupport);
            
            if( (queueFamiliesVec[i].queueCount > 0) && presentSupport )
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
        vkGetPhysicalDeviceFormatProperties( m_physicalDevice, format, &props );
        
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
    if( CSettings::Instance().getEnableStencilBuffer() )
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
    
    if( (m_lastResult = vkCreateImage( m_logicalDevice, &imageInfo, nullptr, &image )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create image! %s") % getError() ) );

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements( m_logicalDevice, image, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
    
    if( (m_lastResult = vkAllocateMemory( m_logicalDevice, &allocInfo, nullptr, &imageMemory )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate image memory! %s") % getError() ) );

    vkBindImageMemory( m_logicalDevice, image, imageMemory, 0 );
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
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if( (m_lastResult = vkCreateBuffer( m_logicalDevice, &bufferInfo, nullptr, &buffer )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create buffer! %s") % getError() ) );

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( m_logicalDevice, buffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
    
    if( (m_lastResult = vkAllocateMemory( m_logicalDevice, &allocInfo, nullptr, &bufferMemory )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate buffer memory! %s") % getError() ) );
    
    vkBindBufferMemory( m_logicalDevice, buffer, bufferMemory, 0);
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
    allocInfo.commandPool = m_primaryCmdPool;
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

    vkQueueSubmit( m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( m_graphicsQueue );

    vkFreeCommandBuffers( m_logicalDevice, m_primaryCmdPool, 1, &commandBuffer);
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
VkCommandPool CDeviceVulkan::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    
    VkCommandPool commandPool;
    if( (m_lastResult = vkCreateCommandPool( m_logicalDevice, &poolInfo, nullptr, &commandPool )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create command pool! %s") % getError() ) );
    
    return commandPool;
}


/***************************************************************************
*   DESC:  Create texture
****************************************************************************/
void CDeviceVulkan::createTexture( CTexture & texture, const std::string & filePath, bool mipMap )
{
    int channels(0);
    unsigned char * pixels = SOIL_load_image( 
        filePath.c_str(),
        &texture.m_size.w,
        &texture.m_size.h,
        &channels,
        SOIL_LOAD_RGBA );
    
    if( pixels == nullptr )
        throw NExcept::CCriticalException( "SOIL Error!", "Error loading image!");
    
    VkDeviceSize imageSize = texture.m_size.w * texture.m_size.h * SOIL_LOAD_RGBA;
    
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
    
    if( mipMap )
    {
        imageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        texture.m_mipLevels = std::floor(std::log2(std::max(texture.m_size.w, texture.m_size.h))) + 1;
    }
    
    createImage(
        texture.m_size.w,
        texture.m_size.h,
        texture.m_mipLevels,
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_IMAGE_TILING_OPTIMAL,
        imageUsageFlags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        texture.m_textureImage,
        texture.m_textureImageMemory );
    
    transitionImageLayout( texture.m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture.m_mipLevels );
    copyBufferToImage( stagingBuffer, texture.m_textureImage, static_cast<uint32_t>(texture.m_size.w), static_cast<uint32_t>(texture.m_size.h) );
    
    if( mipMap )
        generateMipmaps( texture.m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, texture.m_size.w, texture.m_size.h, texture.m_mipLevels );
    else
        transitionImageLayout( texture.m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, texture.m_mipLevels );
    
    vkDestroyBuffer( m_logicalDevice, stagingBuffer, nullptr );
    vkFreeMemory( m_logicalDevice, stagingBufferMemory, nullptr );
    
    // create the image view
    texture.m_textureImageView = createImageView( texture.m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, texture.m_mipLevels, VK_IMAGE_ASPECT_COLOR_BIT );
    
    // Create the texture sampler
    texture.m_textureSampler = createTextureSampler( texture.m_mipLevels );
}


/***************************************************************************
*   DESC:  Generate Mipmaps
****************************************************************************/
void CDeviceVulkan::generateMipmaps( VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels )
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties( m_physicalDevice, imageFormat, &formatProperties );

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
VkSampler CDeviceVulkan::createTextureSampler( uint32_t mipLevels )
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = ((mipLevels > 1) ? mipLevels : 0.f);
    
    VkSampler textureSampler;
            
    if( (m_lastResult = vkCreateSampler( m_logicalDevice, &samplerInfo, nullptr, &textureSampler )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create texture sampler! %s") % getError() ) );
    
    return textureSampler;
}


/***************************************************************************
*   DESC:  Create the image view
****************************************************************************/
VkImageView CDeviceVulkan::createImageView( VkImage image, VkFormat format, uint32_t mipLevels, VkImageAspectFlags aspectFlags )
{
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
    if( (m_lastResult = vkCreateImageView( m_logicalDevice, &viewInfo, nullptr, &imageView )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create texture image view! %s") % getError() ) );

    return imageView;
}


/***************************************************************************
*   DESC:  Create descriptor pool
****************************************************************************/
VkDescriptorPool CDeviceVulkan::createDescriptorPool( size_t setCount )
{
    const int DESC_TYPE_COUNT(2);
    std::vector<VkDescriptorPoolSize> descriptorPoolVec( DESC_TYPE_COUNT * setCount );
    
    for( size_t i = 0; i < descriptorPoolVec.size(); ++i )
    {
        descriptorPoolVec[i].descriptorCount = m_framebufferVec.size();
        
        if( (i & 0x1) == 0 )
            descriptorPoolVec[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        else
            descriptorPoolVec[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    }

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = descriptorPoolVec.size();
    poolInfo.pPoolSizes = descriptorPoolVec.data();
    poolInfo.maxSets = descriptorPoolVec.size();
    
    VkDescriptorPool descriptorPool;
    
    if( (m_lastResult = vkCreateDescriptorPool( m_logicalDevice, &poolInfo, nullptr, &descriptorPool )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError() ) );
    
    return descriptorPool;
}


/***************************************************************************
*   DESC:  Create descriptor sets
****************************************************************************/
void CDeviceVulkan::createDescriptorSet( CTexture & texture, VkDescriptorPool descriptorPool )
{
    std::vector<VkDescriptorSetLayout> layouts( m_framebufferVec.size(), m_descriptorSetLayout );
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = m_framebufferVec.size();
    allocInfo.pSetLayouts = layouts.data();

    texture.m_descriptorSetVec.resize( m_framebufferVec.size() );
    
    if( (m_lastResult = vkAllocateDescriptorSets( m_logicalDevice, &allocInfo, texture.m_descriptorSetVec.data() )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate descriptor sets! %s") % getError() ) );
    
    for( size_t i = 0; i < m_framebufferVec.size(); ++i )
    {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = m_uniformBufVec[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);
        
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.m_textureImageView;
        imageInfo.sampler = texture.m_textureSampler;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = texture.m_descriptorSetVec[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = texture.m_descriptorSetVec[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets( m_logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr );
    }
}


/***************************************************************************
*   DESC:  Get Vulkan error
****************************************************************************/
const char * CDeviceVulkan::getError()
{
    auto iter = m_vulkanErrorMap.find( m_lastResult );
    if( iter != m_vulkanErrorMap.end() )
        return iter->second;

    return "Vulkan Unknown Error";
}
