
/************************************************************************
*    FILE NAME:       commonstate.cpp
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <strategy/strategymanager.h>
#include <gui/menumanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCommonState::CCommonState( const NStateDefs::EGameState gameState, const NStateDefs::EGameState nextState ) :
    iGameState( gameState, nextState )
{
    m_gameStateMap.emplace( "title_screen_state", NStateDefs::EGS_TITLE_SCREEN );
    m_gameStateMap.emplace( "level_1_state", NStateDefs::EGS_LEVEL_1 );
}

CCommonState::CCommonState( const NStateDefs::EGameState gameState, const CStateMessage & stateMsg ) :
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
    if( rEvent.type == NMenuEvent::GAME_STATE_CHANGE )
    {
        if( rEvent.user.code == NTransCode::BEGIN )
        {
            // Block all message processing in the menu manager
            CMenuMgr::Instance().allow( false );

            // Set the message to load and unload the states
            m_stateMessage.setMsg( getLoadState( rEvent.user.data1 ), m_gameState );
        }
    }
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CCommonState::update()
{
    m_scriptComponent.update();
    
    if( !CMenuMgr::Instance().isActive() || m_gameState == NStateDefs::EGS_TITLE_SCREEN )
        CStrategyMgr::Instance().update();

    CMenuMgr::Instance().update();
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CCommonState::transform()
{
    CStrategyMgr::Instance().transform();
    CMenuMgr::Instance().transform();
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CCommonState::recordCommandBuffer( uint32_t cmdBufIndex )
{
    CStrategyMgr::Instance().recordCommandBuffer( cmdBufIndex );
    CMenuMgr::Instance().recordCommandBuffer( cmdBufIndex );

    CStrategyMgr::Instance().updateSecondaryCmdBuf( cmdBufIndex );
    CMenuMgr::Instance().updateSecondaryCmdBuf( cmdBufIndex );
}


/***************************************************************************
*    DESC:  Get the load state
****************************************************************************/
NStateDefs::EGameState CCommonState::getLoadState( void * pVoid )
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
