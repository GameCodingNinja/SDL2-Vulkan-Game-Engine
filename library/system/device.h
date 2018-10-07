
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Interface class to Vulkan API
************************************************************************/

#ifndef __device_h__
#define __device_h__

// Physical component dependency
#include <system/devicevulkan.h>

// Standard lib dependencies
#include <functional>
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
struct SDL_Window;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;
class CPipelineData;
class CDescriptorData;
class CUboData;
class CPushDescriptorSet;
class CTexture;

class CDevice : public CDeviceVulkan
{
public:

    // Get the instance
    static CDevice & Instance()
    {
        static CDevice device;
        return device;
    }

    // Create the window and Vulkan instance
    void create( std::function<void(uint32_t)> callback, const std::string & pipelineCfg );

    // Destroy the window and Vulkan instance
    void destroy();

    // Update the command buffer vector
    void updateCommandBuffer( VkCommandBuffer cmdBuf );

    // Render the frame
    void render();

    // Create secondary command buffers
    std::vector<VkCommandBuffer> createSecondaryCommandBuffers( const std::string & group );

    // Delete a secondary command buffer of a specific group
    void deleteCommandBuffer( const std::string & group, std::vector<VkCommandBuffer> & commandBufVec );

    // Create descriptor sets
    /*std::vector<VkDescriptorSet> createDescriptorSetVec(
        const std::string & group,
        int pipelineIndex,
        const CTexture & texture,
        const std::vector<CMemoryBuffer> & uniformBufVec );*/

    // Create push descriptor set
    void createPushDescriptorSet(
        uint32_t pipelineIndex,
        const CTexture & texture,
        const std::vector<CMemoryBuffer> & uniformBufVec,
        CPushDescriptorSet & pushDescSet );

    // Load the image from file path
    CTexture & createTexture( const std::string & group, const std::string & filePath, bool mipMap = false );

    // Create the descriptor pool group for the textures
    void createDescriptorPoolGroup( const std::string & group, const std::string & descrId, const CDescriptorData & descData, size_t count );

    // Create uniform buffer
    std::vector<CMemoryBuffer> createUniformBufferVec( uint32_t pipelineIndex );
    //std::vector<CMemoryBuffer> createUniformBufferVec( const std::string & group, const std::string & uboId );

    // Delete group assets
    void deleteGroupAssets( const std::string & group );

    // Delete the command pool group
    void deleteCommandPoolGroup( const std::string & group );

    // Load a buffer into video card memory
    template <typename T>
    CMemoryBuffer & creatMemoryBuffer( const std::string & group, const std::string & id, std::vector<T> dataVec, VkBufferUsageFlagBits bufferUsageFlag )
    {
        // Create the map group if it doesn't already exist
        auto mapIter = m_memoryBufferMapMap.find( group );
        if( mapIter == m_memoryBufferMapMap.end() )
            mapIter = m_memoryBufferMapMap.emplace( group, std::map<const std::string, CMemoryBuffer>() ).first;

        // See if this memory buffer has already been loaded
        auto iter = mapIter->second.find( id );

        // If it's not found, create the memory buffer and add it to the list
        if( iter == mapIter->second.end() )
        {
            CMemoryBuffer memoryBuffer;

            // Load buffer into video memory
            CDeviceVulkan::creatMemoryBuffer( dataVec, memoryBuffer, bufferUsageFlag );

            // Insert the new texture info
            iter = mapIter->second.emplace( id, memoryBuffer ).first;
        }

        return iter->second;
    }
    
    template <typename T>
    void creatMemoryBuffer( std::vector<T> dataVec, CMemoryBuffer & memoryBuffer, VkBufferUsageFlagBits bufferUsageFlag )
    {
        // Load buffer into video memory
        CDeviceVulkan::creatMemoryBuffer( dataVec, memoryBuffer, bufferUsageFlag );
    }

    // Update the uniform buffer
    template <typename T>
    void updateUniformBuffer( T & ubo, VkDeviceMemory deviceMemory )
    {
        void* data;
        vkMapMemory( m_logicalDevice, deviceMemory, 0, sizeof(ubo), 0, &data );
        std::memcpy( data, &ubo, sizeof(ubo));
        vkUnmapMemory( m_logicalDevice, deviceMemory );
    }

    // Get the memory buffer if it exists
    CMemoryBuffer getMemoryBuffer( const std::string & group, const std::string & id );

    // Get the pipeline data
    const CPipelineData & getPipelineData( int index ) const;

    // Get the pipeline index
    int getPipelineIndex( const std::string & id );

    // Show/Hide the Window
    void showWindow( bool visible );

    // Set full screen or windowed mode
    void setFullScreen( bool fullscreen );

    // Enable/disable v-sync
    void enableVSync( bool enable );

