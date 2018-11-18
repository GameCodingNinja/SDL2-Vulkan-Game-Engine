
/************************************************************************
*    FILE NAME:       commonstate.cpp
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <managers/cameramanager.h>
#include <utilities/exceptionhandling.h>
#include <strategy/strategymanager.h>
#include <gui/menumanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCommonState::CCommonState( const NGameDefs::EGameState gameState, const NGameDefs::EGameState nextState ) :
    iGameState( gameState, nextState )
{
    m_gameStateMap.emplace( "title_screen_state", NGameDefs::EGS_TITLE_SCREEN );
    m_gameStateMap.emplace( "run_state", NGameDefs::EGS_RUN );
}

CCommonState::CCommonState( const NGameDefs::EGameState gameState, const CStateMessage & stateMsg ) :
    iGameState( gameState, stateMsg )
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CCommonState::~CCommonState()
{
}


/***************************************************************************
*    DESC:  handle events
****************************************************************************/
void CCommonState::handleEvent( const SDL_Event & rEvent )
{
    // Have the menu manager handle events
    CMenuMgr::Instance().handleEvent( rEvent );

    // Check for the "game change state" message
    if( rEvent.type == NMenuDefs::EME_MENU_GAME_STATE_CHANGE )
    {
        if( rEvent.user.code == NMenuDefs::ETC_BEGIN )
        {
            // Block all message processing in the menu manager
            CMenuMgr::Instance().allow( false );

            // Set the message to load and unload the states
            m_stateMessage.setMsg( getLoadState( rEvent.user.data1 ), m_gameState );
        }
    }
}


/***************************************************************************
*    DESC:  Handle Misc processes
****************************************************************************/
void CCommonState::miscProcess()
{
    CStrategyMgr::Instance().miscProcess();
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CCommonState::update()
{
    m_scriptComponent.update();
    
    if( !CMenuMgr::Instance().isActive() || m_gameState == NGameDefs::EGS_TITLE_SCREEN )
        CStrategyMgr::Instance().update();

    CMenuMgr::Instance().update();
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CCommonState::transform()
{
    if( !CMenuMgr::Instance().isActive() || m_gameState == NGameDefs::EGS_TITLE_SCREEN )
        CStrategyMgr::Instance().transform();

    CMenuMgr::Instance().transform();
    
    CCameraMgr::Instance().transform();
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CCommonState::recordCommandBuffer( uint32_t index )
{
    CStrategyMgr::Instance().recordCommandBuffer( index, CCameraMgr::Instance().getFinalMatrix() );
    CMenuMgr::Instance().recordCommandBuffer( index, CCameraMgr::Instance().getFinalMatrix() );
}


/***************************************************************************
*    DESC:  Get the load state
****************************************************************************/
NGameDefs::EGameState CCommonState::getLoadState( void * pVoid )
{
    if( pVoid == nullptr )
    {
        throw NExcept::CCriticalException("Load state not defined!",
            boost::str( boost::format("Message pointer is null!\n\n%s\nLine: %s")
                % __func__ % __LINE__ ));
    }

    const std::string & rNameStr = *(std::string *)pVoid;
    if( rNameStr.empty() )
    {
        throw NExcept::CCriticalException("Load state not defined!",
            boost::str( boost::format("Need to define a state to load.\n\n%s\nLine: %s")
                % __func__ % __LINE__ ));
    }

    // Find the state to return
    auto iter = m_gameStateMap.find( rNameStr );
    if( iter == m_gameStateMap.end() )
    {
        throw NExcept::CCriticalException("Load state not found!",
            boost::str( boost::format("Load state not defined or incorrectly defined! (%s).\n\n%s\nLine: %s")
                % rNameStr % __func__ % __LINE__ ));
    }

    return iter->second;
}
