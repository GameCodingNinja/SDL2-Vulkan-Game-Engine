
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Interface class to Vulkan API
************************************************************************/

#ifndef __device_h__
#define __device_h__

// Physical component dependency
#include <system/devicevulkan.h>

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
    NVulkan::CTexture & loadTexture( const std::string & group, const std::string & filePath, bool mipMap = false );
    
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
    void createTextureImage() override;
    
private:
    
    // The window we'll be rendering to
    SDL_Window * m_pWindow;
    
    // Map of gamepad pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;
    
    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, NVulkan::CTexture > > m_textureMapMap;

};

#endif  // __device_h__


