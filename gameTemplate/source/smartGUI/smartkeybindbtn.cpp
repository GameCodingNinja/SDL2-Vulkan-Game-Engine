
/************************************************************************
*    FILE NAME:       smartkeybindbtn.cpp
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

// Physical component dependency
#include "smartkeybindbtn.h"

// Game lib dependencies
#include <gui/icontrol.h>
#include <gui/uisubcontrol.h>
#include <common/defs.h>
#include <managers/actionmanager.h>
#include <utilities/genfunc.h>

// Standard lib dependencies
#include <string>
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartKeyBindBtn::CSmartKeyBindBtn( iControl * piControl ) :
    CSmartGuiControl( piControl )
{
}


/***************************************************************************
*    DESC:  Called when the control is created
*           Sets the action ID string for the given device
****************************************************************************/
void CSmartKeyBindBtn::create()
{
    uint disableCounter(0);
    const std::string actionNameStr = m_piControl->getStringVec().back();
    
    CUISubControl * pSubControl = NGenFunc::DynCast<CUISubControl>(m_piControl);
    
    for( uint i = 0; i < NDefs::MAX_UNIQUE_DEVICES; ++i )
    {
        std::string idStr;
        bool configurable;
        
        // Get the pointer to the sub control label associated with the device
        CUIControl * pControl = pSubControl->getSubControl(i);
    
        // If the ID is defined in the controller mapping XML, set it's string Id
        if( CActionMgr::Instance().getDeviceActionStr(NDefs::EDeviceId(i), actionNameStr, idStr, configurable) )
        {
            pControl->createFontString( idStr );
            
            if( !configurable )
            {
                disableCounter++;
                pControl->setState( NUIControl::ECS_DISABLED );
            }
        }
        else
        {
            disableCounter++;
            pControl->setState( NUIControl::ECS_DISABLED );
            pControl->createFontString( "NA" );
        }
    }
    
    // If all 3 device types are not configuable, disable the button
    if( disableCounter == NDefs::MAX_UNIQUE_DEVICES )
        m_piControl->setState( NUIControl::ECS_DISABLED );
}


/***************************************************************************
*    DESC:  Called when the control is executed
****************************************************************************/
void CSmartKeyBindBtn::execute()
{
    // Disable all action checking so that most buttons can be key mapped
    // without being acted on
    CActionMgr::Instance().enableAction(false);
}


/***************************************************************************
*    DESC:  Handle events
****************************************************************************/
void CSmartKeyBindBtn::handleEvent( const SDL_Event & rEvent )
{
    if( m_piControl->isSelected() && 
        ((rEvent.type == SDL_KEYUP) ||
         (rEvent.type == SDL_MOUSEBUTTONUP) ||
         (rEvent.type == SDL_CONTROLLERBUTTONUP)) )
    {
        // Check for escape to disable key mapping process and save changes
        if( (rEvent.type == SDL_KEYUP) && (rEvent.key.keysym.sym == SDLK_ESCAPE) )
        {
            // Re-enable action checking
            CActionMgr::Instance().enableAction(true);
            
            // Dispatch a message to clear the selected control and put it back into active state
            NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_REACTIVATE );
        }
        else
        {
            std::string idStr;
            const std::string actionNameStr = m_piControl->getStringVec().back();
            NDefs::EDeviceId deviceId = CActionMgr::Instance().resetAction( rEvent, actionNameStr, idStr );

            if( deviceId != NDefs::DEVICE_NULL )
            {
                // Get the sub control holding the string
                CUISubControl * pSubControl = NGenFunc::DynCast<CUISubControl>(m_piControl);
                CUIControl * pControl = pSubControl->getSubControl( deviceId );

                // Reset the string Id
                pControl->createFontString( idStr );
                
                // Save the key binding changes to file
                CActionMgr::Instance().saveToFile();
                
                // Re-enable action checking
                CActionMgr::Instance().enableAction(true);

                // Dispatch a message to clear the selected control and put it back into active state
                NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_REACTIVATE );
            }
        }
    }
}
