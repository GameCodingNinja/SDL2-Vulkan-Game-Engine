
/************************************************************************
*    FILE NAME:       loadstate.cpp
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
#include "loadstate.h"

// Standard lib dependencies
#include <string>
#include <vector>
#include <thread>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <system/device.h>
#include <objectdata/objectdatamanager.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <2d/sprite2d.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <physics/physicsworldmanager.h>

// Game dependencies
#include "loadscreenanimationinfo.h"
//#include "../game/savefile.h"

/************************************************************************
*    desc:  Constructer
************************************************************************/
CLoadState::CLoadState( const CStateMessage & stateMsg )
{
    // Set the game state for this object
    m_gameState = EGS_GAME_LOAD;

    // Copy over the message to react to
    m_stateMessage = stateMsg;

}   // Constructer


/***************************************************************************
*    desc:  Animate from thread durring the load
****************************************************************************/
void CLoadState::Animate()
{
    m_exitThread = false;

    try
    {
        const std::string group("(loadingScreen)");
        
        // Create the OpenGL context just for this thread.
        SDL_GLContext context = SDL_GL_CreateContext( CDevice::Instance().GetWindow() );
        if( context == nullptr )
            throw NExcept::CCriticalException("OpenGL context could not be created in load screen thread!", SDL_GetError() );

        m_mutex.lock();

        // Load everything needed for this OpenGL context. OpenGL contexts only see what is in it's calling thread
        CShaderMgr::Instance().LoadFromXML( "data/shaders/load_shader.cfg" );
        CObjectDataMgr::Instance().LoadGroup2D(group);

        m_mutex.unlock();

        // Need the projection matrix
        const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );

        // Load the animation data
        std::vector<CLoadScrnAnim> animDataVec;
        LoadAnimData(
            group,
            animDataVec,
            XMLNode::openFileHelper( "data/objects/2d/loadscreens/gameLoadScreen.cfg", "loadscreen" ) );
        
        // This function was written for onlt one animation
        if( animDataVec.size() > 1 )
            throw NExcept::CCriticalException("Loading screen error", "More animations loaded then designed to handle!" );

        // Calc fps
        const float time = 1000.0 / animDataVec.back().GetFPS();
        float timer = -1;

        int counter(0);
        do
        {
            // Get the elapsed time
            CHighResTimer::Instance().CalcElapsedTime();

            timer -= CHighResTimer::Instance().GetElapsedTime();

            if( timer < 0 )
            {
                // Clear the screen
                glClear( GL_COLOR_BUFFER_BIT );

                animDataVec.back().GetSprite()->GetVisualComponent().SetTextureID( counter );
                animDataVec.back().GetSprite()->Render( matrix );

                SDL_GL_SwapWindow( CDevice::Instance().GetWindow() );

                // Apparently it's a good practice to do this at the end of a render cycle
                CShaderMgr::Instance().UnbindShaderProgram();
                CTextureMgr::Instance().UnbindTexture();
                CVertBufMgr::Instance().UnbindBuffers();

                counter = (counter + 1) % animDataVec.back().GetFrameCount();

                timer = time;
            }

            SDL_Delay( 2 );
        }
        while( !m_exitThread );

        // Unload the assets
        CObjectDataMgr::Instance().FreeGroup2D(group);
        CShaderMgr::Instance().FreeShader( "load_shader_2d" );

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
        NGenFunc::PostDebugMsg( "Unknown error when animating loading screen" );
        
        m_mutex.unlock();
    }

}   // Animate


/************************************************************************
*    desc:  Init the animation load data
************************************************************************/
void CLoadState::LoadAnimData(
    const std::string & group,
    std::vector<CLoadScrnAnim> & animDataVec,
    const XMLNode & node )
{
    animDataVec.reserve( node.nChildNode() );
    
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        XMLNode animNode = node.getChildNode( "anim", i );

        animDataVec.emplace_back();

        // Allocate the sprite
        animDataVec.back().AllocateSprite(
            CObjectDataMgr::Instance().GetData2D( group, animNode.getAttribute( "name" ) ) );

        // Get the position, scale and half the screen size
        CPoint<float> pos = NParseHelper::LoadPosition( animNode );
        CPoint<float> scale = NParseHelper::LoadScale( animNode );
        CSize<float> scrnHalf = CSettings::Instance().GetDefaultSizeHalf();

        // Set the position
        animDataVec.back().GetSprite()->SetPos( CPoint<float>(scrnHalf.w, -scrnHalf.h, 0) + pos );

        // Set the scale
        animDataVec.back().GetSprite()->SetScale( scale );

        // One time transform because we are not moving it anywhere
        animDataVec.back().GetSprite()->Transform();

        // Get the animation info
        animDataVec.back().SetFrameCount( std::atoi(animNode.getAttribute( "count" )) );
        animDataVec.back().SetFPS( std::atoi(animNode.getAttribute( "fps" )) );
    }

}   // LoadAnimData


