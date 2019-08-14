
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
#include <gui/icontrol.h>
#include <gui/menu.h>
#include <gui/uiscrollbox.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartResetKeyBindBtn::CSmartResetKeyBindBtn( iControl * piControl ) :
    CSmartGuiControl( piControl )
{
}   // constructor


/***************************************************************************
*    DESC:  Called when the control is executed - quits the game
****************************************************************************/
void CSmartResetKeyBindBtn::execute()
{
    // Reset the key bindings for all controls and save
    CActionMgr::Instance().resetKeyBindingsToDefault();
    
    // Get a pointer to the scroll box
    CMenu & rMenu = CMenuMgr::Instance().getMenu( "key_bindings_menu" );
    CUIScrollBox * pScrollBoxCtrl = NGenFunc::DynCast<CUIScrollBox>(rMenu.getPtrToControl("key_binding_scroll_box"));
    
    // Reset all the key binding buttons
    const auto & scrollCtrlVec = pScrollBoxCtrl->getScrollCtrlVec();
    for( auto iter : scrollCtrlVec )
        iter->smartCreate();
}
