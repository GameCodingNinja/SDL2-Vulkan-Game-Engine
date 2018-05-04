
/************************************************************************
*    FILE NAME:       smartresolutionbtn.cpp
*
*    DESCRIPTION:     Class CSmartResBtn
************************************************************************/

// Physical component dependency
#include "smartresolutionbtn.h"

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <system/device.h>
#include <gui/uibuttonlist.h>
#include <gui/uicontrol.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartResolutionBtn::CSmartResolutionBtn( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl ),
    m_resIndex(-1)
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is created - Set the resolution strings
****************************************************************************/
void CSmartResolutionBtn::Create()
{
    SDL_DisplayMode curMode;
    SDL_GetCurrentDisplayMode(0, &curMode);
    std::vector< CSize<int> > resVec;

    // Get the current display size from the settings
    const CSize<int> gameSize = CSettings::Instance().GetSize();

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
        m_pUIControl->SetStringToList( boost::str( boost::format("%d x %d") % iter.w % iter.h ) );

        m_resVec.push_back( iter );

        if( gameSize == iter )
            m_resIndex = counter;

        ++counter;
    }

}   // Create


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartResolutionBtn::HandleEvent( const SDL_Event & rEvent )
{
    if( (m_resIndex > -1) && (rEvent.type == NMenu::EGE_MENU_TRANS_IN) )
        (*dynamic_cast<CUIButtonList *>(m_pUIControl)).UpdateDisplay( m_resIndex );

}   // HandleEvent


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartResolutionBtn::Execute()
{
    EnableDisableApplyBtn();

}   // Execute


/***************************************************************************
*    decs:  Set the resolution change
****************************************************************************/
void CSmartResolutionBtn::SetResolutionChange()
{
    m_resIndex = (*dynamic_cast<CUIButtonList *>(m_pUIControl)).GetActiveIndex();

    CSettings::Instance().SetSize( m_resVec[m_resIndex] );
    CSettings::Instance().CalcRatio();
    CDevice::Instance().CreateProjMatrix();

}   // SetResolutionChange


/***************************************************************************
*    decs:  Was the resolution changed
****************************************************************************/
bool CSmartResolutionBtn::WasResolutionChanged()
{
    if( m_resIndex != (*dynamic_cast<CUIButtonList *>(m_pUIControl)).GetActiveIndex() )
        return true;

    return false;

}   // WasResolutionChanged

