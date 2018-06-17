
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Class used for window and openGL management
************************************************************************/

// Physical component dependency
#include <system/device.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <common/size.h>

// Standard lib dependencies
#include <cstring>
#include <fstream>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL_vulkan.h>

/************************************************************************
*    DESC:  Debug callback
************************************************************************/
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkFlags msgFlags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t srcObject,
    size_t location,
    int32_t msgCode,
    const char *pLayerPrefix,
    const char *pMsg,
    void *pUserData) {

    // std::string info = "validation layer: " + msg;
    NGenFunc::PostDebugMsg( pMsg );

    return VK_FALSE;
}

VkDebugReportCallbackEXT msg_callback = nullptr;
PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = nullptr;


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CDevice::CDevice() :
    m_pWindow(nullptr),
    m_vulkanInstance(nullptr),
    m_vulkanSurface(nullptr),
    m_physicalDevice(nullptr),
    m_logicalDevice(nullptr),
    m_graphicsQueueFamilyIndex(0),
    m_presentQueueFamilyIndex(0),
    m_graphicsQueue(nullptr),
    m_presentQueue(nullptr),
    m_swapchain(nullptr),
    m_pipelineLayout(nullptr),
    m_renderPass(nullptr),
    m_graphicsPipeline(nullptr),
    m_commandPool(nullptr),
    m_currentFrame(0),
    m_maxConcurrentFrames(0),
    m_lastResult(VK_SUCCESS),
    vkDestroySwapchainKHR(nullptr),
    vkGetSwapchainImagesKHR(nullptr)
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
CDevice::~CDevice()
{

}


/***************************************************************************
*   DESC:  Destroy the window and Vulkan instance
****************************************************************************/
void CDevice::destroy()
{
    if( m_logicalDevice != nullptr )
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
        
        if( m_commandPool != nullptr )
        {
            vkDestroyCommandPool( m_logicalDevice, m_commandPool, nullptr );
            m_commandPool = nullptr;
        }

        if( m_shaderModuleVert != nullptr )
        {
            vkDestroyShaderModule( m_logicalDevice, m_shaderModuleVert, nullptr );
            m_shaderModuleVert = nullptr;
        }
        
        if( m_shaderModuleFrag != nullptr )
        {
            vkDestroyShaderModule( m_logicalDevice, m_shaderModuleFrag, nullptr );
            m_shaderModuleFrag = nullptr;
        }

        vkDestroyDevice( m_logicalDevice, nullptr );
        m_logicalDevice = nullptr;
    }
    
    if( m_vulkanInstance != nullptr )
    {
        if( CSettings::Instance().getDebugMode() && (DestroyDebugReportCallback != nullptr) )
            DestroyDebugReportCallback( m_vulkanInstance, msg_callback, nullptr );

        vkDestroyInstance( m_vulkanInstance, nullptr );
        m_vulkanInstance = nullptr;
    }

    if( m_pWindow != nullptr )
    {
        SDL_DestroyWindow( m_pWindow );
        m_pWindow = nullptr;
    }
}


/***************************************************************************
*   DESC:  Destroy the swap chain
****************************************************************************/
void CDevice::destroySwapChain()
{
    if( m_logicalDevice != nullptr )
    {
        if( !m_framebufferVec.empty() )
        {
            for( auto framebuffer : m_framebufferVec )
                vkDestroyFramebuffer( m_logicalDevice, framebuffer, nullptr );
            
            m_framebufferVec.clear();
        }
        
        if( m_graphicsPipeline != nullptr )
        {
            vkDestroyPipeline( m_logicalDevice, m_graphicsPipeline, nullptr );
            m_graphicsPipeline = nullptr;
        }
        
        if( m_renderPass != nullptr )
        {
            vkDestroyRenderPass( m_logicalDevice, m_renderPass, nullptr );
            m_renderPass = nullptr;
        }
        
        if( m_pipelineLayout != nullptr )
        {
            vkDestroyPipelineLayout( m_logicalDevice, m_pipelineLayout, nullptr );
            m_pipelineLayout = nullptr;
        }
        
        if( m_swapchain != nullptr )
        {
            for( auto imageView : m_swapChainImageViewVec )
                vkDestroyImageView( m_logicalDevice, imageView, nullptr );

            m_swapChainImageViewVec.clear();

            vkDestroySwapchainKHR( m_logicalDevice, m_swapchain, nullptr );
            m_swapchain = nullptr;
        }
    }
}


