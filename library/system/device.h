
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Interface class to Vulkan API
************************************************************************/

#pragma once

// Physical component dependency
#include <system/devicevulkan.h>

// Standard lib dependencies
#include <system/descriptorallocator.h>
#include <common/size.h>
#include <common/color.h>

// Standard lib dependencies
#include <functional>
#include <string>
#include <vector>
#include <map>

// SDL lib dependencies
#include <SDL3/SDL.h>

// Forward declaration(s)
struct SDL_Window;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;
class SPipelineData;
class SDescriptorData;
class SUboData;
class CPushDescriptorSet;
class CTexture;
class CModel;
class CMeshBinaryFileHeader;
struct SDL_RWops;

class CDevice : public CDeviceVulkan
{
public:

    // Get the instance
    static CDevice & Instance()
    {
        static CDevice device;
        return device;
    }

    // Init the device
    void init( std::function<void(uint32_t)> callback );

    // Create the window and Vulkan instance
    void create( const std::string & pipelineCfg );

    // Destroy the window and Vulkan instance
    void destroy();

    // Update the command buffer vector
    void updateSecondaryCmdBuf( VkCommandBuffer cmdBuf );

    // Render the frame
    void render();

    // Create secondary command buffers
    std::vector<VkCommandBuffer> createSecondaryCommandBuffers( const std::string & group );
    
    // Create the command pool group
    VkCommandPool createSecondaryCommandPool( const std::string & group );

    // Create push descriptor set
    void createPushDescriptorSet(
        uint32_t pipelineIndex,
        const CTexture & texture,
        const std::vector<CMemoryBuffer> & uniformBufVec,
        CPushDescriptorSet & pushDescSet );

    // Get the descriptor sets
    CDescriptorSet * getDescriptorSet(
        int pipelineIndex,
        const CTexture & texture,
        const std::vector<CMemoryBuffer> & uniformBufVec );

    // Recycle the descriptor set
    void recycleDescriptorSet( CDescriptorSet * pDescriptorSet );

    // Load the image from file path
    CTexture & createTexture( const std::string & group, CTexture & rTexture );

    // Create uniform buffer
    std::vector<CMemoryBuffer> createUniformBufferVec( uint32_t pipelineIndex );

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

            // Insert the buffer into the map
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
    const SPipelineData & getPipelineData( int index ) const;

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

    // Init the sensors
    void initSensors();

    // Close out the game pads
    void closeGamepads();

    // Init current gamepads plugged in at startup
    void initStartupGamepads();

    // Add/Remove the game pad
    void addGamepad( SDL_JoystickID id );
    void removeGamepad( SDL_JoystickID id );

    // Get the gamepad count
    std::size_t getGamepadCount();

    // Get window
    SDL_Window * getWindow();

    // Wait for Vulkan render to finish
    void waitForIdle();

    // Get descriptor data map
    const std::map< const std::string, SDescriptorData > & getDescriptorDataMap() const;

    // Get descriptor data map
    const SDescriptorData & getDescriptorData( const std::string & id ) const;

