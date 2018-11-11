
/************************************************************************
*    FILE NAME:       smartresolutionbtn.cpp
*
*    DESCRIPTION:     Class CSmartResBtn
************************************************************************/

// Physical component dependency
#include "smartresolutionbtn.h"

// Game lib dependencies
#include <managers/cameramanager.h>
#include <gui/uibuttonlist.h>
#include <gui/uicontrol.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <string>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartResolutionBtn::CSmartResolutionBtn( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl ),
    m_resIndex(-1)
{
}


/***************************************************************************
*    decs:  Called when the control is created - Set the resolution strings
****************************************************************************/
void CSmartResolutionBtn::create()
{
    SDL_DisplayMode curMode;
    SDL_GetCurrentDisplayMode(0, &curMode);
    std::vector< CSize<int> > resVec;

    // Get the current display size from the settings
    const CSize<int> gameSize = CSettings::Instance().getResolution();

    int displayCount = SDL_GetNumDisplayModes(0);
    for( int i = 0; i < displayCount; i++ )
    {
        SDL_DisplayMode mode;

        if( SDL_GetDisplayMode(0, i, &mode) == 0 )
        {
            //NGenFunc::PostDebugMsg(
            //    boost::str( boost::format("%d, %d - %d x %d") % SDL_BITSPERPIXEL(mode.format) % mode.refresh_rate % mode.w % mode.h ) );

            CSize<int> size(mode.w, mode.h);

            // Keep out any duplicates
            if( std::find(resVec.begin(), resVec.end(), size) == resVec.end() )
                resVec.push_back( size );
        }
    }

    // The resolutions are in greatest to smallest. We need the order reversed
    std::reverse(resVec.begin(), resVec.end());

    int counter(0);
    for( auto & iter : resVec )
    {
        m_pUIControl->setStringToList( boost::str( boost::format("%d x %d") % iter.w % iter.h ) );

        m_resVec.push_back( iter );

        if( gameSize == iter )
            m_resIndex = counter;

        ++counter;
    }
}


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartResolutionBtn::handleEvent( const SDL_Event & rEvent )
{
    if( (m_resIndex > -1) && (rEvent.type == NMenuDefs::EME_MENU_TRANS_IN) )
        (*dynamic_cast<CUIButtonList *>(m_pUIControl)).updateDisplay( m_resIndex );
}


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartResolutionBtn::execute()
{
    enableDisableApplyBtn();
}


/***************************************************************************
*    decs:  Set the resolution change
****************************************************************************/
void CSmartResolutionBtn::setResolutionChange()
{
    m_resIndex = (*dynamic_cast<CUIButtonList *>(m_pUIControl)).getActiveIndex();

    CSettings::Instance().setSize( m_resVec[m_resIndex] );
    CSettings::Instance().calcRatio();
    CCameraMgr::Instance().createProjMatrix();
}


/***************************************************************************
*    decs:  Was the resolution changed
****************************************************************************/
bool CSmartResolutionBtn::wasResolutionChanged()
{
    if( m_resIndex != (*dynamic_cast<CUIButtonList *>(m_pUIControl)).getActiveIndex() )
        return true;

    return false;
}