/***************************************************************************
*    desc:  Load from thread during the loading screen
****************************************************************************/
void CLoadState::Load()
{
    // Start the animated loading thread
    std::thread animThread( &CLoadState::Animate, this );


    // ----------------- UNLOAD ASSETS SECTION -----------------

    // Unload any data groups
    for( auto & iter : m_stateMessage.m_groupUnload )
        CObjectDataMgr::Instance().FreeGroup2D( iter );

    if( !m_stateMessage.m_physicsWorldDestroy.empty() )
        CPhysicsWorldManager::Instance().DestroyWorld2D( m_stateMessage.m_physicsWorldDestroy );


    // ------------------ LOAD ASSETS SECTION ------------------

    // Load any data groups
    m_mutex.lock();
    
    for( auto & iter : m_stateMessage.m_groupLoad )
        CObjectDataMgr::Instance().LoadGroup2D( iter );
    
    m_mutex.unlock();
    
    if( !m_stateMessage.m_physicsWorldCreate.empty() )
        CPhysicsWorldManager::Instance().CreateWorld2D( m_stateMessage.m_physicsWorldCreate );
    
    
    // for testing
    SDL_Delay( 1000 );


    // Tell the thread it's time to finish
    m_exitThread = true;

    // Wait for the thread to finish
    animThread.join();


    /*
    // Free the stages
    CStage2D::Instance().ClearStage();

    // Free all the actor data
    CActorManager::Instance().ClearAll();

    // Clear out the actor data
    CActorDataList::Instance().Clear();

    // Free the instance mesh data
    CInstanceMeshManager::Instance().Clear();

    // Free the hud
    CHudManager::Instance().Clear();

    // Clear all the Mega Textures
    CMegaTextureManager::Instance().Clear();

    // Clear all the generators
    CGeneratorMgr::Instance().Clear();

    // Unload any data groups
    for( size_t i = 0; i < stateMessage.groupUnload.size(); ++i )
        CObjectDataList2D::Instance().FreeDataGroup( stateMessage.groupUnload[i] );


    // Load any data groups
    for( size_t i = 0; i < stateMessage.groupLoad.size(); ++i )
        CObjectDataList2D::Instance().LoadDataGroup( stateMessage.groupLoad[i] );

    // Create any mega textures
    for( size_t i = 0; i < stateMessage.createMegaTexture.size(); ++i )
        CMegaTextureManager::Instance().CreateMegaTexture(
            stateMessage.createMegaTexture[i].get<0>(),
            stateMessage.createMegaTexture[i].get<1>() );

    // Create any instance Meshes
    for( size_t i = 0; i < stateMessage.createInstanceMesh.size(); ++i )
        CInstanceMeshManager::Instance().CreateInstanceMesh(
            stateMessage.createInstanceMesh[i] );

    // Load up the actor data list
    if( !stateMessage.actorDataLoad.empty() )
        CActorDataList::Instance().LoadFromXML( stateMessage.actorDataLoad );

    // Create any actors
    for( size_t i = 0; i < stateMessage.createActor.size(); ++i )
        CActorManager::Instance().CreateActor2D( stateMessage.createActor[i] );

    // Create any actor vectors
    for( size_t i = 0; i < stateMessage.createActorVec.size(); ++i )
        CActorManager::Instance().CreateActorVec2D( stateMessage.createActorVec[i] );

    // Load the 2d stage
    if( !stateMessage.stage2DLoad.empty() )
        CStage2D::Instance().LoadStage( stateMessage.stage2DLoad );

    // Create the generators
    if( !stateMessage.generatorLoadPath.empty() )
    {
        // Get the position of the player as the new focus position
        CGeneratorMgr::Instance().CreateGenerators(
            stateMessage.generatorLoadPath,
            CActorManager::Instance().GetActor2D( "player_ship" )->GetPos().GetIntComponent() );
    }

    // Save the game file
    if( (stateMessage.nextState == EGS_TITLE_SCREEN) &&
        (stateMessage.lastState == EGS_RUN))
    {
        CSaveFile::Instance().Save();
    }*/

}   // Load


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CLoadState::DoStateChange()
{
    Load();

    return true;

}   // DoStateChange


/***************************************************************************
*    desc:  Get the next state to load
****************************************************************************/
EGameState CLoadState::GetNextState()
{
    return m_stateMessage.m_nextState;

}   // GetNextState





