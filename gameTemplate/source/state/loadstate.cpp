
/************************************************************************
*    FILE NAME:       loadstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "loadstate.h"

// Game dependencies
#include "statedefs.h"
#include "titlescreenstate.h"
#include "level1state.h"

// Game lib dependencies
#include <system/device.h>
#include <objectdata/objectdatamanager.h>
#include <managers/actionmanager.h>
#include <script/scriptmanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <strategy/strategymanager.h>
#include <strategy/strategy.h>
#include <strategy/strategyloader.h>
#include <common/color.h>
#include <sprite/sprite.h>
#include <node/inode.h>

// Standard lib dependencies
#include <thread>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CLoadState::CLoadState( const CStateMessage & stateMsg ) :
    CCommonState( NStateDefs::EGS_GAME_LOAD, stateMsg )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CLoadState::~CLoadState()
{
    CStrategyMgr::Instance().deleteStrategy( "_load_" );
    CDevice::Instance().deleteCommandPoolGroup( "(load)" );
    CObjectDataMgr::Instance().freeGroup( "(load)" );
}


/************************************************************************
*    DESC:  Do any pre-load init's
************************************************************************/
void CLoadState::init()
{
    // Load group specific assets
    CObjectDataMgr::Instance().loadGroup( "(load)" );

    // Load the Strategy
    CStrategyloader::Instance().load( "data/objects/strategy/state/loadscreen.loader" );
    
    // Start the fade in and animation
    m_scriptComponent.prepare( "(state)", "State_FadeIn" );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CLoadState::handleEvent( const SDL_Event & rEvent )
{
    // Event sent from script
    if( rEvent.type == NStateDefs::ESE_FADE_IN_COMPLETE )
    {
        std::thread load(&CLoadState::assetLoad, this);
        load.detach();
    }
    // Event sent from script
    else if( rEvent.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
    {
        // Set the flag to change the state
        m_changeState = true;
    }
    // Event sent from thread
    else if( rEvent.type == NStateDefs::ESE_THREAD_LOAD_COMPLETE )
    {
        m_scriptComponent.prepare( "(state)", "State_FadeOut" );
    }
}


/***************************************************************************
*    DESC:  Load the assets
****************************************************************************/
void CLoadState::assetLoad()
{
    try
    {
        if( m_stateMessage.getLoadState() == NStateDefs::EGS_TITLE_SCREEN )
            CTitleScreenState::load();

        else if( m_stateMessage.getLoadState() == NStateDefs::EGS_LEVEL_1 )
            CLevel1State::load();

        NGenFunc::DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
    }
    catch( NExcept::CCriticalException & ex )
    {
        m_errorTitle = ex.getErrorTitle();
        m_errorMsg = ex.getErrorMsg();
    }
    catch( std::exception const & ex )
    {
        m_errorTitle = "Standard Exception";
        m_errorMsg = ex.what();
    }
    catch(...)
    {
        m_errorTitle = "Unknown Error";
        m_errorMsg = "Something bad happened and I'm not sure what it was.";
    }
}


/***************************************************************************
*    DESC:  Is the state done
****************************************************************************/
bool CLoadState::doStateChange()
{
    // Re-throw any threaded exceptions
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException( m_errorTitle, m_errorMsg );

    return m_changeState;
}


/***************************************************************************
*    DESC:  Get the next state to load
****************************************************************************/
NStateDefs::EGameState CLoadState::getNextState()
{
    return m_stateMessage.getLoadState();
}
