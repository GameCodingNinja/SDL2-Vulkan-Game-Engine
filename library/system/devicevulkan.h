
/************************************************************************
*    FILE NAME:       devicevulkan.h
*
*    DESCRIPTION:     Class used for Vulkan API implementation
************************************************************************/

#ifndef __device_vulkan_h__
#define __device_vulkan_h__

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/texture.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Vulkan lib dependencies
#if defined(__ANDROID__)
#include <system/vulkan_wrapper.h>
#else
#include <vulkan/vulkan.h>
#endif


class CDeviceVulkan
{
protected:
    
    // Constructor
    CDeviceVulkan();

    // Destructor
    virtual ~CDeviceVulkan();
    
    // Create Vulkan instance
    void create(
        const std::vector<const char*> & validationNameVec,
        const std::vector<const char*> & instanceExtensionNameVec );
    
    // Destroy the Vulkan instance
    void destroy();
    
    // A controlled way to destroy the game created assets
    virtual void destroyAssets() = 0;
    
    // Get Vulkan error
    const char * getError();
    
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
    virtual void createSurface() = 0;
    
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
    
    // Create the descriptor set layout
    void createDescriptorSetLayout();
    
    // Create the graphics pipeline
    void createGraphicsPipeline();
    
    // Create the frame buffer
    void createFrameBuffer();
    
    // Create the command pool
    void createCommandPool();
    
    // Create depth resources
    void createDepthResources();
    
    // Create texture
    void createTexture( NVulkan::CTexture & texture, const std::string & filePath, bool mipMap );
    
    // Create texture sampler
    VkSampler createTextureSampler( uint32_t mipLevels );
    
    // Create the vertex buffer
    void createVertexBuffer();
    
    // Create the index buffer
    void createIndexBuffer();
    
    // Create uniform buffer
    void createUniformBuffer();
    
    // Create descriptor pool
    void createDescriptorPool();
    
    // Create descriptor sets
    void createDescriptorSet();
    
    // Create the command buffers
    void createCommandBuffers();
    
    // Record the command buffers
    void recordCommandBuffers( uint32_t cmdBufIndex );
    
    // Update the uniform buffer
    void updateUniformBuffer( uint32_t unfBufIndex );
    
    // Create the Semaphores and fences
    void createSyncObjects();
    
    // Recreate swap chain
    void recreateSwapChain();
    
    // Create a buffer
    void createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer & buffer,
        VkDeviceMemory & bufferMemory );
    
    // Create image
    void createImage(
        uint32_t width,
        uint32_t height,
        uint32_t mipLevels,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkImage & image,
        VkDeviceMemory & imageMemory );
    
    // Copy a buffer
    void copyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size );
    
    // Destroy the swap chain
    void destroySwapChain();
    
    // Copy buffer helper functions
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands( VkCommandBuffer commandBuffer );
    
    // Transition image layout
    void transitionImageLayout( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels );
    
    // Copy a buffer to an image
    void copyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height );
    
    // Create the image view
    VkImageView createImageView( VkImage image, VkFormat format, uint32_t mipLevels, VkImageAspectFlags aspectFlags );
    
    // Generate Mipmaps
    void generateMipmaps( VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels );
    
    // Find supported format
    VkFormat findSupportedFormat( const std::vector<VkFormat> & candidates, VkImageTiling tiling, VkFormatFeatureFlags features );
    
    // Find the depth format
    VkFormat findDepthFormat();
    
    
    
    void tmpShaderSetup();
    
    // Create texture image
    virtual void createTextureImage() = 0;

protected:
    
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
    
    // Descriptor set layout
    VkDescriptorSetLayout m_descriptorSetLayout;
    
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
    
    // Uniform buffers
    std::vector<VkBuffer> m_uniformBufVec;
    std::vector<VkDeviceMemory> m_uniformBufMemVec;
    
    // Descriptor pool
    VkDescriptorPool m_descriptorPool;
    
    // Descriptor Set
    std::vector<VkDescriptorSet> m_descriptorSetVec;
    
    // Command pool
    std::vector<VkCommandBuffer> m_primaryCmdBufVec;
    
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
    
    // Texture members
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;
    
    // Depth buffer members
    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;
    
    // Vulkan functions
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    VkDebugReportCallbackEXT vkDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
    
    // temporary members
    VkShaderModule m_shaderModuleVert;
    VkShaderModule m_shaderModuleFrag;
    
    std::vector<VkCommandBuffer> m_squareCmdBufVec;
};

#endif  // __device_vulkan_h__