/***************************************************************************
*   DESC:  Create the window and Vulkan instance
****************************************************************************/
void CDevice::create()
{
    // Initialize SDL - The File I/O and Threading subsystems are initialized by default.
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER ) < 0 )
        throw NExcept::CCriticalException("SDL could not initialize!", SDL_GetError() );

    // All file I/O is handled by SDL and SDL_Init must be called before doing any I/O.
    CSettings::Instance().loadXML();

    // Use OpenGL 3.3 core
    /*SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, CSettings::Instance().getMajorVersion() );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, CSettings::Instance().getMinorVersion() );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, CSettings::Instance().getProfile() );

    // Do we add stencil buffer
    if( CSettings::Instance().getCreateStencilBuffer() )
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, CSettings::Instance().getStencilBufferBitSize() );*/
    
    // Set the maximum concurrent frames that can be rendered
    m_maxConcurrentFrames = CSettings::Instance().getMaxConcurrentFrameRender();

    // Get the render size of the window
    const CSize<int> size( CSettings::Instance().getSize() );

    // Create window
    m_pWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.getW(), size.getH(), SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );
    if( m_pWindow == nullptr )
        throw NExcept::CCriticalException("Game window could not be created!", SDL_GetError() );

    
    ///////////////////////////////////////////////////
    // Prepare to create the Vulkan instance
    //////////////////////////////////////////////////
    
    uint32_t instanceExtensionCount(0);
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, nullptr) || (instanceExtensionCount == 0) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension count!", SDL_GetError() );

    std::vector<const char*> instanceExtensionNameVec(instanceExtensionCount);
    std::vector<const char*> validationNameVec;
        
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, instanceExtensionNameVec.data()) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension names!", SDL_GetError() );

    // If we are in debug mode, add validation and debug reporting extension
    if( CSettings::Instance().getDebugMode() )
    {
        validationNameVec.push_back("VK_LAYER_LUNARG_standard_validation");
        instanceExtensionNameVec.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
        
        // Print out extension list for debug mode
        for( auto iter : instanceExtensionNameVec )
            NGenFunc::PostDebugMsg( "Instance Extension: " + std::string(iter));
    }

    // Create the vulkan instance
    createVulkanInstance( validationNameVec, instanceExtensionNameVec );
    
    // Select a physical device (GPU)
    selectPhysicalDevice();
    
    // Create the logical device
    createLogicalDevice( validationNameVec );
    
    tmpShaderSetup();
    
    // Setup the swap chain to be created
    setupSwapChain();
    
    // Create the swap chain
    createSwapChain();
    
    // Create the render pass
    createRenderPass();
    
    // Create the graphics pipeline
    createGraphicsPipeline();
    
    // Create the frame buffer
    createFrameBuffer();
    
    // Create the command pool
    createCommandPool();
    
    // Create the command buffers
    createCommandBuffers();
    
    // Create the Semaphores and fences
    createSyncObjects();
    
    
    // Create context
    /*m_context = SDL_GL_CreateContext( m_pWindow );
    if( m_context == nullptr )
        throw NExcept::CCriticalException("OpenGL context could not be created!", SDL_GetError() );*/

    #if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
    // Initialize GLEW
    /*glewExperimental = GL_TRUE;
    uint32_t glewError = glewInit();
    if( glewError != GLEW_OK )
        throw NExcept::CCriticalException("Error initializing GLEW!",
            boost::str( boost::format("Error initializing GLEW (%s).\n\n%s\nLine: %s")
                % glewGetErrorString( glewError ) % __FUNCTION__ % __LINE__ ));

    // Enable/disable v-sync
    enableVSync( CSettings::Instance().getVSync() );*/

    #else

    // Enable/disable v-sync
    //CSettings::Instance().setVSync( true );
    //enableVSync( true );

    #endif

    // Set the full screen
    if( CSettings::Instance().getFullScreen() )
        setFullScreen( CSettings::Instance().getFullScreen() );

    // Depth testing is off by default. Enable it?
    //if( CSettings::Instance().getEnableDepthBuffer() )
    //    glEnable(GL_DEPTH_TEST);

    // Init current gamepads plugged in at startup
    initStartupGamepads();
}


