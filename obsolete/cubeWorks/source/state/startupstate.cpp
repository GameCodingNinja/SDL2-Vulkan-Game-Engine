
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// SDL lib dependencies
#include <SDL.h>

#if defined(__IPHONEOS__) || defined(__ANDROID__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include "startupstate.h"

// Game lib dependencies
//#include <managers/hudmanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/shadermanager.h>
#include <managers/fontmanager.h>
#include <managers/soundmanager.h>
#include <managers/actionmanager.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdatamanager.h>
#include <common/color.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptpoint.h>
#include <script/scriptglobals.h>
#include <scriptstdstring/scriptstdstring.h>
#include <2d/sprite2d.h>
#include <3d/sprite3d.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/menu.h>
#include <gui/uicontrol.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworldmanager.h>
#include <3d/physicscomponent3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <vector>
#include <thread>

// Game dependencies
//#include "../game/savefile.h"

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStartUpState::CStartUpState()
{
    // Set the game states for this object
    m_gameState = EGS_STARTUP;
    m_nextState = EGS_TITLE_SCREEN;

}   // Constructer


/***************************************************************************
*    desc:  Animate from thread durring the load
****************************************************************************/
void CStartUpState::Animate()
{
    try
    {
        // Create the OpenGL context just for this thread.
        SDL_GLContext context = SDL_GL_CreateContext( CDevice::Instance().GetWindow() );
        if( context == nullptr )
            throw NExcept::CCriticalException("OpenGL context could not be created in startup thread!", SDL_GetError() );

        // Init the start of the animation
        InitAnimation( "(startup)" );
        
        // Animate the fade
        for( auto & iter : m_animDataVec )
        {
            Fade( iter, CColor(0,0,0,0), CColor(1,1,1,1) );

            SDL_Delay( iter.GetDisplayDelay() );

            Fade( iter, CColor(1,1,1,1), CColor(0,0,0,0) );
            
            // Delay between fades
            SDL_Delay( iter.GetEndDelay() );
        }
   
        // Clean up after the animation is completed
        CleanUpAnimation( "(startup)" );
        
        // Context is no longer needed
        SDL_GL_DeleteContext( context );
    }
    catch( NExcept::CCriticalException & ex )
    {
        NGenFunc::PostDebugMsg( ex.GetErrorTitle() + " - " + ex.GetErrorMsg() );
        
        m_mutex.unlock();
    }
    catch( std::exception const & ex )
    {
        NGenFunc::PostDebugMsg( ex.what() );
        
        m_mutex.unlock();
    }
    catch(...)
    {
        NGenFunc::PostDebugMsg( "Unknown error when animating startup screen." );
        
        m_mutex.unlock();
    }

}   // Animate


/***************************************************************************
*    desc:  Init the start of the animation
****************************************************************************/
void CStartUpState::InitAnimation( const std::string & group )
{
    m_mutex.lock();

    // Load everything needed for this OpenGL context. OpenGL contexts only see what is in it's calling thread
    CShaderMgr::Instance().LoadFromXML( "data/shaders/load_shader.cfg" );
    CObjectDataMgr::Instance().LoadGroup2D( group );

    m_mutex.unlock();

    // Load the animation data
    LoadAnimData(
        group,
        m_animDataVec,
        XMLNode::openFileHelper( "data/objects/2d/loadscreens/startupLoadScreen.cfg", "loadscreen" ) );
        
}   // InitAnimation


/***************************************************************************
*    desc:  Clean up after the animation is completed
*           These can only be used once for the unique OpenGL context
****************************************************************************/
void CStartUpState::CleanUpAnimation( const std::string & group )
{
    // Unload the assets
    CObjectDataMgr::Instance().FreeGroup2D(group);
    CShaderMgr::Instance().FreeShader( "load_shader_2d" );
    
}   // CleanUpAnimation


