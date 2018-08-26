
/************************************************************************
*    FILE NAME:       device.cpp
*
*    DESCRIPTION:     Interface class to Vulkan API
************************************************************************/

// Physical component dependency
#include <system/device.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies

// SDL lib dependencies
#include <SDL.h>
#include <SDL_vulkan.h>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CDevice::CDevice() :
    m_pWindow(nullptr)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CDevice::~CDevice()
{
}   // destructor


/***************************************************************************
*   DESC:  Create the window and Vulkan instance
****************************************************************************/
void CDevice::create( const std::string & vertShader, const std::string & fragShader )
{
    // Initialize SDL - The File I/O and Threading subsystems are initialized by default.
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER ) < 0 )
        throw NExcept::CCriticalException("SDL could not initialize!", SDL_GetError() );

    // All file I/O is handled by SDL and SDL_Init must be called before doing any I/O.
    CSettings::Instance().loadXML();
    
    // Get the render size of the window
    const CSize<int> size( CSettings::Instance().getSize() );
    
    uint32_t flags( SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );
    #if !(defined(__IOS__) || defined(__ANDROID__))
    flags |= SDL_WINDOW_RESIZABLE;
    #endif

    // Create window
    m_pWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.getW(), size.getH(), flags );
    if( m_pWindow == nullptr )
        throw NExcept::CCriticalException("Game window could not be created!", SDL_GetError() );
    
    uint32_t instanceExtensionCount(0);
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, nullptr) || (instanceExtensionCount == 0) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension count!", SDL_GetError() );

    std::vector<const char*> instanceExtensionNameVec(instanceExtensionCount);
    std::vector<const char*> validationNameVec;
        
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, instanceExtensionNameVec.data()) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension names!", SDL_GetError() );

    // If we want validation, add it and debug reporting extension
    if( CSettings::Instance().isValidationLayers() )
    {
        validationNameVec.push_back("VK_LAYER_LUNARG_standard_validation");
        instanceExtensionNameVec.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
        
        // Print out extension list for validation layers
        for( auto iter : instanceExtensionNameVec )
            NGenFunc::PostDebugMsg( "Instance Extension: " + std::string(iter));
    }
    
    // Create the Vulkan instance and graphics pipeline
    CDeviceVulkan::create( validationNameVec, instanceExtensionNameVec, vertShader, fragShader );
    
    createTextureImage();
    createVertexBuffer();
    
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
*   DESC:  Destroy the window and Vulkan instance
****************************************************************************/
void CDevice::destroy()
{
    // Destroy the Vulkan instance
    CDeviceVulkan::destroy();

    if( m_pWindow != nullptr )
    {
        SDL_DestroyWindow( m_pWindow );
        m_pWindow = nullptr;
    }
}


/***************************************************************************
*   DESC:  A controlled way to destroy the game created assets
****************************************************************************/
void CDevice::destroyAssets()
{
    if( m_logicalDevice != VK_NULL_HANDLE )
    {
        // Free all textures in all groups
        for( auto & mapMapIter : m_textureMapMap )
        {
            for( auto & mapIter : mapMapIter.second )
            {
                vkDestroyImage( m_logicalDevice, mapIter.second.m_textureImage, nullptr );
                vkFreeMemory( m_logicalDevice, mapIter.second.m_textureImageMemory, nullptr );
                vkDestroyImageView( m_logicalDevice, mapIter.second.m_textureImageView, nullptr );
                vkDestroySampler( m_logicalDevice, mapIter.second.m_textureSampler, nullptr );
            }
        }
        
        m_textureMapMap.clear();

        // Free all memory buffer groups
        for( auto & mapMapIter : m_bufferMapMap )
        {
            for( auto & mapIter : mapMapIter.second )
            {
                vkDestroyBuffer( m_logicalDevice, mapIter.second.m_buffer, nullptr );
                vkFreeMemory( m_logicalDevice, mapIter.second.m_deviceMemory, nullptr );
            }
        }
        
        m_bufferMapMap.clear();
    }
}


