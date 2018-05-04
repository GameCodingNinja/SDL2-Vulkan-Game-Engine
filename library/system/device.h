
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

// SDL lib dependencies
#include <SDL.h>

// Standard lib dependencies
#include <string>
#include <map>

class CDevice
{
public:

    // Get the instance
    static CDevice & Instance()
    {
        static CDevice device;
        return device;
    }

    // Create the window and OpenGL context
    void create();

    // Show/Hide the Window
    void showWindow( bool visible );

    // Get the OpenGl Context
    SDL_GLContext getContext();

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

private:

    // Constructor
    CDevice();

    // Destructor
    virtual ~CDevice();
    
    // Init current gamepads plugged in at startup
    void initStartupGamepads();

private:

    // The window we'll be rendering to
    SDL_Window * m_pWindow;

    // OpenGL context
    SDL_GLContext m_context;

    // Vector of gamepage pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;

};

#endif  // __device_h__

