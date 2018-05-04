
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include "game.h"

// Game lib dependencies
#include <common/build_defs.h>
#include <system/device.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <strategy/strategymanager.h>
#include <managers/actionmanager.h>
#include <managers/cameramanager.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/meshmanager.h>
#include <gui/menumanager.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptplaylist.h>
#include <script/scriptpoint.h>
#include <script/scriptsize.h>
#include <script/scriptglobals.h>
#include <script/scriptisprite.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptcamera.h>
#include <script/scriptcameramanager.h>
#include <script/scriptmenu.h>
#include <script/scriptshadermanager.h>
#include <script/scriptobjectdatamanager.h>
#include <script/scriptstrategymanager.h>
#include <script/scriptistrategy.h>
#include <script/scriptactionmanager.h>
#include <script/scriptsettings.h>
#include <script/scripthighresolutiontimer.h>
#include <script/scriptmenumanager.h>
#include <script/scriptfontmanager.h>
#include <script/scriptscriptmanager.h>
#include <script/scriptuicontrol.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scriptmath/scriptmath.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGame::CGame()
    : m_pWindow(nullptr),
      m_context(nullptr),
      m_gameRunning(false),
      m_clearBufferMask(0)
{
    if( NBDefs::IsDebugMode() )
        CStatCounter::Instance().connect( boost::bind(&CGame::statStringCallBack, this, _1) );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CGame::~CGame()
{
    // Destroy the OpenGL context
    if( m_context != nullptr )
        SDL_GL_DeleteContext( m_context );

    // Destroy window
    if( m_pWindow != nullptr )
        SDL_DestroyWindow( m_pWindow );

    // Quit SDL subsystems
    SDL_Quit();
}


/***************************************************************************
*   DESC:  Create the game Window
 ****************************************************************************/
void CGame::create()
{
    // Create the window and OpenGL context
    CDevice::Instance().create();

    // Get local copies of the device handles
    m_pWindow = CDevice::Instance().getWindow();
    m_context = CDevice::Instance().getContext();

    // Game start init
    init();
}


/************************************************************************
*    DESC:  OpenGL Init
************************************************************************/
void CGame::openGLInit()
{
    // Init the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Init the stencil clear mask based on the bit size of the mask
    // Stencil buffer can only be 1 or 8 bits per pixel
    if( CSettings::Instance().getStencilBufferBitSize() == 1 )
        glStencilMask(0x1);
    else if( CSettings::Instance().getStencilBufferBitSize() == 8 )
        glStencilMask(0xff);

    // Cull the back face
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Make the zero texture the active texture
    glActiveTexture(GL_TEXTURE0);

    // Init the clear buffer mask
    if( CSettings::Instance().getClearTargetBuffer() )
        m_clearBufferMask |= GL_COLOR_BUFFER_BIT;

    if( CSettings::Instance().getEnableDepthBuffer() )
        m_clearBufferMask |= GL_DEPTH_BUFFER_BIT;

    if( CSettings::Instance().getClearStencilBuffer() )
        m_clearBufferMask |= GL_STENCIL_BUFFER_BIT;

    if( CSettings::Instance().getEnableDepthBuffer() )
        glEnable( GL_DEPTH_TEST );

    // Clear the back buffer and flip it prior to showing the window
    // Keeps us from seeing a flash or flicker of pre init junk
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( m_pWindow );

    // Show the window
    CDevice::Instance().showWindow( true );

    // Display a black screen
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( m_pWindow );
}


/************************************************************************
*    DESC:  Init the game
************************************************************************/
void CGame::init()
{
    openGLInit();

    // Handle some events on startup
    pollEvents();

    // Load the script list table
    CScriptMgr::Instance().loadListTable( "data/scripts/scriptListTable.lst" );

    // Register the script items
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptArray( CScriptMgr::Instance().getEnginePtr(), false );
    RegisterScriptDictionary( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptMath( CScriptMgr::Instance().getEnginePtr() );
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptSize::Register();
    NScriptCamera::Register();
    NScriptiSprite::Register();
    NScriptSound::Register();
    NScriptPlayLst::Register();
    NScriptiStrategy::Register();
    NScriptSoundManager::Register();
    NScriptShaderManager::Register();
    NScriptObjectDataManager::Register();
    NScriptStrategyManager::Register();
    NScriptCameraManager::Register();
    NScriptActionManager::Register();
    NScriptSettings::Register();
    NScriptHighResolutionTimer::Register();
    NScriptUIControl::Register();
    NScriptMenu::Register();
    NScriptMenuManager::Register();
    NScriptFontManager::Register();
    NScriptScriptManager::Register();

    CScriptMgr::Instance().loadGroup("(main)");
    CScriptMgr::Instance().prepare("(main)", "main");

    CHighResTimer::Instance().calcElapsedTime();

    // Let the games begin
    startGame();
}


/***************************************************************************
*   DESC:  Poll for game events
****************************************************************************/
void CGame::pollEvents()
{
    // Event handler
    SDL_Event msgEvent;

    CActionMgr::Instance().clearQueue();

    // Handle events on queue
    while( SDL_PollEvent( &msgEvent ) )
    {
        CActionMgr::Instance().queueEvent( msgEvent );

        // let the game handle the event
        // turns true on quit
        if( handleEvent( msgEvent ) )
        {
            // Stop the game
            m_gameRunning = false;

            // Hide the window to give the impression of a quick exit
            CDevice::Instance().showWindow( false );

            break;
        }
    }
}


/***************************************************************************
*   DESC:  Main game loop
****************************************************************************/
bool CGame::gameLoop()
{
    // Poll for game events
    pollEvents();

    // Get our elapsed time
    CHighResTimer::Instance().calcElapsedTime();

    // Main script update
    CScriptMgr::Instance().update();

    if( m_gameRunning )
    {
        // Clear the buffers
        glClear( m_clearBufferMask );

        // Process all game states
        CStrategyMgr::Instance().miscProcess();
        CStrategyMgr::Instance().update();
        CMenuMgr::Instance().update();
        CStrategyMgr::Instance().transform();
        CCameraMgr::Instance().transform();
        CStrategyMgr::Instance().render();

        // Do the back buffer swap
        SDL_GL_SwapWindow( m_pWindow );

        // Unbind everything after a round of rendering
        CShaderMgr::Instance().unbind();
        CTextureMgr::Instance().unbind();
        CVertBufMgr::Instance().unbind();

        // Inc the cycle
        if( NBDefs::IsDebugMode() )
            CStatCounter::Instance().incCycle();
    }

    return m_gameRunning;
}


/************************************************************************
*    DESC:  Callback for the state string
************************************************************************/
void CGame::statStringCallBack( const std::string & statStr )
{
    if( !CSettings::Instance().getFullScreen() )
        SDL_SetWindowTitle( m_pWindow, statStr.c_str() );
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
bool CGame::handleEvent( const SDL_Event & rEvent )
{
    if( (rEvent.type == SDL_QUIT) || (rEvent.type == SDL_APP_TERMINATING) )
        return true;
    
    // Handle events for the menu manager
    CMenuMgr::Instance().handleEvent( rEvent );
    
    // Check for the "game change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Block all message processing in the menu manager
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            CMenuMgr::Instance().allow( false );

        // Clear out all the trees
        else if( rEvent.user.code == NMenu::ETC_END )
            CMenuMgr::Instance().clearActiveTrees();
    }

    // Filter out these events. Can't do this through the normal event filter
    if( (rEvent.type >= SDL_JOYAXISMOTION) && (rEvent.type <= SDL_JOYBUTTONUP) )
        return false;

    else if( rEvent.type == SDL_CONTROLLERDEVICEADDED )
        CDevice::Instance().addGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_CONTROLLERDEVICEREMOVED )
        CDevice::Instance().removeGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_APP_LOWMEMORY )
        displayErrorMsg( "Low Memory Error", "The device is experiencing low memory. Try freeing up some apps." );

    // In a traditional game, want the pause menu to display when the game is sent to the background
    else if( (rEvent.type == SDL_APP_WILLENTERBACKGROUND) && !CMenuMgr::Instance().isMenuActive() )
        NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION );

    return false;
}


