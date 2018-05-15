
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Class used for window and openGL management
************************************************************************/

#ifndef __device_h__
#define __device_h__

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// SDL lib dependencies
#include <SDL.h>

// Vulkan lib dependencies
#include <vulkan/vulkan.h>

class CDevice
{
public:

    // Get the instance
    static CDevice & Instance()
    {
        static CDevice device;
        return device;
    }

    // Destroy the window and Vulkan instance
    void destroy();
    
    // Create the window and OpenGL context
    void create();

    // Show/Hide the Window
    void showWindow( bool visible );

    // Get the SDL window
    SDL_Window * getWindow();

    // Close out the game pads
    void closeGamepads();

    // Set full screen or windowed mode
    void setFullScreen( bool fullscreen );

    // Enable/disable v-sync
    void enableVSync( bool enable );

    // Add/Remove the game pad
    void addGamepad( int id );
    void removeGamepad( int id );
    
    // Get the gamepad count
    size_t getGamepadCount();
    
    // Get Vulkan error
    const char * getError();

private:

    // Constructor
    CDevice();

    // Destructor
    virtual ~CDevice();
    
    // Init current gamepads plugged in at startup
    void initStartupGamepads();
    
    // Find the queue family index
    uint32_t findQueueFamilyIndex( VkPhysicalDevice physicalDevice, uint32_t queueMask );
    uint32_t findQueueFamilyIndex( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface );
    
    // Check if the device extension is supported
    bool isDeviceExtension( VkPhysicalDevice physicalDevice, const char* extenName );

private:

    // The window we'll be rendering to
    SDL_Window * m_pWindow;
    
    // Vulkan instance
    VkInstance m_vulkanInstance;
    
    // Vulkan rendering surface
    VkSurfaceKHR m_vulkanSurface;
    
    // The selected GPU
    VkPhysicalDevice m_physicalDevice;
    
    // Vulkan logical device
    VkDevice m_logicalDevice;
    
    // Graphics queue handle
    VkQueue m_graphicsQueue;
    
    // Present queue handle
    VkQueue m_presentQueue;

    // OpenGL context
    //SDL_GLContext m_context;

    // Map of gamepad pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;
    
    // Map of Vulkan errors
    std::map<VkResult, const char *> m_vulkanError;
    
    // Last error result
    VkResult m_lastResult;
    
    // List of grpahics hardware
    //std::vector<VkPhysicalDevice> m_physicalDeviceVec;
};

#endif  // __device_h__