/************************************************************************
*    desc:  Fade to color                                                             
************************************************************************/
void CStartUpState::Fade(
    CLoadScrnAnim & loadScrnAnim,
    const CColor & cur,
    const CColor & finalColor )
{
    float time = loadScrnAnim.GetFadeTime();
    CColor inc = (finalColor - cur) / time;
    CColor current = cur;
    
    // Need the projection matrix
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );

    do
    {
        // Get the elapsed time
        CHighResTimer::Instance().CalcElapsedTime();

        time -= CHighResTimer::Instance().GetElapsedTime();
        current += inc * CHighResTimer::Instance().GetElapsedTime();

        if( time < 0 )
            current = finalColor;

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        loadScrnAnim.GetSprite()->GetVisualComponent().SetColor( current );
        loadScrnAnim.GetSprite()->Render( matrix );

        SDL_GL_SwapWindow( CDevice::Instance().GetWindow() );

        // Apparently it's a good practice to do this at the end of a render cycle
        CShaderMgr::Instance().UnbindShaderProgram();
        CTextureMgr::Instance().UnbindTexture();
        CVertBufMgr::Instance().UnbindBuffers();

        SDL_Delay( 2 );
    }
    while( time > 0 );

}   // FadeTo


/************************************************************************
*    desc:  Init the animation load data                                                             
************************************************************************/
void CStartUpState::LoadAnimData(
    const std::string & group,
    std::vector<CLoadScrnAnim> & animDataVec,
    const XMLNode & node )
{
    animDataVec.reserve( node.nChildNode() );
    
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        XMLNode fadeNode = node.getChildNode( "fade", i );

        animDataVec.emplace_back();
        
        // Allocate the sprite
        animDataVec.back().AllocateSprite(
            CObjectDataMgr::Instance().GetData2D( group, fadeNode.getAttribute( "name" ) ) );
          
        // One time transform because we are not moving it anywhere
        animDataVec.back().GetSprite()->Transform();

        // Get the animation info
        animDataVec.back().SetFadeTime( std::atoi(fadeNode.getAttribute( "fadeTime" )) );
        animDataVec.back().SetDisplayDelay( std::atoi(fadeNode.getAttribute( "displayDelay" )) );
        animDataVec.back().SetEndDelay( std::atoi(fadeNode.getAttribute( "endDelay" )) );
    }
    
}   // LoadAnimData


/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CStartUpState::Load()
{
    // Load all of the meshes and materials in these groups
    CObjectDataMgr::Instance().LoadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
    
    // Android doesn't support multiple OpenGLES2 contexes
    #if defined(__ANDROID__)
    // Do the asynchronous load
    AsynchronousLoad();
    #else
    // Do the synchronous load
    SynchronousLoad();
    #endif

}   // Load


/***************************************************************************
*    desc:  Load the assets while animating in a thread
****************************************************************************/
void CStartUpState::SynchronousLoad()
{
    // Start the animated loading thread
    std::thread animThread( &CStartUpState::Animate, this );

    std::string errorTitle;
    std::string errorMsg;

    // Trap exception to allow animation thread to return. =
    // Otherwise it will assert and block the exception information
    // Then re-throw exception.
    try
    {
        // Load the assets
        AssetLoad();
    }
    catch( NExcept::CCriticalException & ex )
    {
        errorTitle = ex.GetErrorTitle();
        errorMsg = ex.GetErrorMsg();
    }
    catch( std::exception const & ex )
    {
        errorTitle = "Standard Exception";
        errorMsg = ex.what();
    }
    catch( ... )
    {
        errorTitle = "Unknown Error";
        errorMsg = "Something bad happened and I'm not sure what it was.";
    }

    // Wait for the thread to finish
    animThread.join();

    // If there was an error, re-throw exception
    if( !errorMsg.empty() )
        throw NExcept::CCriticalException( errorTitle, errorMsg );
    
}   // SynchronousLoad


