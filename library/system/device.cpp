
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Class used for window and openGL management
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <system/device.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <common/size.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CDevice::CDevice() :
    m_pWindow(nullptr),
    m_context(nullptr)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CDevice::~CDevice()
{
}


/***************************************************************************
*   DESC:  Create the window and OpenGL context
 ****************************************************************************/
void CDevice::create()
{
    // Initialize SDL - The File I/O and Threading subsystems are initialized by default.
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER ) < 0 )
        throw NExcept::CCriticalException("SDL could not initialize!", SDL_GetError() );

    // All file I/O is handled by SDL and SDL_Init must be called before doing any I/O.
    CSettings::Instance().loadXML();

    // Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, CSettings::Instance().getMajorVersion() );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, CSettings::Instance().getMinorVersion() );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, CSettings::Instance().getProfile() );

    // Do we add stencil buffer
    if( CSettings::Instance().getCreateStencilBuffer() )
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, CSettings::Instance().getStencilBufferBitSize() );

    // Get the window size
    const CSize<int> size( CSettings::Instance().getSize() );

    // Create window
    m_pWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.getW(), size.getH(), SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN );
    if( m_pWindow == nullptr )
        throw NExcept::CCriticalException("Game window could not be created!", SDL_GetError() );

    // Create context
    m_context = SDL_GL_CreateContext( m_pWindow );
    if( m_context == nullptr )
        throw NExcept::CCriticalException("OpenGL context could not be created!", SDL_GetError() );

    #if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    uint32_t glewError = glewInit();
    if( glewError != GLEW_OK )
        throw NExcept::CCriticalException("Error initializing GLEW!",
            boost::str( boost::format("Error initializing GLEW (%s).\n\n%s\nLine: %s")
                % glewGetErrorString( glewError ) % __FUNCTION__ % __LINE__ ));

    // Enable/disable v-sync
    enableVSync( CSettings::Instance().getVSync() );

    #else

    // Enable/disable v-sync
    CSettings::Instance().setVSync( true );
    enableVSync( true );

    #endif

    // Set the full screen
    if( CSettings::Instance().getFullScreen() )
        setFullScreen( CSettings::Instance().getFullScreen() );

    // Depth testing is off by default. Enable it?
    if( CSettings::Instance().getEnableDepthBuffer() )
        glEnable(GL_DEPTH_TEST);

    // Init current gamepads plugged in at startup
    initStartupGamepads();
}


/***************************************************************************
*   DESC:  Enable/disable v-sync
 ****************************************************************************/
void CDevice::enableVSync( bool enable )
{
    if( SDL_GL_SetSwapInterval( (enable == true) ? 1 : 0 ) < 0 )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set VSync! SDL GL Error: %s") % SDL_GetError() ) );
}


/***************************************************************************
*   DESC:  Show/Hide the Window
 ****************************************************************************/
void CDevice::showWindow( bool visible )
{
    if( visible )
        SDL_ShowWindow( m_pWindow );
    else
        SDL_HideWindow( m_pWindow );
}


/***************************************************************************
*   DESC:  Set full screen or windowed mode
 ****************************************************************************/
void CDevice::setFullScreen( bool fullscreen )
{
    int flag(0);

    if( fullscreen )
        flag = SDL_WINDOW_FULLSCREEN;

    if( SDL_SetWindowFullscreen( m_pWindow, flag ) < 0 )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set full screen! SDL GL Error: %s") % SDL_GetError() ) );
}


/***************************************************************************
*   DESC:  Get the OpenGl Context
 ****************************************************************************/
SDL_GLContext CDevice::getContext()
{
    return m_context;
}


/***************************************************************************
*   DESC:  Get the SDL window
 ****************************************************************************/
SDL_Window * CDevice::getWindow()
{
    return m_pWindow;
}


/***************************************************************************
*   DESC:  Init current gamepads plugged in at startup
 ****************************************************************************/
void CDevice::initStartupGamepads()
{
    // May not need this anymore
    //int newMappings = SDL_GameControllerAddMappingsFromFile("data/settings/gamecontrollerdb.txt");
    //NGenFunc::PostDebugMsg( boost::str( boost::format("New controller mappings found: %d - Number of controllers found: %d") % newMappings % (int)SDL_NumJoysticks() ) );

    for( int i = 0; i < SDL_NumJoysticks(); ++i )
        addGamepad( i );
}


/***************************************************************************
*   DESC:  Add/Remove the game pad
 ****************************************************************************/
void CDevice::addGamepad( int id )
{
    if( SDL_IsGameController( id ) )
    {
        SDL_GameController * pGamePad = SDL_GameControllerOpen(id);
        if( pGamePad != NULL )
        {
            //NGenFunc::PostDebugMsg( boost::str( boost::format("Game controller added: %d - %s") % id % SDL_GameControllerNameForIndex(id) ) );
            m_pGamepadMap.emplace( id, pGamePad );
        }
    }
}

void CDevice::removeGamepad( int id )
{
    auto iter = m_pGamepadMap.find( id );
    if( iter != m_pGamepadMap.end() )
    {
        SDL_GameControllerClose( iter->second );
        m_pGamepadMap.erase( iter );
    }
}


/***************************************************************************
*   DESC:  Get the gamepad count
 ****************************************************************************/
size_t CDevice::getGamepadCount()
{
    return m_pGamepadMap.size();
}
