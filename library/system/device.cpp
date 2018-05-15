
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Class used for window and openGL management
************************************************************************/

/*#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif*/

// Physical component dependency
#include <system/device.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <common/size.h>

// Standard lib dependencies
#include <cstring>

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
PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = nullptr;
PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = nullptr;
PFN_vkDebugReportMessageEXT DebugReportMessage = nullptr;


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CDevice::CDevice() :
    m_pWindow(nullptr),
    m_vulkanInstance(nullptr),
    m_vulkanSurface(nullptr),
    m_physicalDevice(nullptr),
    m_logicalDevice(nullptr),
    m_graphicsQueue(nullptr),
    m_presentQueue(nullptr),
    m_lastResult(VK_SUCCESS)
{
    m_vulkanError.emplace( VK_SUCCESS,                        "Vulkan Success!" );
    m_vulkanError.emplace( VK_NOT_READY,                      "Vulkan Not Ready!" );
    m_vulkanError.emplace( VK_TIMEOUT,                        "Vulkan Timeout!" );
    m_vulkanError.emplace( VK_EVENT_SET,                      "Vulkan Event Set!" );
    m_vulkanError.emplace( VK_EVENT_RESET,                    "Vulkan Event Reset!" );
    m_vulkanError.emplace( VK_INCOMPLETE,                     "Vulkan Incomplete!" );
    m_vulkanError.emplace( VK_ERROR_OUT_OF_HOST_MEMORY,       "Vulkan Out Of Host memory!" );
    m_vulkanError.emplace( VK_ERROR_OUT_OF_DEVICE_MEMORY,     "Vulkan Out Of Device Memory!" );
    m_vulkanError.emplace( VK_ERROR_INITIALIZATION_FAILED,    "Vulkan Initialization Failed!" );
    m_vulkanError.emplace( VK_ERROR_DEVICE_LOST,              "Vulkan Device Lost!" );
    m_vulkanError.emplace( VK_ERROR_MEMORY_MAP_FAILED,        "Vulkan Memory Map Failed!" );
    m_vulkanError.emplace( VK_ERROR_LAYER_NOT_PRESENT,        "Vulkan Layer Not Present!" );
    m_vulkanError.emplace( VK_ERROR_EXTENSION_NOT_PRESENT,    "Vulkan nExtension Not Present!" );
    m_vulkanError.emplace( VK_ERROR_FEATURE_NOT_PRESENT,      "Vulkan Feature Not Present!" );
    m_vulkanError.emplace( VK_ERROR_INCOMPATIBLE_DRIVER,      "Vulkan Incompatible Driver!" );
    m_vulkanError.emplace( VK_ERROR_TOO_MANY_OBJECTS,         "Vulkan Too Many Objects!" );
    m_vulkanError.emplace( VK_ERROR_FORMAT_NOT_SUPPORTED,     "Vulkan Format Not Supported!" );
    m_vulkanError.emplace( VK_ERROR_FRAGMENTED_POOL,          "Vulkan Fragmented Pool!" );
    m_vulkanError.emplace( VK_ERROR_OUT_OF_POOL_MEMORY,       "Vulkan Out Of Pool Memory!" );
    m_vulkanError.emplace( VK_ERROR_INVALID_EXTERNAL_HANDLE,  "Vulkan Invalid External Handle!" );
    m_vulkanError.emplace( VK_ERROR_SURFACE_LOST_KHR,         "Vulkan Surface Lost KHR!" );
    m_vulkanError.emplace( VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "Vulkan Native Window In Use KHR!" );
    m_vulkanError.emplace( VK_SUBOPTIMAL_KHR,                 "Vulkan Suboptimal KHR!" );
    m_vulkanError.emplace( VK_ERROR_OUT_OF_DATE_KHR,          "Vulkan Out Of Date KHR!" );
    m_vulkanError.emplace( VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "Vulkan Incompatible Display KHR!" );
    m_vulkanError.emplace( VK_ERROR_VALIDATION_FAILED_EXT,    "Vulkan Validation Failed Ext!" );
    m_vulkanError.emplace( VK_ERROR_INVALID_SHADER_NV,        "Vulkan Invalid Shader NV!" );
    m_vulkanError.emplace( VK_ERROR_NOT_PERMITTED_EXT,        "Vulkan Not Permitted Ext!" );
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
        vkDestroyDevice(m_logicalDevice, nullptr);
        m_logicalDevice = nullptr;
    }
    
    if( m_vulkanInstance != nullptr )
    {
        if( CSettings::Instance().getDebugMode() && (DestroyDebugReportCallback != nullptr) )
            DestroyDebugReportCallback(m_vulkanInstance, msg_callback, nullptr);

        vkDestroyInstance(m_vulkanInstance, nullptr);
        m_vulkanInstance = nullptr;
    }

    if( m_pWindow != nullptr )
    {
        SDL_DestroyWindow( m_pWindow );
        m_pWindow = nullptr;
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

    // Get the window size
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

    std::vector<const char*> extensionNameVec(instanceExtensionCount);
    std::vector<const char*> validationNameVec;
        
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, extensionNameVec.data()) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension names!", SDL_GetError() );

    // If we are in debug mode, add validation and debug reporting extension
    if( CSettings::Instance().getDebugMode() )
    {
        validationNameVec.push_back("VK_LAYER_LUNARG_standard_validation");
        extensionNameVec.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
        
        // Print out extension list for debug mode
        for( auto iter : extensionNameVec )
            NGenFunc::PostDebugMsg( "Instance Extension: " + std::string(iter));
    }

    const VkApplicationInfo app = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = CSettings::Instance().getGameName().c_str(),
        .applicationVersion = CSettings::Instance().getGameVersion(),
        .pEngineName = CSettings::Instance().getEngineName().c_str(),
        .engineVersion = CSettings::Instance().getEngineVersion(),
        .apiVersion = VK_MAKE_VERSION(CSettings::Instance().getMajorVersion(), CSettings::Instance().getMinorVersion(), 0)
    };

    VkInstanceCreateInfo inst_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0, // Reserved for future use
        .pApplicationInfo = &app,
        .enabledLayerCount = static_cast<uint32_t>(validationNameVec.size()),
        .ppEnabledLayerNames = validationNameVec.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensionNameVec.size()),
        .ppEnabledExtensionNames = extensionNameVec.data()
    };

    // Try to create the instance
    if( (m_lastResult = vkCreateInstance(&inst_info, nullptr, &m_vulkanInstance)) )
        throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Could not create instance! %s") % getError() ) );

    
    ///////////////////////////////////////////////////
    // Setup the debug call back
    //////////////////////////////////////////////////
    
    if( CSettings::Instance().getDebugMode() )
    {
        if( !(CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr( m_vulkanInstance, "vkCreateDebugReportCallbackEXT")) )
            throw NExcept::CCriticalException( "Vulkin Error!", "Unable to find PFN_vkCreateDebugReportCallbackEXT!" );

        if( !(DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr( m_vulkanInstance, "vkDestroyDebugReportCallbackEXT")) )
            throw NExcept::CCriticalException( "Vulkin Error!", "Unable to find PFN_vkDestroyDebugReportCallbackEXT!" );

        if( !(DebugReportMessage = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(m_vulkanInstance, "vkDebugReportMessageEXT")) )
            throw NExcept::CCriticalException( "Vulkin Error!", "Unable to find PFN_vkDebugReportMessageEXT!" );

        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
            .pfnCallback = debugCallback,
            .pUserData = nullptr
        };

        if( (m_lastResult = CreateDebugReportCallback(m_vulkanInstance, &dbgCreateInfo, nullptr, &msg_callback)) )
            throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Could not create debug report callback! %s") % getError() ) );
    }
    
    // Create the Vulkan surface
    if( !SDL_Vulkan_CreateSurface( m_pWindow, m_vulkanInstance, &m_vulkanSurface ) )
        throw NExcept::CCriticalException("Could not create Vulkan surface!", SDL_GetError() );
    
    

    ///////////////////////////////////////////////////
    // Find a suitable GPU
    //////////////////////////////////////////////////
    
    // Select a graphics device
    uint32_t gpuCount(0);
    if( (m_lastResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, nullptr)) || (gpuCount == 0) )
        throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Could not enumerate physical device count! %s") % getError() ) );

    std::vector<VkPhysicalDevice> physicalDeviceHandleVec(gpuCount);

    if( (m_lastResult = vkEnumeratePhysicalDevices(m_vulkanInstance, &gpuCount, physicalDeviceHandleVec.data())) )
        throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Could not enumerate physical device info! %s") % getError() ) );

    // Select the discrete GPU if one is available that supports the graphics bit
    uint32_t graphicsQueueFamilyIndex = UINT32_MAX;
    for( auto iter : physicalDeviceHandleVec )
    {
        auto props = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(iter, &props);

        // Print out the GPU in debug mode
        if( CSettings::Instance().getDebugMode() )
            NGenFunc::PostDebugMsg( "GPU: " + std::string(props.deviceName));
        
        graphicsQueueFamilyIndex = findQueueFamilyIndex( iter, VK_QUEUE_GRAPHICS_BIT );

        // If we found a discrete GPU, our work here is done
        if( props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        {
            if( graphicsQueueFamilyIndex != UINT32_MAX )
            {
                m_physicalDevice = iter;
                break;
            }
        }

        if( graphicsQueueFamilyIndex != UINT32_MAX )
            m_physicalDevice = iter;
    }

    if( (m_physicalDevice == nullptr) || (graphicsQueueFamilyIndex == UINT32_MAX) )
        throw NExcept::CCriticalException( "Vulkin Error!", "Suitable GPU could not be found!" );
    

    ///////////////////////////////////////////////////
    // Create a logical device
    //////////////////////////////////////////////////
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = graphicsQueueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority
    };
    
    // Get all the features supported on this device
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);
    
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    
    if( CSettings::Instance().getDebugMode() )
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationNameVec.size());
        createInfo.ppEnabledLayerNames = validationNameVec.data();
    }
    
    // Create the logical device
    if( (m_lastResult = vkCreateDevice( m_physicalDevice, &createInfo, nullptr, &m_logicalDevice)) )
        throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Failed to create logical device! %s") % getError() ) );
    
    // Get a handle to the graphics queue
    vkGetDeviceQueue( m_logicalDevice, graphicsQueueFamilyIndex, 0, &m_graphicsQueue );
    if( m_graphicsQueue == nullptr )
        throw NExcept::CCriticalException( "Vulkin Error!", "Could not get handle to graphics queue family!" );
    
    // Get a handle to the present queue - Could be different but most likely in the same queue family
    uint32_t presentQueueFamilyIndex = findQueueFamilyIndex( m_physicalDevice, m_vulkanSurface );
    vkGetDeviceQueue( m_logicalDevice, presentQueueFamilyIndex, 0, &m_presentQueue );
    if( m_presentQueue == nullptr )
        throw NExcept::CCriticalException( "Vulkin Error!", "Could not get handle to present queue family!" );
    
    
    ///////////////////////////////////////////////////
    // Setup swap chain
    //////////////////////////////////////////////////
    
    if( !isDeviceExtension( m_physicalDevice, VK_KHR_SWAPCHAIN_EXTENSION_NAME ) )
        throw NExcept::CCriticalException( "Vulkin Error!", "No swap chain support!" );
    
    
    
    
    
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
*   DESC:  Check if the device extension is supported
****************************************************************************/
bool CDevice::isDeviceExtension( VkPhysicalDevice physicalDevice, const char* extenName )
{
    uint32_t deviceExtensionCount = 0;
    if( (m_lastResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, nullptr )) || (deviceExtensionCount == 0) )
        throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Could not enumerate device extension count! %s") % getError() ) );
    
    std::vector<VkExtensionProperties> availableExtensions(deviceExtensionCount);
    
    if( (m_lastResult = vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &deviceExtensionCount, availableExtensions.data() )) )
        throw NExcept::CCriticalException( "Vulkin Error!", boost::str( boost::format("Could not enumerate device extension info! %s") % getError() ) );
    
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
    auto iter = m_vulkanError.find( m_lastResult );
    if( iter != m_vulkanError.end() )
        return iter->second;

    return "Vulkan Unknown Error";
}