/***************************************************************************
*    desc:  Load the assets with start & end animation
****************************************************************************/
void CStartUpState::AsynchronousLoad()
{
    // Init the start of the animation
    InitAnimation( "(startup)" );
    
    // Do the fade in
    Fade( m_animDataVec.at(0), CColor(0,0,0,0), CColor(1,1,1,1) );
    
    // Load the assets
    AssetLoad();
    
    // Get the elapsed time
    CHighResTimer::Instance().CalcElapsedTime();
    
    // How much time did this take
    float time = CHighResTimer::Instance().GetElapsedTime();
    
    // If this ended too soon then wait for a bit so that it's consistant
    if( time < m_animDataVec.at(0).GetDisplayDelay() )
        SDL_Delay( m_animDataVec.at(0).GetDisplayDelay() - time );
    
    // Do the fade out
    Fade( m_animDataVec.at(0), CColor(1,1,1,1), CColor(0,0,0,0) );
            
    // Delay between fades
    SDL_Delay( m_animDataVec.at(0).GetEndDelay() );

    // Clean up after the animation is completed
    CleanUpAnimation( "(startup)" );

}   // AsynchronousLoad


/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CStartUpState::AssetLoad()
{
    // Load the 3D group list table
    CObjectDataMgr::Instance().LoadListTable( "data/objects/3d/objectDataList/dataListTable.lst" );

    // Load the action manager - Must be loaded before memu system
    CActionMgr::Instance().LoadActionFromXML( "data/settings/controllerMapping.cfg" );
    
    // Load sound resources for the menu
    CSoundMgr::Instance().LoadListTable( "data/sound/soundListTable.lst" );
    CSoundMgr::Instance().LoadGroup("(menu)");
    
    // Load the script list tables
    CScriptManager::Instance().LoadListTable( "data/objects/2d/scripts/scriptListTable.lst" );
    CScriptManager::Instance().LoadListTable( "data/objects/3d/scripts/scriptListTable.lst" );
    
    // Load the physics list table
    CPhysicsWorldManager::Instance().LoadListTable( "data/objects/2d/physics/physicsListTable.lst" );
    CPhysicsWorldManager::Instance().LoadListTable( "data/objects/3d/physics/physicsListTable.lst" );
    
    // Register the script items
    RegisterStdString( CScriptManager::Instance().GetEnginePtr() );
    NScriptGlobals::Register( CScriptManager::Instance().GetEnginePtr() );
    NScriptColor::Register( CScriptManager::Instance().GetEnginePtr() );
    NScriptPoint::Register( CScriptManager::Instance().GetEnginePtr() );
    CSprite2D::Register( CScriptManager::Instance().GetEnginePtr() );
    CSprite3D::Register( CScriptManager::Instance().GetEnginePtr() );
    
    m_mutex.lock();
    
    // Load in any fonts
    CFontMgr::Instance().LoadFromXML( "data/textures/fonts/font.lst" );
    
    // Shaders must always be loaded first because they are accessed from object data
    CShaderMgr::Instance().LoadFromXML( "data/shaders/shader.cfg" );

    // Load all of the meshes and materials in these groups
    CObjectDataMgr::Instance().LoadGroup2D("(menu)");
    CObjectDataMgr::Instance().LoadGroup2D( "(title_screen)" );
    CObjectDataMgr::Instance().LoadGroup3D( "(cube)" );
    
    m_mutex.unlock();

    // Load group specific script items
    CScriptManager::Instance().LoadGroup("(menu)");
    CScriptManager::Instance().LoadGroup("(cube)");

    // Create the menu system
    CMenuManager::Instance().LoadFromXML( "data/objects/2d/menu/tree.list" );

    // Create the physics
    CPhysicsWorldManager::Instance().CreateWorld3D( "(cube)" );

    #if !(defined(__IPHONEOS__) || defined(__ANDROID__))
    // Init the currently plugged in game controllers
    CDevice::Instance().InitStartupGamepads();
    #endif
    
}   // PreMutexLoad


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CStartUpState::DoStateChange()
{
    // load game assets
    Load();

    return true;

}   // DoStateChange