/***************************************************************************
*   DESC:  Create the vulkan instance
****************************************************************************/
void CDevice::createVulkanInstance(
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
    instCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationNameVec.size());
    instCreateInfo.ppEnabledLayerNames = validationNameVec.data();
    instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNameVec.size());
    instCreateInfo.ppEnabledExtensionNames = instanceExtensionNameVec.data();

    // Try to create the instance
    if( (m_lastResult = vkCreateInstance(&instCreateInfo, nullptr, &m_vulkanInstance)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create instance! %s") % getError() ) );
    
    // Get a function pointer to the vulkan vkDestroySwapchainKHR
    if( !(vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkDestroySwapchainKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkDestroySwapchainKHR!" );
    
    // Get a function pointer to the vulkan vkGetSwapchainImagesKHR
    if( !(vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkGetSwapchainImagesKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkGetSwapchainImagesKHR!" );

    
    ///////////////////////////////////////////////////
    // Setup the debug call back
    //////////////////////////////////////////////////
    
    if( CSettings::Instance().getDebugMode() )
    {
        PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = nullptr;
        
        // Get the function instances of the debug report callback messages
        if( !(CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr( m_vulkanInstance, "vkCreateDebugReportCallbackEXT")) )
            throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkCreateDebugReportCallbackEXT!" );

        if( !(DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr( m_vulkanInstance, "vkDestroyDebugReportCallbackEXT")) )
            throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkDestroyDebugReportCallbackEXT!" );

        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        dbgCreateInfo.pfnCallback = debugCallback;
        dbgCreateInfo.pUserData = nullptr;

        if( (m_lastResult = CreateDebugReportCallback(m_vulkanInstance, &dbgCreateInfo, nullptr, &msg_callback)) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create debug report callback! %s") % getError() ) );
    }
    
    // Create the Vulkan surface
    if( !SDL_Vulkan_CreateSurface( m_pWindow, m_vulkanInstance, &m_vulkanSurface ) )
        throw NExcept::CCriticalException("Could not create Vulkan surface!", SDL_GetError() );
}


/***************************************************************************
*   DESC:  Select a physical device (GPU)
****************************************************************************/
void CDevice::selectPhysicalDevice()
{
    // Select a graphics device
    uint32_t gpuCount(0);
    if( (m_lastResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, nullptr)) || (gpuCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate physical device count! %s") % getError() ) );

    std::vector<VkPhysicalDevice> physicalDeviceHandleVec(gpuCount);

    if( (m_lastResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, physicalDeviceHandleVec.data())) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate physical device info! %s") % getError() ) );

    // Select the discrete GPU if one is available that supports the graphics bit
    m_graphicsQueueFamilyIndex = UINT32_MAX;
    for( auto iter : physicalDeviceHandleVec )
    {
        auto props = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(iter, &props);

        // Print out the GPU in debug mode
        if( CSettings::Instance().getDebugMode() )
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
void CDevice::createLogicalDevice( const std::vector<const char*> & validationNameVec )
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
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &physicalDeviceFeatures);
    
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &deviceQueueInfo;
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationNameVec.size());
    createInfo.ppEnabledLayerNames = validationNameVec.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDeviceExtensionNameVec.size());
    createInfo.ppEnabledExtensionNames = physicalDeviceExtensionNameVec.data();
    createInfo.pEnabledFeatures = &physicalDeviceFeatures;
    
    if( CSettings::Instance().getDebugMode() )
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationNameVec.size());
        createInfo.ppEnabledLayerNames = validationNameVec.data();
    }
    
    // Create the logical device
    if( (m_lastResult = vkCreateDevice( m_physicalDevice, &createInfo, nullptr, &m_logicalDevice)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create logical device! %s") % getError() ) );
    
    // Get a handle to the graphics and present queue family - Could be different but most likely in the same queue family
    // Get a handle to the graphics queue
    vkGetDeviceQueue( m_logicalDevice, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue );
    if( m_graphicsQueue == nullptr )
        throw NExcept::CCriticalException( "Vulkan Error!", "Could not get handle to graphics queue family!" );
    
    // Get a handle to the present queue
    m_presentQueueFamilyIndex = findQueueFamilyIndex( m_physicalDevice, m_vulkanSurface );
    vkGetDeviceQueue( m_logicalDevice, m_presentQueueFamilyIndex, 0, &m_presentQueue );
    if( m_presentQueue == nullptr )
        throw NExcept::CCriticalException( "Vulkan Error!", "Could not get handle to present queue family!" );
}


/***************************************************************************
*   DESC:  Setup the swap chain to be created
****************************************************************************/
void CDevice::setupSwapChain()
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
    if( (m_lastResult = GetPhysicalDeviceSurfaceCapabilities( m_physicalDevice, m_vulkanSurface, &m_surfCapabilities )) )
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
    
    if( (m_lastResult = GetPhysicalDeviceSurfacePresentModes( m_physicalDevice, m_vulkanSurface, &surfacePresModeCount, surfacePresModeVec.data())) )
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
    VkSurfaceTransformFlagBitsKHR preTransform = m_surfCapabilities.currentTransform;
    if (m_surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
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
}


/***************************************************************************
*   DESC:  Create the swap chain
****************************************************************************/
void CDevice::createSwapChain()
{
    PFN_vkCreateSwapchainKHR CreateSwapchain = nullptr;
    if( !(CreateSwapchain = (PFN_vkCreateSwapchainKHR)vkGetInstanceProcAddr( m_vulkanInstance, "vkCreateSwapchainKHR")) )
        throw NExcept::CCriticalException( "Vulkan Error!", "Unable to find PFN_vkCreateSwapchainKHR!" );
    
    // Determine the number of VkImage's to use in the swap chain.
    uint32_t minImageCount = m_surfCapabilities.minImageCount;
    
    if( CSettings::Instance().getTripleBuffering() )
        ++minImageCount;
    
    // Application must settle for fewer images than desired
    if ((m_surfCapabilities.maxImageCount > 0) && (minImageCount > m_surfCapabilities.maxImageCount))
        minImageCount = m_surfCapabilities.maxImageCount;
    
    // Set the extent of the render resolution
    VkExtent2D swapchainExtent;
    
    // width and height are either both -1, or both not -1.
    if (m_surfCapabilities.currentExtent.width == (uint32_t)-1)
    {
        // Get the render size of the window
        const CSize<int> size( CSettings::Instance().getSize() );
    
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = size.getW();
        swapchainExtent.height = size.getH();
    }
    else
    {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = m_surfCapabilities.currentExtent;
    }
    
    m_swapchainInfo.minImageCount = minImageCount;
    m_swapchainInfo.imageExtent = swapchainExtent;
    
    // In the event the graphics and present queue family doesn't match
    uint32_t queueFamilyIndices[] = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};
    if( m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex )
    {
        m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        m_swapchainInfo.queueFamilyIndexCount = 2;
        m_swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    
    // Create the swap chain
    if( (m_lastResult = CreateSwapchain( m_logicalDevice, &m_swapchainInfo, nullptr, &m_swapchain)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create swap chain! %s") % getError() ) );
    
    // get the swap chain images. NOTE: Swap chain images are destroyed when the swap chain is destroyed.
    uint32_t swapChainImageCount;
    if( (m_lastResult = vkGetSwapchainImagesKHR( m_logicalDevice, m_swapchain, &swapChainImageCount, nullptr)) || (swapChainImageCount == 0) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not enumerate swap chain image count! %s") % getError() ) );
    
    std::vector<VkImage> swapChainImage(swapChainImageCount);
    
    if( (m_lastResult = vkGetSwapchainImagesKHR( m_logicalDevice, m_swapchain, &swapChainImageCount, swapChainImage.data())) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not get swap chain images! %s") % getError() ) );
    
    // Print out info if the swap images don't match
    if( minImageCount != swapChainImageCount )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Swap chain image don't match! (%d / %d)") % minImageCount % swapChainImageCount ));
    
    m_swapChainImageViewVec.reserve(swapChainImage.size());
    
    for( size_t i = 0; i < swapChainImage.size(); ++i )
    {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = swapChainImage[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = m_swapchainInfo.imageFormat;
        imageViewCreateInfo.components = {};
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange = {};
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        
        VkImageView imageView(nullptr);
        if( (m_lastResult = vkCreateImageView( m_logicalDevice, &imageViewCreateInfo, nullptr, &imageView)) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create image view! %s") % getError() ) );
        
        m_swapChainImageViewVec.push_back(imageView);
    }
}


/***************************************************************************
*   DESC:  Temp shader setup
****************************************************************************/
void CDevice::tmpShaderSetup()
{
    // Load shaders  **** temporary code ****
    std::vector<char> shaderVert = readFile("data/shaders/vulkanTriangleVert.spv");
    std::vector<char> shaderFrag = readFile("data/shaders/vulkanTriangleFrag.spv");
    
    VkShaderModuleCreateInfo shaderInfo = {};
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = shaderVert.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t*>(shaderVert.data());
    
    if( (m_lastResult = vkCreateShaderModule( m_logicalDevice, &shaderInfo, nullptr, &m_shaderModuleVert)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create vertex shader! %s") % getError() ) );
    
    shaderInfo.codeSize = shaderFrag.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t*>(shaderFrag.data());
    
    if( (m_lastResult = vkCreateShaderModule( m_logicalDevice, &shaderInfo, nullptr, &m_shaderModuleFrag)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create fragment shader! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the render pass
****************************************************************************/
void CDevice::createRenderPass()
{
    // Create the pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if( (m_lastResult = vkCreatePipelineLayout( m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create pipeline layout! %s") % getError() ) );
    
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
    
    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
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
    
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    if( (m_lastResult = vkCreateRenderPass( m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create render pass! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the graphics pipeline
****************************************************************************/
void CDevice::createGraphicsPipeline()
{
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
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    
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
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    
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
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if( (m_lastResult = vkCreateGraphicsPipelines( m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Failed to create graphics pipeline! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the frame buffer
****************************************************************************/
void CDevice::createFrameBuffer()
{
    m_framebufferVec.resize( m_swapChainImageViewVec.size() );
    
    for( size_t i = 0; i < m_swapChainImageViewVec.size(); ++i )
    {
        std::vector<VkImageView> attachmentVec = { m_swapChainImageViewVec[i] };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = attachmentVec.size();
        framebufferInfo.pAttachments = attachmentVec.data();
        framebufferInfo.width = m_swapchainInfo.imageExtent.width;
        framebufferInfo.height = m_swapchainInfo.imageExtent.height;
        framebufferInfo.layers = 1;

        if( (m_lastResult = vkCreateFramebuffer( m_logicalDevice, &framebufferInfo, nullptr, &m_framebufferVec[i])) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create frame buffer! %s") % getError() ) );
    }
}


/***************************************************************************
*   DESC:  Create the command pool
****************************************************************************/
void CDevice::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    
    if( (m_lastResult = vkCreateCommandPool( m_logicalDevice, &poolInfo, nullptr, &m_commandPool)) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not create command pool! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Create the command buffers
****************************************************************************/
void CDevice::createCommandBuffers()
{
    m_commandBufferVec.resize( m_framebufferVec.size() );
    
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = (uint32_t) m_commandBufferVec.size();
    
    if( (m_lastResult = vkAllocateCommandBuffers( m_logicalDevice, &commandBufferAllocateInfo, m_commandBufferVec.data())) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not allocate command buffers! %s") % getError() ) );
    
    // Setup the command buffers
    for( size_t i = 0; i < m_commandBufferVec.size(); ++i )
    {
        // Start command buffer recording
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        
        if( (m_lastResult = vkBeginCommandBuffer( m_commandBufferVec[i], &beginInfo )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not begin recording command buffer! %s") % getError() ) );
        
        // Start a render pass
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = m_framebufferVec[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_swapchainInfo.imageExtent;
        
        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        
        vkCmdBeginRenderPass( m_commandBufferVec[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );
        vkCmdBindPipeline( m_commandBufferVec[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline );
        vkCmdDraw( m_commandBufferVec[i], 3, 1, 0, 0 );
        vkCmdEndRenderPass( m_commandBufferVec[i] );

        if( (m_lastResult = vkEndCommandBuffer( m_commandBufferVec[i] )) )
            throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not record command buffer! %s") % getError() ) );
    }
}


/***************************************************************************
*   DESC:  Create the Semaphores and fences
****************************************************************************/
void CDevice::createSyncObjects()
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
*   DESC:  Render the frame 
****************************************************************************/
void CDevice::render()
{
    vkWaitForFences( m_logicalDevice, 1, &m_frameFenceVec[m_currentFrame], VK_TRUE, UINT64_MAX );
    vkResetFences( m_logicalDevice, 1, &m_frameFenceVec[m_currentFrame] );
    
    uint32_t imageIndex(0);
    vkAcquireNextImageKHR( m_logicalDevice, m_swapchain, UINT64_MAX, m_imageAvailableSemaphoreVec[m_currentFrame], VK_NULL_HANDLE, &imageIndex );
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphoreVec[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBufferVec[imageIndex];
    
    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphoreVec[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    if( (m_lastResult = vkQueueSubmit( m_graphicsQueue, 1, &submitInfo, m_frameFenceVec[m_currentFrame] )) )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not submit draw command buffer! %s") % getError() ) );
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    
    VkSwapchainKHR swapChains[] = {m_swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    
    // Present the swap chain
    m_lastResult = vkQueuePresentKHR( m_presentQueue, &presentInfo );
    if( (m_lastResult == VK_ERROR_OUT_OF_DATE_KHR) || (m_lastResult == VK_SUBOPTIMAL_KHR) )
        recreateSwapChain();
    
    else if( m_lastResult != VK_SUCCESS )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not present swap chain image! %s") % getError() ) );
    
    m_currentFrame = (m_currentFrame + 1) % m_maxConcurrentFrames;
}


/***************************************************************************
*   DESC:  Recreate swap chain
****************************************************************************/
void CDevice::recreateSwapChain()
{
    // Wait for the logical device to be idle before doing the clean up
    vkDeviceWaitIdle( m_logicalDevice );
    
    // Destroy the current swap chain
    destroySwapChain();
    
    vkFreeCommandBuffers( m_logicalDevice, m_commandPool, static_cast<uint32_t>(m_commandBufferVec.size()), m_commandBufferVec.data() );
        
    // Setup the swap chain to be created
    setupSwapChain();
    
    // Create the swap chain
    createSwapChain();
    
    // Create the render pass
    createRenderPass();
    
    // Create the graphics pipeline
    createGraphicsPipeline();
    
    // Create the frame buffer
    createFrameBuffer();
    
    // Create the command buffers
    createCommandBuffers();
}




/***************************************************************************
*   DESC:  Check if the device extension is supported
****************************************************************************/
bool CDevice::isDeviceExtension( VkPhysicalDevice physicalDevice, const char* extenName )
{
    uint32_t deviceExtensionCount = 0;
    if( (m_lastResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, nullptr )) || (deviceExtensionCount == 0) )
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
uint32_t CDevice::findQueueFamilyIndex( VkPhysicalDevice physicalDevice, uint32_t queueMask )
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

uint32_t CDevice::findQueueFamilyIndex( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface )
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
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            
            if( (queueFamiliesVec[i].queueCount > 0) && presentSupport )
                return i;
        }
    }
    
    return UINT32_MAX;
}


/***************************************************************************
*   DESC:  read the file
****************************************************************************/
std::vector<char> CDevice::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file!");

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}





/***************************************************************************
*   DESC:  Enable/disable v-sync
****************************************************************************/
void CDevice::enableVSync( bool enable )
{
    if( SDL_GL_SetSwapInterval( (enable == true) ? 1 : 0 ) < 0 )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set VSync! SDL GL Error: %s") % SDL_GetError() ) );
}


/***************************************************************************
*   DESC:  Show/Hide the Window
****************************************************************************/
void CDevice::showWindow( bool visible )
{
    if( visible )
        SDL_ShowWindow( m_pWindow );
    else
        SDL_HideWindow( m_pWindow );
}


/***************************************************************************
*   DESC:  Set full screen or windowed mode
****************************************************************************/
void CDevice::setFullScreen( bool fullscreen )
{
    int flag(0);

    if( fullscreen )
        flag = SDL_WINDOW_FULLSCREEN;

    if( SDL_SetWindowFullscreen( m_pWindow, flag ) < 0 )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set full screen! SDL GL Error: %s") % SDL_GetError() ) );
}


/***************************************************************************
*   DESC:  Get the SDL window
****************************************************************************/
SDL_Window * CDevice::getWindow()
{
    return m_pWindow;
}


/***************************************************************************
*   DESC:  Init current gamepads plugged in at startup
****************************************************************************/
void CDevice::initStartupGamepads()
{
    // May not need this anymore
    //int newMappings = SDL_GameControllerAddMappingsFromFile("data/settings/gamecontrollerdb.txt");
    //NGenFunc::PostDebugMsg( boost::str( boost::format("New controller mappings found: %d - Number of controllers found: %d") % newMappings % (int)SDL_NumJoysticks() ) );

    for( int i = 0; i < SDL_NumJoysticks(); ++i )
        addGamepad( i );
}


/***************************************************************************
*   DESC:  Add/Remove the game pad
****************************************************************************/
void CDevice::addGamepad( int id )
{
    if( SDL_IsGameController( id ) )
    {
        SDL_GameController * pGamePad = SDL_GameControllerOpen(id);
        if( pGamePad != NULL )
        {
            //NGenFunc::PostDebugMsg( boost::str( boost::format("Game controller added: %d - %s") % id % SDL_GameControllerNameForIndex(id) ) );
            m_pGamepadMap.emplace( id, pGamePad );
        }
    }
}

void CDevice::removeGamepad( int id )
{
    auto iter = m_pGamepadMap.find( id );
    if( iter != m_pGamepadMap.end() )
    {
        SDL_GameControllerClose( iter->second );
        m_pGamepadMap.erase( iter );
    }
}


/***************************************************************************
*   DESC:  Get the gamepad count
****************************************************************************/
size_t CDevice::getGamepadCount()
{
    return m_pGamepadMap.size();
}


/***************************************************************************
*   DESC:  Get Vulkan error
****************************************************************************/
const char * CDevice::getError()
{
    auto iter = m_vulkanErrorMap.find( m_lastResult );
    if( iter != m_vulkanErrorMap.end() )
        return iter->second;

    return "Vulkan Unknown Error";
}
