
/************************************************************************
*    FILE NAME:       smartresetkeybind.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartresetkeybind.h"

// Game lib dependencies
#include <managers/actionmanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <gui/uiscrollbox.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartResetKeyBindBtn::CSmartResetKeyBindBtn( CUIControl * pUIControl ) :
    CSmartGuiControl( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed - quits the game
****************************************************************************/
void CSmartResetKeyBindBtn::Execute()
{
    // Reset the key bindings for all controls and save
    CActionMgr::Instance().ResetKeyBindingsToDefault();
    
    // Get a pointer to the scroll box
    CMenu & rMenu = CMenuManager::Instance().GetMenu( "key_bindings_menu" );
    CUIScrollBox * pScrollBoxCtrl = NGenFunc::DynCast<CUIScrollBox>(rMenu.GetPtrToControl("key_binding_scroll_box"));
    
    // Reset all the key binding buttons
    const auto & scrollCtrlVec = pScrollBoxCtrl->GetScrollCtrlVec();
    for( auto iter : scrollCtrlVec )
        iter->SmartCreate();

}   // Execute