/***************************************************************************
*   DESC:  Render the frame 
****************************************************************************/
void CDevice::render()
{
    vkWaitForFences( m_logicalDevice, 1, &m_frameFenceVec[m_currentFrame], VK_TRUE, UINT64_MAX );
    
    uint32_t imageIndex(0);
    m_lastResult = vkAcquireNextImageKHR( m_logicalDevice, m_swapchain, UINT64_MAX, m_imageAvailableSemaphoreVec[m_currentFrame], VK_NULL_HANDLE, &imageIndex );
    if( (m_lastResult == VK_ERROR_OUT_OF_DATE_KHR) || (m_lastResult == VK_SUBOPTIMAL_KHR) )
    {
        recreateSwapChain();
        return;
    }
    else if( m_lastResult == VK_ERROR_SURFACE_LOST_KHR )
        createSurface();

    else if( m_lastResult != VK_SUCCESS )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not present swap chain image! %s") % getError() ) );
    
    // Update the uniform buffer
    updateUniformBuffer( imageIndex );
    
    // Record the command buffers
    recordCommandBuffers( imageIndex );
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphoreVec[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_primaryCmdBufVec[imageIndex];
    
    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphoreVec[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    vkResetFences( m_logicalDevice, 1, &m_frameFenceVec[m_currentFrame] );
    
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
    
    else if( m_lastResult == VK_ERROR_SURFACE_LOST_KHR )
        createSurface();
    
    else if( m_lastResult != VK_SUCCESS )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Could not present swap chain image! %s") % getError() ) );
    
    m_currentFrame = (m_currentFrame + 1) % m_maxConcurrentFrames;
}


/***************************************************************************
*   DESC:  Create the surface
****************************************************************************/
void CDevice::createSurface()
{
    if( !SDL_Vulkan_CreateSurface( m_pWindow, m_vulkanInstance, &m_vulkanSurface ) )
        throw NExcept::CCriticalException("Could not create Vulkan surface!", SDL_GetError() );
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

/************************************************************************
*    DESC:  Load the image from file path
************************************************************************/
NVulkan::CTexture & CDevice::loadTexture( const std::string & group, const std::string & filePath, bool mipMap )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_textureMapMap.find( group );
    if( mapMapIter == m_textureMapMap.end() )
        mapMapIter = m_textureMapMap.emplace( group, std::map<const std::string, NVulkan::CTexture>() ).first;

    // See if this texture has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, load the texture and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        NVulkan::CTexture texture;

        // Load the image from file path
        createTexture( texture, filePath, mipMap );

        // Insert the new texture info
        mapIter = mapMapIter->second.emplace( filePath, texture ).first;
    }
    
    return mapIter->second;
}


/************************************************************************
*    DESC:  Delete a texture in a group
************************************************************************/
void CDevice::deleteTextureGroup( const std::string & group )
{
    // Free the texture group if it exists
    auto mapMapIter = m_textureMapMap.find( group );
    if( mapMapIter != m_textureMapMap.end() )
    {
        // Delete all the textures in this group
        for( auto & mapIter : mapMapIter->second )
        {
            vkDestroyImage( m_logicalDevice, mapIter.second.m_textureImage, nullptr );
            vkFreeMemory( m_logicalDevice, mapIter.second.m_textureImageMemory, nullptr );
            vkDestroyImageView( m_logicalDevice, mapIter.second.m_textureImageView, nullptr );
            vkDestroySampler( m_logicalDevice, mapIter.second.m_textureSampler, nullptr );
        }

        // Erase this group
        m_textureMapMap.erase( mapMapIter );
    }
}


/***************************************************************************
*   DESC:  Enable/disable v-sync
****************************************************************************/
void CDevice::enableVSync( bool enable )
{
    //if( SDL_GL_SetSwapInterval( (enable == true) ? 1 : 0 ) < 0 )
    //    NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set VSync! SDL GL Error: %s") % SDL_GetError() ) );
}


/***************************************************************************
*   DESC:  Set full screen or windowed mode
****************************************************************************/
void CDevice::setFullScreen( bool fullscreen )
{
    if( m_pWindow )
    {
        int flag(0);

        if( fullscreen )
            flag = SDL_WINDOW_FULLSCREEN;

        if( SDL_SetWindowFullscreen( m_pWindow, flag ) < 0 )
            NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set full screen! SDL GL Error: %s") % SDL_GetError() ) );
    }
}


/***************************************************************************
*   DESC:  Display error message
****************************************************************************/
void CDevice::displayErrorMsg( const std::string & title, const std::string & msg )
{
    if( m_pWindow )
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), m_pWindow );
}




/***************************************************************************
*   DESC:  Set window title
****************************************************************************/
void CDevice::setWindowTitle( const std::string & title )
{
    if( m_pWindow )
        SDL_SetWindowTitle( m_pWindow, title.c_str() );
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
*   DESC:  Get window
****************************************************************************/
SDL_Window * CDevice::getWindow()
{
    return m_pWindow;
}






/***************************************************************************
*   DESC:  Create texture image
****************************************************************************/
void CDevice::createTextureImage()
{
    NVulkan::CTexture texture;
    
    texture = loadTexture( "test", "data/textures/titleScreen/title_background.jpg" );
    
    m_textureImage = texture.m_textureImage;
    m_textureImageMemory = texture.m_textureImageMemory;
    m_textureImageView = texture.m_textureImageView;
    m_textureSampler = texture.m_textureSampler;
    m_descriptorSetVec = texture.m_descriptorSetVec;
}

/***************************************************************************
*   DESC:  Create texture image
****************************************************************************/
void CDevice::createVertexBuffer()
{
    const std::vector<CVertex> vertices =
    {
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}
    };

    const std::vector<uint16_t> indices =
    {
        0, 1, 2, 2, 3, 0
    };
    
    CMemoryBuffer vboBuffer = loadBuffer( "test", "vbo", vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
    CMemoryBuffer iboBuffer = loadBuffer( "test", "ibo", indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
    
    // Vertex buffer
    m_vertexBuffer = vboBuffer.m_buffer;
    m_vertexBufferMemory = vboBuffer.m_deviceMemory;
    m_indexBuffer = iboBuffer.m_buffer;
    m_indexBufferMemory = iboBuffer.m_deviceMemory;
}