
/************************************************************************
*    FILE NAME:       smartconfirmbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartconfirmbtn.h"

// Game dependencies
#include "smartresetkeybind.h"

// Game lib dependencies
#include <gui/icontrol.h>
#include <gui/menumanager.h>
#include <gui/menu.h>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartConfirmBtn::CSmartConfirmBtn( iControl * piControl ) :
    CSmartGuiControl( piControl )
{
}


/***************************************************************************
*    DESC:  Called when the control is executed
****************************************************************************/
void CSmartConfirmBtn::execute()
{
    CMenu & rMenu = CMenuMgr::Instance().getMenu("confirmation_menu");
    iControl * pYesBtn = rMenu.getPtrToControl("yes_btn");
    iControl * pMegLbl = rMenu.getPtrToControl("message_lbl");

    CSmartGuiControl * pSmartGuiCtrl(NULL);
    std::string conformationMsg;
    std::string executionAction;
    NUIControlDefs::EControlActionType actionType(NUIControlDefs::ECAT_BACK);

    if( m_piControl->getName() == "exit_btn" )
    {
        actionType = NUIControlDefs::ECAT_QUIT_GAME;
        conformationMsg = "Do you want to|quit the game?";
    }
    else if( m_piControl->getName() == "main_menu_btn" )
    {
        conformationMsg = "Are you sure you|want to go back to|the main menu?";
        actionType = NUIControlDefs::ECAT_GAME_STATE_CHANGE;
        executionAction = "title_screen_state";
    }
    else if( m_piControl->getName() == "Key_Binding_reset_btn" )
    {
        pSmartGuiCtrl = new CSmartResetKeyBindBtn( pYesBtn );
        conformationMsg = "Reset all key bindings|to their default settings?";
    }
    else if( m_piControl->getName() == "continue_btn" )
    {
        conformationMsg = "Are you sure you|want to continue|on to the next state?";
        actionType = NUIControlDefs::ECAT_GAME_STATE_CHANGE;
        executionAction = "level_1_state";
    }
    /*else if( m_pUIControl->GetName().find(".game_save") != string::npos )
    {
        // Find the file name save no. deliminator
        size_t dashPos = m_pUIControl->GetName().find("_");

        // Get the save no.
        std::string saveNo = pUIControl->GetName().substr(0, dashPos);

        pSmartGuiCtrl = new CSmartNewGameSaveBtn( pYesBtn );
        conformationMsg = "Do you want to|overwrite Save " + saveNo + " with|the new game save file?";

        // Set the control name which is the file name to overwrite.
        executionAction = pUIControl->GetName();

        // Set the action type to kill the menu
        actionType = NUIControlDefs::ECAT_DEACTIVATE;
    }*/

    // Set the conformation menu
    pYesBtn->setSmartGui( pSmartGuiCtrl );
    pYesBtn->setActionType( actionType );
    pYesBtn->setExecutionAction( executionAction );
    pMegLbl->createFontString( conformationMsg );
}
