
/************************************************************************
*    FILE NAME:       commonstate.cpp
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

// Physical component dependency
#include "commonstate.h"

// SDL lib dependencies
#include <SDL.h>

// Game lib dependencies
#include <gui/menumanager.h>
#include <system/device.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCommonState::CCommonState()
{
}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCommonState::~CCommonState()
{

}   // Destructer


/***************************************************************************
*    desc:  handle events
****************************************************************************/
void CCommonState::HandleEvent( const SDL_Event & rEvent )
{
    // Have the menu manager handle events
    CMenuManager::Instance().HandleEvent( rEvent );

}   // HandleEvent


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::Update()
{
    // Update the menus
    CMenuManager::Instance().Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CCommonState::Transform()
{
    // Transform the menus
    CMenuManager::Instance().Transform();

}   // Transform


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PostRender()
{
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    CMenuManager::Instance().Render( matrix );

}   // PostRender2D