    // Display error massage
    void displayErrorMsg( const std::string & title, const std::string & msg );

    // Set window title
    void setWindowTitle( const std::string & title );

    // Close out the game pads
    void closeGamepads();

    // Init current gamepads plugged in at startup
    void initStartupGamepads();

    // Add/Remove the game pad
    void addGamepad( int id );
    void removeGamepad( int id );

    // Get the gamepad count
    size_t getGamepadCount();

    // Get window
    SDL_Window * getWindow();

    // Wait for Vulkan render to finish
    void waitForIdle();

    // Get descriptor data map
    const std::map< const std::string, CDescriptorData > & getDescriptorDataMap() const;

    // Get descriptor data map
    const CDescriptorData & getDescriptorData( const std::string & id ) const;

    // Delete a uniform buffer vec
    void deleteUniformBufferVec( std::vector<CMemoryBuffer> & commandBufVec );

    // Begin the recording of the command buffer
    void beginCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer );

    // End the recording of the command buffer
    void endCommandBuffer( VkCommandBuffer cmdBuffer );

    // Set/Get last pipeline used
    void setLastPipeline( VkPipeline lastPipeline );
    VkPipeline getLastPipeline();

    // Create the shared font IBO buffer
    void createSharedFontIBO( std::vector<uint16_t> & iboVec );

    // Get the shared font ibo buffer
    CMemoryBuffer & getSharedFontIBO();
    
    // Get the shared font ibo max indice count
    size_t getSharedFontIBOMaxIndiceCount();
    
    // Free the memory buffer
    void freeMemoryBuffer( CMemoryBuffer & memoryBuffer );

private:

    // Constructor
    CDevice();

    // Destructor
    virtual ~CDevice();

    // Create the surface
    void createSurface() override;

    // Create the pipelines from config file
    void createPipelines( const std::string & filePath );

    // Create the shader
    VkShaderModule createShader( const std::string & filePath );

    // Recreate the pipeline
    void recreatePipelines() override;

    // Delete a texture in a group
    void deleteTextureGroup( const std::string & group );

    // Delete a Descriptor Pool group
    void deleteDescriptorPoolGroup( const std::string & group );

    // Delete a buffer in a group
    void deleteMemoryBufferGroup( const std::string & group );

    // Delete the UBO vector group
    //void deleteUboVecGroup( const std::string & group );

    // Record the command buffers
    void recordCommandBuffers( uint32_t cmdBufIndex );

    // A controlled way to destroy the assets
    void destroyAssets() override;

    // Destroy the swap chain
    void destroySwapChain() override;

private:

    // Record ommand buffer call back function
    std::function<void(uint32_t)> RecordCommandBufferCallback;

    // The window we'll be rendering to
    SDL_Window * m_pWindow;

    // Map of gamepad pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;

    // Map containing a group of command pools
    std::map< const std::string, VkCommandPool > m_commandPoolMap;

    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureMapMap;

    // Map containing a group of descriptor pools
    std::map< const std::string, std::map< const std::string, VkDescriptorPool > > m_descriptorPoolMapMap;

    // Map containing a group of descriptor sets
    //std::map< const std::string, std::map< const std::string, std::vector<VkDescriptorSet> > > m_descriptorSetMapMap;

    // Map containing a group of memory buffer handles
    std::map< const std::string, std::map< const std::string, CMemoryBuffer > > m_memoryBufferMapMap;

    // Map containing a group of uniform buffer handles
    //std::map< const std::string, std::map< const std::string, std::vector<CMemoryBuffer> > > m_uboVecMapMap;

    // Map containing loaded shader module
    std::map< const std::string, VkShaderModule > m_shaderModuleMap;

    // Vector containing data for creating the pipeline
    std::vector< CPipelineData > m_pipelineDataVec;

    // Map containing index to pipeline in vector
    std::map< const std::string, int > m_pipelineIndexMap;

    // Command buffer of sprite objects to be rendered
    std::vector<VkCommandBuffer> m_secondaryCommandBufVec;

    // Map containing ubo information
    std::map< const std::string, CUboData > m_uboDataMap;

    // Map containing descriptor information for descriptor creation
    std::map< const std::string, CDescriptorData > m_descriptorDataMap;

    // Map containing descriptor set layouts
    std::map< const std::string, VkDescriptorSetLayout > m_descriptorSetLayoutMap;

    // Map containing pipeline layouts
    std::map< const std::string, VkPipelineLayout > m_pipelineLayoutMap;

    // Keep track of the last pipeline used
    VkPipeline m_lastPipeline = VK_NULL_HANDLE;

    // Current dynamic font IBO indices size
    size_t m_currentMaxFontIndices = 0;

    // Shared font IBO
    CMemoryBuffer m_sharedFontIbo;
};

#endif  // __device_h__


