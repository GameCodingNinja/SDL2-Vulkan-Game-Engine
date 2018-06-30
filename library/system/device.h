
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
#if defined(__ANDROID__)
#include <system/vulkan_wrapper.h>
#else
#include <vulkan/vulkan.h>
#endif


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
    
    // Render the frame
    void render();

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
    uint32_t findQueueFamilyIndex( VkPhysicalDevice physicalDevice );
    
    // Find the GPU memory type
    uint32_t findMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties );
    
    // Check if the device extension is supported
    bool isDeviceExtension( VkPhysicalDevice physicalDevice, const char* extenName );
    
    // Create the vulkan instance
    void createVulkanInstance(
        const std::vector<const char*> & validationNameVec,
        const std::vector<const char*> & instanceExtensionNameVec );
    
    // Create the Vulkan surface
    void createVulkanSurface();
    
    // Select a physical device (GPU)
    void selectPhysicalDevice();
    
    // Create the logical device
    void createLogicalDevice( const std::vector<const char*> & validationNameVec );
    
    // Setup the swap chain to be created
    void setupSwapChain();
    
    // Create the swap chain
    void createSwapChain();
    
    // Create the render pass
    void createRenderPass();
    
    // Create the graphics pipeline
    void createGraphicsPipeline();
    
    // Create the frame buffer
    void createFrameBuffer();
    
    // Create the command pool
    void createCommandPool();
    
    // Create a buffer
    void createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer & buffer,
        VkDeviceMemory & bufferMemory );
    
    // Copy a buffer
    void copyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size );
    
    // Create the vertex buffer
    void createVertexBuffer();
    
    // Create the index buffer
    void createIndexBuffer();
    
    // Create the command buffers
    void createCommandBuffers();
    
    // Create the Semaphores and fences
    void createSyncObjects();
    
    // Recreate swap chain
    void recreateSwapChain();
    
    // Destroy the swap chain
    void destroySwapChain();
    
    void tmpShaderSetup();

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
    
    // Graphics queue family index
    uint32_t m_graphicsQueueFamilyIndex;
    
    // Present queue family index
    uint32_t m_presentQueueFamilyIndex;

    // Graphics queue handle
    VkQueue m_graphicsQueue;
    
    // Present queue handle
    VkQueue m_presentQueue;
    
    // Surface capabilities
    VkSurfaceCapabilitiesKHR m_surfCapabilities = {};
    
    // Swap chain
    VkSwapchainKHR m_swapchain;
    
    // Swap chain info
    VkSwapchainCreateInfoKHR m_swapchainInfo = {};
    
    // Pipeline layout
    VkPipelineLayout m_pipelineLayout;
    
    // Render pass
    VkRenderPass m_renderPass;
    
    // Graphics pipeline
    VkPipeline m_graphicsPipeline;
    
    // Frame buffer
    std::vector<VkFramebuffer> m_framebufferVec;
    
    // Command pool
    VkCommandPool m_commandPool;
    
    // Vertex buffer
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
    
    // Command pool
    std::vector<VkCommandBuffer> m_commandBufferVec;

    // Map of gamepad pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;
    
    // Map of Vulkan errors
    std::map<VkResult, const char *> m_vulkanErrorMap;
    
    // Semaphores
    std::vector<VkSemaphore> m_imageAvailableSemaphoreVec;
    std::vector<VkSemaphore> m_renderFinishedSemaphoreVec;
    
    // Frame fence
    std::vector<VkFence> m_frameFenceVec;
    
    // The current frame
    int m_currentFrame;
    
    // Max concurrent frames that can be rendered
    int m_maxConcurrentFrames;
    
    // Last error result
    VkResult m_lastResult;
    
    // Swap chain images
    std::vector<VkImageView> m_swapChainImageViewVec;
    
    // Vulkan functions
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    VkDebugReportCallbackEXT vkDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
    
    // temporary members
    VkShaderModule m_shaderModuleVert;
    VkShaderModule m_shaderModuleFrag;
};

#endif  // __device_h__

