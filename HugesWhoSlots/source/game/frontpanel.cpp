
/************************************************************************
*    FILE NAME:       frontpanel.cpp
*
*    DESCRIPTION:     Game specific class container to manage buttons and meters.
*                     Does NOT own the pointers. Do Not Free
************************************************************************/

// Physical component dependency
#include "frontpanel.h"

// Game lib dependencies
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <2d/sprite2d.h>
#include <objectdata/objectdatamanager.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CFrontPanel::CFrontPanel() :
    m_pPlayBtn(nullptr),
    m_pWinMeter(nullptr),
    m_pCreditMeter(nullptr)
{
}


/************************************************************************
*    DESC:  Set the buttons
************************************************************************/
void CFrontPanel::setButtons( CUIButton * pPlayBtn, std::vector<CUIControl *> & pOtherBtnVec )
{
    m_pPlayBtn = pPlayBtn;
    m_pOtherBtnVec = pOtherBtnVec;
}


/************************************************************************
*    DESC:  Set the meters
************************************************************************/
void CFrontPanel::setMeters( CUIMeter * pWinMeter, CUIMeter * pCreditMeter )
{
    m_pWinMeter = pWinMeter;
    m_pCreditMeter = pCreditMeter;
}


/************************************************************************
*    DESC:  Init a new game
************************************************************************/
void CFrontPanel::initGame( uint credits )
{
    if( m_pWinMeter != nullptr )
        m_pWinMeter->clear();

    if( m_pCreditMeter != nullptr )
        m_pCreditMeter->set( credits );

    for( auto iter : m_pOtherBtnVec )
        iter->disableControl();
}


/************************************************************************
*    DESC:  Start the bang up
************************************************************************/
void CFrontPanel::startBangUp( uint win, uint credits )
{
    if( m_pWinMeter != nullptr )
        m_pWinMeter->startBangUp( win );

    if( m_pCreditMeter != nullptr )
        m_pCreditMeter->startBangUp( credits );
}


/************************************************************************
*    DESC:  Are the meters banging
************************************************************************/
bool CFrontPanel::isBanging()
{
    bool result(false);

    if( m_pWinMeter != nullptr )
        result |= m_pWinMeter->isBanging();

    if( m_pCreditMeter != nullptr )
        result |= m_pCreditMeter->isBanging();

    return result;
}


/************************************************************************
*    DESC:  Start the fast bang
************************************************************************/
void CFrontPanel::fastBang()
{
    if( m_pWinMeter != nullptr )
        m_pWinMeter->fastBang();

    if( m_pCreditMeter != nullptr )
        m_pCreditMeter->fastBang();
}


/************************************************************************
*    DESC:  Enable the buttons
************************************************************************/
void CFrontPanel::enableButtons( bool allowPlay )
{
    for( auto iter : m_pOtherBtnVec )
        iter->enableControl();

    if( (m_pPlayBtn != nullptr) && allowPlay )
        m_pPlayBtn->callSpriteScriptFuncKey( "playReady", true );
}
