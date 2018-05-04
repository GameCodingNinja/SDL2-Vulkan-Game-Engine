
/************************************************************************
*    FILE NAME:       smartkeybindbtn.cpp
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

// Physical component dependency
#include "smartkeybindbtn.h"

// Standard lib dependencies
#include <string>
#include <vector>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <gui/uisubcontrol.h>
#include <common/defs.h>
#include <managers/actionmanager.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartKeyBindBtn::CSmartKeyBindBtn( CUIControl * pUIControl ) :
    CSmartGuiControl( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is created
*           Sets the action ID string for the given device
****************************************************************************/
void CSmartKeyBindBtn::Create()
{
    uint disableCounter(0);
    const std::string actionNameStr = m_pUIControl->GetStringVec().back();
    
    CUISubControl * pSubControl = NGenFunc::DynCast<CUISubControl>(m_pUIControl);
    
    for( uint i = 0; i < NDefs::MAX_UNIQUE_DEVICES; ++i )
    {
        std::string idStr;
        bool configurable;
        
        // Get the pointer to the sub control label associated with the device
        CUIControl * pControl = pSubControl->GetSubControl(i);
    
        // If the ID is defined in the controller mapping XML, set it's string Id
        if( CActionMgr::Instance().GetDeviceActionStr(NDefs::EDeviceId(i), actionNameStr, idStr, configurable) )
        {
            pControl->CreateFontString( idStr );
            
            if( !configurable )
            {
                disableCounter++;
                pControl->SetState( NUIControl::ECS_DISABLED );
            }
        }
        else
        {
            disableCounter++;
            pControl->SetState( NUIControl::ECS_DISABLED );
            pControl->CreateFontString( "NA" );
        }
    }
    
    // If all 3 device types are not configuable, disable the button
    if( disableCounter == NDefs::MAX_UNIQUE_DEVICES )
        m_pUIControl->SetState( NUIControl::ECS_DISABLED );

}   // Create


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartKeyBindBtn::Execute()
{
    // Disable all action checking so that most buttons can be key mapped
    // without being acted on
    CActionMgr::Instance().EnableAction(false);
    
}   // Execute


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartKeyBindBtn::HandleEvent( const SDL_Event & rEvent )
{
    if( m_pUIControl->IsSelected() && 
        ((rEvent.type == SDL_KEYUP) ||
         (rEvent.type == SDL_MOUSEBUTTONUP) ||
         (rEvent.type == SDL_CONTROLLERBUTTONUP)) )
    {
        // Check for escape to disable key mapping process and save changes
        if( (rEvent.type == SDL_KEYUP) && (rEvent.key.keysym.sym == SDLK_ESCAPE) )
        {
            // Re-enable action checking
            CActionMgr::Instance().EnableAction(true);
            
            // Dispatch a message to clear the selected control and put it back into active state
            NGenFunc::DispatchEvent( NMenu::EGE_MENU_REACTIVATE );
        }
        else
        {
            std::string idStr;
            const std::string actionNameStr = m_pUIControl->GetStringVec().back();
            NDefs::EDeviceId deviceId = CActionMgr::Instance().ResetAction( rEvent, actionNameStr, idStr );

            if( deviceId != NDefs::DEVICE_NULL )
            {
                // Get the sub control holding the string
                CUISubControl * pSubControl = NGenFunc::DynCast<CUISubControl>(m_pUIControl);
                CUIControl * pControl = pSubControl->GetSubControl( deviceId );

                // Reset the string Id
                pControl->CreateFontString( idStr );
                
                // Save the key binding changes to file
                CActionMgr::Instance().SaveToFile();
                
                // Re-enable action checking
                CActionMgr::Instance().EnableAction(true);

                // Dispatch a message to clear the selected control and put it back into active state
                NGenFunc::DispatchEvent( NMenu::EGE_MENU_REACTIVATE );
            }
        }
    }
    
}   // HandleEvent
