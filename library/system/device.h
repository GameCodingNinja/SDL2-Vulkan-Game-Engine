
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Interface class to Vulkan API
************************************************************************/

#ifndef __device_h__
#define __device_h__

// Physical component dependency
#include <system/devicevulkan.h>

// Game dependencies
#include <common/memorybuffer.h>
#include <common/vertex.h>

// SDL lib dependencies
#include <SDL.h>

// Standard lib dependencies
#include <functional>

// Forward declaration(s)
struct SDL_Window;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;

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
    void create( std::function<void(uint32_t)> callback, const std::string & vertShader, const std::string & fragShader );
    
    // Destroy the window and Vulkan instance
    void destroy();
    
    // Update the command buffer vector
    void updateCommandBuffer( VkCommandBuffer cmdBuf );
    
    // Render the frame
    void render();
    
    // Create secondary command buffers
    std::vector<VkCommandBuffer> createSecondaryCommandBuffers( const std::string & group );
    
    // Create descriptor sets
    std::vector<VkDescriptorSet> createDescriptorSet(
        const std::string & group,
        CTexture & texture,
        std::vector<CMemoryBuffer> & uniformBufVec,
        VkDeviceSize sizeOfUniformBuf );
    
    // Load the image from file path
    CTexture & createTexture( const std::string & group, const std::string & filePath, bool mipMap = false );
    
    // Create group assets
    void createGroupAssets( const std::string & group );
    
    // Delete group assets
    void deleteGroupAssets( const std::string & group );
    
    // Load a buffer into video card memory
    template <typename T>
    CMemoryBuffer & loadBuffer( const std::string & group, const std::string & name, std::vector<T> dataVec, VkBufferUsageFlagBits bufferUsageFlag )
    {
        // Create the map group if it doesn't already exist
        auto mapIter = m_memoryBufferMapMap.find( group );
        if( mapIter == m_memoryBufferMapMap.end() )
            mapIter = m_memoryBufferMapMap.emplace( group, std::map<const std::string, CMemoryBuffer>() ).first;

        // See if this texture has already been loaded
        auto iter = mapIter->second.find( name );

        // If it's not found, load the texture and add it to the list
        if( iter == mapIter->second.end() )
        {
            CMemoryBuffer memoryBuffer;
            
            // Load buffer into video memory
            loadVKBuffer( dataVec, memoryBuffer, bufferUsageFlag );

            // Insert the new texture info
            iter = mapIter->second.emplace( name, memoryBuffer ).first;
        }

        return iter->second;
    }
    
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

private:
    
    // Constructor
    CDevice();

    // Destructor
    virtual ~CDevice();
    
    // Create the surface
    void createSurface() override;
    
    // Create the command pool group for command buffer generation
    void createCommandPoolGroup( const std::string & group );
    
    // Create the descriptor pool group for the textures
    void createDescriptorPoolGroup( const std::string & group );
    
    // Delete a texture in a group
    void deleteTextureGroup( const std::string & group );
    
    // Delete a Descriptor Pool group
    void deleteDescriptorPoolGroup( const std::string & group );
    
    // Delete a buffer in a group
    void deleteMemoryBufferGroup( const std::string & group );
    
    // Delete the command pool group
    void deleteCommandPoolGroup( const std::string & group );
    
    // Record the command buffers
    void recordCommandBuffers( uint32_t cmdBufIndex );
    
    // A controlled way to destroy the assets
    void destroyAssets() override;
    
    // Get the number of textures in this group
    size_t getTextureGroupCount( const std::string & group );
    
    // Update the uniform buffer
    void updateUniformBuffer( uint32_t unfBufIndex );
    
    
    
    // Test functions
    void createTextureImage();
    void createVertexBuffer();
    void createCommandPool();
    void recordTestCommandBuffers( uint32_t cmdBufIndex );
    
private:
    
    // Command buffer call back function
    std::function<void(uint32_t)> gameCmdBufferUpdateCallback;
    
    // The window we'll be rendering to
    SDL_Window * m_pWindow;
    
    // Map of gamepad pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;
    
    // Map containing a group of command pools
    std::map< const std::string, VkCommandPool > m_commandPoolMap;
    
    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureMapMap;
    
    // Map containing a group of descriptor pools
    std::map< const std::string, VkDescriptorPool > m_descriptorPoolMap;
    
    // Map containing a group of memory handles
    std::map< const std::string, std::map< const std::string, CMemoryBuffer > > m_memoryBufferMapMap;
    
    // Command buffer of sprite objects to be rendered
    std::vector<VkCommandBuffer> m_secondaryCommandBufVec;
    
    
    // Test code members
    std::vector<VkCommandBuffer> m_squareCmdBufVec;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
    int lastCmdIndex = -1;
    // Descriptor Set for this image
    std::vector<VkDescriptorSet> m_descriptorSetVec;
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;

};

#endif  // __device_h__


