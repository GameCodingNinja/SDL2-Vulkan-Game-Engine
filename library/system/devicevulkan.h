
/************************************************************************
*    FILE NAME:       devicevulkan.h
*
*    DESCRIPTION:     Class used for Vulkan API implementation
************************************************************************/

#ifndef __device_vulkan_h__
#define __device_vulkan_h__

// Game lib dependencies
#include <common/defs.h>
#include <common/memorybuffer.h>

// Standard lib dependencies
#include <cstring>
#include <string>
#include <vector>
#include <map>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CTexture;
class CMemoryBuffer;
class CPipelineData;
class CDescriptorData;

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
        const std::vector<const char*> & instanceExtensionNameVec,
        const std::vector<const char*> & physicalDeviceExtensionNameVec );
    
    // Destroy the Vulkan instance
    void destroy();
    
    // Create the command pool
    VkCommandPool createCommandPool();
    
    // Create secondary command buffers
    std::vector<VkCommandBuffer> createSecondaryCommandBuffers( VkCommandPool cmdPool );
    
    // Destroy the swap chain
    virtual void destroySwapChain();
    
    // Recreate swap chain
    void recreateSwapChain();
    
    // Recreate the pipeline
    virtual void recreatePipelines() = 0;
    
    // Create the uniform buffer Vec for ubo buffer writes
    std::vector<CMemoryBuffer> createUniformBufferVec( VkDeviceSize sizeOfUniformBuf );
    
    // Create texture
    void createTexture( CTexture & texture, bool mipMap );
    
    // Create descriptor pool
    VkDescriptorPool createDescriptorPool( const CDescriptorData & descData, size_t setCount );
    
    // Create descriptor sets
    std::vector<VkDescriptorSet> createDescriptorSetVec(
        const CTexture & texture,
        const CDescriptorData & descData,
        const CPipelineData & pipelineData,
        const std::vector<CMemoryBuffer> & uniformBufVec,
        VkDescriptorPool descriptorPool );
    
    // Create the shader
    VkShaderModule createShader( const std::string & filePath );
    
    // Create the descriptor set layout
    VkDescriptorSetLayout createDescriptorSetLayout( CDescriptorData & descData );
    
    // Create the pipeline layout
    VkPipelineLayout createPipelineLayout( VkDescriptorSetLayout descriptorSetLayout );
    
    // Create the pipeline
    void createPipeline( CPipelineData & pipelineData );
    
    // Get Vulkan error
    const char * getError();
    
    // Load a buffer into video card memory
    template <typename T>
    void loadVKBuffer( std::vector<T> dataVec, CMemoryBuffer & memoryBuffer, VkBufferUsageFlagBits bufferUsageFlag )
    {
        VkDeviceSize bufferSize = sizeof(dataVec.back()) * dataVec.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory );

        void* data;
        vkMapMemory( m_logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data );
        std::memcpy( data, dataVec.data(), (size_t) bufferSize );
        vkUnmapMemory( m_logicalDevice, stagingBufferMemory );

        createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsageFlag,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            memoryBuffer.m_buffer,
            memoryBuffer.m_deviceMemory );

        copyBuffer( stagingBuffer, memoryBuffer.m_buffer, bufferSize );

        vkDestroyBuffer( m_logicalDevice, stagingBuffer, nullptr );
        vkFreeMemory( m_logicalDevice, stagingBufferMemory, nullptr );
    }
    
private:
    
    // A controlled way to destroy the game created assets
    virtual void destroyAssets() = 0;
    
    // Create the Vulkan surface
    virtual void createSurface() = 0;
    
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
    
    // Select a physical device (GPU)
    void selectPhysicalDevice();
    
    // Create the logical device
    void createLogicalDevice(
        const std::vector<const char*> & validationNameVec,
        const std::vector<const char*> & physicalDeviceExtensionNameVec );
    
    // Setup the swap chain to be created
    void setupSwapChain();
    
    // Create the swap chain
    void createSwapChain();
    
    // Create the render pass
    void createRenderPass();
    
    // Create the frame buffer
    void createFrameBuffer();
    
    // Create the primary command pool
    void createPrimaryCommandPool();
    
    // Create depth resources
    void createDepthResources();
    
    // Create the primary command buffers
    void createPrimaryCommandBuffers();
    
    // Create the Semaphores and fences
    void createSyncObjects();
    
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
    
    // Create a buffer
    void createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer & buffer,
        VkDeviceMemory & bufferMemory );
    
    // Copy a buffer
    void copyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size );
    
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
    
    // Create texture sampler
    VkSampler createTextureSampler( uint32_t mipLevels );
    
    // Find supported format
    VkFormat findSupportedFormat( const std::vector<VkFormat> & candidates, VkImageTiling tiling, VkFormatFeatureFlags features );
    
    // Find the depth format
    VkFormat findDepthFormat();

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
    
    // Swap chain
    VkSwapchainKHR m_swapchain;
    
    // Swap chain info
    VkSwapchainCreateInfoKHR m_swapchainInfo = {};
    
    // Render pass
    VkRenderPass m_renderPass;
    
    // Frame buffer
    std::vector<VkFramebuffer> m_framebufferVec;
    
    // Primary Command pool. Only use for primary command buffers
    VkCommandPool m_primaryCmdPool;
    
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
    
    // Depth buffer members
    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;
    
    // Vulkan functions
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    VkDebugReportCallbackEXT vkDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
    PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
};

#endif  // __device_vulkan_h__

