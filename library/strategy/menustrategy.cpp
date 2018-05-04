
/************************************************************************
*    FILE NAME:       menustrategy.cpp
*
*    DESCRIPTION:     Menu strategy
************************************************************************/

// Physical component dependency
#include <strategy/menustrategy.h>

// Game lib dependencies
#include <managers/cameramanager.h>
#include <gui/menumanager.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenuStrategy::CMenuStrategy()
{
}


/************************************************************************
*    DESC:  Transform the menu
************************************************************************/
void CMenuStrategy::transform()
{
    CMenuMgr::Instance().transformMenu();
}


/***************************************************************************
*    DESC:  Render the menu
****************************************************************************/
void CMenuStrategy::render()
{
    const auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    CMenuMgr::Instance().renderMenu( camera.getFinalMatrix() );
}