/***************************************************************************
*   DESC:  Display error massage
****************************************************************************/
void CGame::displayErrorMsg( const std::string & title, const std::string & msg )
{
    printf("Error: %s, %s", title.c_str(), msg.c_str() );

    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), m_pWindow );
}


/***************************************************************************
*   DESC:  Start the game
****************************************************************************/
void CGame::startGame()
{
    m_gameRunning = true;
}


/***************************************************************************
*   DESC:  Stop the game
****************************************************************************/
void CGame::stopGame()
{
    m_gameRunning = false;
}


/***************************************************************************
*  DESC:  Is the game running?
*
*  ret: bool - true or false if game is running
****************************************************************************/
bool CGame::isGameRunning() const
{
    return m_gameRunning;
}


/***************************************************************************
*    decs:  Filter out events we don't want
****************************************************************************/
int FilterEvents( void * userdata, SDL_Event * pEvent )
{
    // Return 0 to indicate that the event should be removed from the event queue

    // Do our own deadzone filtering
    if( pEvent->type == SDL_CONTROLLERAXISMOTION )
    {
        // Analog stick max values -32768 to 32767
        const int deadZone = CSettings::Instance().getGamePadStickDeadZone() *
            defs_ANALOG_PERCENTAGE_CONVERTION;

        if( ((pEvent->caxis.axis >= SDL_CONTROLLER_AXIS_LEFTX) &&
            (pEvent->caxis.axis <= SDL_CONTROLLER_AXIS_RIGHTY)) &&
            (((pEvent->caxis.value >= 0) && (pEvent->caxis.value < deadZone)) ||
            ((pEvent->caxis.value <= 0) && (pEvent->caxis.value > -deadZone))) )
            return 0;
    }

    // Return 1 to indicate that the event should stay in the event queue
    return 1;
}
