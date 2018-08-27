
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
    
    // Create the window and OpenGL context
    void create( const std::string & vertShader, const std::string & fragShader );
    
    // Destroy the window and Vulkan instance
    void destroy();
    
    // Load the image from file path
    CTexture & loadTexture( const std::string & group, const std::string & filePath, bool mipMap = false );
    
    // Create the descriptor pool
    VkDescriptorPool createDescriptorPool( const std::string & group, size_t setCount );
    
    // Get the number of textures in this group
    size_t getTextureGroupCount( const std::string & group );
    
    // Delete a texture in a group
    void deleteTextureGroup( const std::string & group );
    
    // Show/Hide the Window
    void showWindow( bool visible );
    
    // Set full screen or windowed mode
    void setFullScreen( bool fullscreen );

    // Enable/disable v-sync
    void enableVSync( bool enable );
    
    // Render the frame
    void render();
    
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
    
    // Load a buffer into video card memory
    template <typename T>
    CMemoryBuffer & loadBuffer( const std::string & group, const std::string & name, std::vector<T> dataVec, VkBufferUsageFlagBits bufferUsageFlag )
    {
        // Create the map group if it doesn't already exist
        auto mapMapIter = m_bufferMapMap.find( group );
        if( mapMapIter == m_bufferMapMap.end() )
            mapMapIter = m_bufferMapMap.emplace( group, std::map<const std::string, CMemoryBuffer>() ).first;

        // See if this texture has already been loaded
        auto mapIter = mapMapIter->second.find( name );

        // If it's not found, load the texture and add it to the list
        if( mapIter == mapMapIter->second.end() )
        {
            CMemoryBuffer memoryBuffer;
            
            // Load buffer into video memory
            loadVKBuffer( dataVec, memoryBuffer, bufferUsageFlag );

            // Insert the new texture info
            mapIter = mapMapIter->second.emplace( name, memoryBuffer ).first;
        }

        return mapIter->second;
    }

private:
    
    // Constructor
    CDevice();

    // Destructor
    virtual ~CDevice();
    
    // Create the surface
    void createSurface() override;
    
    // A controlled way to destroy the assets
    void destroyAssets() override;

    // Create texture image
    void createTextureImage();
    
    // Create the vertex buffer
    void createVertexBuffer();
    
private:
    
    // The window we'll be rendering to
    SDL_Window * m_pWindow;
    
    // Map of gamepad pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;
    
    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureMapMap;
    
    // Map containing a group of descriptor pools
    std::map< const std::string, VkDescriptorPool > m_descriptorPoolMap;
    
    // Map containing a group of memory handles
    std::map< const std::string, std::map< const std::string, CMemoryBuffer > > m_bufferMapMap;

};

#endif  // __device_h__