    // Begin the recording of the command buffer
    void beginCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer );

    // End the recording of the command buffer
    void endCommandBuffer( VkCommandBuffer cmdBuffer );

    // Create the shared font IBO buffer
    void createSharedFontIBO( std::vector<uint16_t> & iboVec );

    // Get the shared font ibo buffer
    CMemoryBuffer & getSharedFontIBO();
    
    // Get the shared font ibo max indice count
    std::size_t getSharedFontIBOMaxIndiceCount();
    
    // Add a memory buffer to the delete queue
    void AddToDeleteQueue( CMemoryBuffer & memoryBuffer );
    void AddToDeleteQueue( std::vector<CMemoryBuffer> & commandBufVec );
    
    // Create a model
    void createModel(
        const std::string & group,
        const std::string & filePath,
        CModel & model );
    
    // Is the transfer queue unique to allow for loading and rendering at the same time?
    bool isTransferQueueUnique();

    // Get the number of frames since the start of the game
    uint32_t getFrameCounter();

    // Change the resolution
    void changeResolution( const CSize<float> & size, bool fullScreen );

    // Set the clear color
    void setClearColor( float r, float g, float b, float a );

    // Connect to the viewport signal
    void connectViewportSignal( const deviceViewportSignal_t::slot_type & slot );

    // Disconnect all signal slots
    void disconnect();

    // Delete a secondary command buffer of a specific group
    //void deleteCommandBuffer( const std::string & group, std::vector<VkCommandBuffer> & commandBufVec );

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

    // Delete the texture in a group
    void deleteTextureGroup( const std::string & group );

    // Delete the memory buffer group
    void deleteMemoryBufferGroup( const std::string & group );
    
    // Delete the model group
    void deleteModelGroup( const std::string & group );

    // Add a delete function to the delete map
    void AddToDeleteQueue( std::function<void(VkDevice logicalDevice)> deleteFunc );
    void AddToDeleteQueue( CTexture & texture );

    // Record the command buffers
    void recordCommandBuffers( uint32_t cmdBufIndex );

    // A controlled way to destroy the assets
    void destroyAssets() override;

    // Destroy the swap chain
    void destroySwapChain() override;
    
    // Load 3d mesh file
    void loadFrom3DM(
        const std::string & group,
        const std::string & filePath,
        CModel & model );
    
    // Load 3d mesh file with textures
    void load3DM(
        SDL_IOStream * pFile,
        const CMeshBinaryFileHeader & fileHeader,
        const std::string & group,
        const std::string & filePath,
        CModel & model );
    
    // Do the tag check to insure we are in the correct spot
    void tagCheck( SDL_IOStream * file, const std::string & filePath );

    // Allocate the descriptor pool and first sets
    CDescriptorSet * allocateDescriptorPoolSet(
        std::map< const std::string, CDescriptorAllocator >::iterator & allocIter,
        const CTexture & texture,
        const std::vector<CMemoryBuffer> & uniformBufVec,
        const SPipelineData & rPipelineData,
        const SDescriptorData & rDescData );

    // Update the descriptor set
    void updateDescriptorSet(
        CDescriptorSet * pDescriptorSet,
        int pipelineIndex,
        const CTexture & texture,
        const std::vector<CMemoryBuffer> & uniformBufVec );

    // Handle memory operations based on frame counter
    void frameCounterMemoryOperations();

    // Handle the resolution change
    virtual void handleResolutionChange( int width, int height ) override;

    // Select the VkCompareOp mode
    VkCompareOp selectVkCompareOp( const std::string modeStr, VkCompareOp defaultValue );

    // Select the VkStencilOp mode
    VkStencilOp selectVkStencilOp( const std::string modeStr, VkStencilOp defaultValue );

private:

    // Record ommand buffer call back function
    std::function<void(uint32_t)> RecordCommandBufferCallback;

    // The window we'll be rendering to
    SDL_Window * m_pWindow;

    // Map of gamepad pointers
    std::map<int, SDL_Gamepad *> m_pGamepadMap;

    // Map containing a group of command pools
    std::map< const std::string, VkCommandPool > m_commandPoolMap;

    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureMapMap;

    // Map containing a group of descriptor allocator
    std::map< const std::string, CDescriptorAllocator > m_descriptorAllocatorMap;

    // Map containing a group of memory buffer handles
    std::map< const std::string, std::map< const std::string, CMemoryBuffer > > m_memoryBufferMapMap;

    // Map containing loaded shader module
    std::map< const std::string, VkShaderModule > m_shaderModuleMap;

    // Vector containing data for creating the pipeline
    std::vector< SPipelineData > m_pipelineDataVec;

    // Map containing index to pipeline in vector
    std::map< const std::string, int > m_pipelineIndexMap;

    // Command buffer of sprite objects to be rendered
    std::vector<VkCommandBuffer> m_secondaryCommandBufVec;

    // Map containing ubo information
    std::map< const std::string, SUboData > m_uboDataMap;

    // Map containing descriptor information for descriptor creation
    std::map< const std::string, SDescriptorData > m_descriptorDataMap;

    // Map containing descriptor set layouts
    std::map< const std::string, VkDescriptorSetLayout > m_descriptorSetLayoutMap;

    // Map containing pipeline layouts
    std::map< const std::string, VkPipelineLayout > m_pipelineLayoutMap;

    // Vector for deleting VK memory buffers
    std::map< uint32_t, std::vector<std::function<void(VkDevice logicalDevice)>> > m_memoryDeleteMap;
    
    // Map containing a group array of vbo, ibo and texture id's
    std::map< const std::string, std::map< const std::string, CModel > > m_modelMapMap;

    // Current dynamic font IBO indices size
    std::size_t m_currentMaxFontIndices = 0;

    // Shared font IBO
    CMemoryBuffer m_sharedFontIbo;

    // counter that increments for each frame
    uint32_t m_frameCounter = 0;

    // The current frame
    size_t m_currentFrame = 0;

    // The clear color
    CColor m_clearColor;
};
