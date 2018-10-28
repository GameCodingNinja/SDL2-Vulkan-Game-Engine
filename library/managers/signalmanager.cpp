
/************************************************************************
*    FILE NAME:       signalmanager.cpp
*
*    DESCRIPTION:     Class for handling messaging
************************************************************************/

// Physical component dependency
#include <managers/signalmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <common/defs.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <sprite/sprite.h>

// SDL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSignalMgr::CSignalMgr()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSignalMgr::~CSignalMgr()
{
}


/************************************************************************
*    DESC:  Connect/Disconnect to the smart gui control signal
************************************************************************/
void CSignalMgr::connect_smartGui( const SmartGuiControlSignal::slot_type & slot )
{
    m_smartGuiControlSignal.connect(slot);
}

void CSignalMgr::disconnect_smartGui()
{
    m_smartGuiControlSignal.disconnect_all_slots();
}


/************************************************************************
*    DESC:  Connect/Disconnect to the smart menu signal
************************************************************************/
void CSignalMgr::connect_smartMenu( const SmartMenuSignal::slot_type & slot )
{
    m_smartMenuSignal.connect(slot);
}

void CSignalMgr::disconnect_smartMenu()
{
    m_smartMenuSignal.disconnect_all_slots();
}


/************************************************************************
*    DESC:  Connect/Disconnect to the Ai create signal
************************************************************************/
void CSignalMgr::connect_aICreate( const AICreateSignal::slot_type & slot )
{
    m_aiCreateSignal.connect(slot);
}

void CSignalMgr::disconnect_aICreate()
{
    m_aiCreateSignal.disconnect_all_slots();
}


/************************************************************************
*    DESC:  Connect/Disconnect to the resolution change signal
************************************************************************/
void CSignalMgr::connect_resolutionChange( const BasicFunction::slot_type & slot )
{
    m_resolutionChange.connect(slot);
}

void CSignalMgr::disconnect_resolutionChange()
{
    m_resolutionChange.disconnect_all_slots();
}


/************************************************************************
*    DESC:  Broadcast smart gui control signal
************************************************************************/
void CSignalMgr::broadcast( CUIControl * pControl )
{
    m_smartGuiControlSignal(pControl);
}


/************************************************************************
*    DESC:  Broadcast smart menu signal
************************************************************************/
void CSignalMgr::broadcast( CMenu * pMenu )
{
    m_smartMenuSignal(pMenu);
}


/************************************************************************
*    DESC:  Broadcast AI Sprite create signal
************************************************************************/
void CSignalMgr::broadcast( const std::string & aiName, CSprite * pSprite )
{
    m_aiCreateSignal(aiName, pSprite);
}


/************************************************************************
*    DESC:  Broadcast the resolution change
************************************************************************/
void CSignalMgr::broadcast_ResolutionChange()
{
    m_resolutionChange();
}
