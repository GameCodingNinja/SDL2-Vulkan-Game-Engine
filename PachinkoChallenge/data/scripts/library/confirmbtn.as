//
//  FILE NAME:  confirmbtn.as
//

//
//  Execution script for a button control to change to the confirmation menu
//
void ConfirmBtn_execute( uiControl & control )
{
    uiControl @yesBtn = MenuMgr.getMenu( "confirmation_menu" ).getControl( "yes_btn" );
    uiControl @msgLbl = MenuMgr.getMenu( "confirmation_menu" ).getControl( "message_lbl" );
    
    string actionType;
    string executionAction;
    string conformationMsg;
    
    if( control.getName() == "exit_btn" )
    {
        actionType = "quit_game";
        conformationMsg = "Do you want to|quit the game?";
    }
    else if( control.getName() == "continue_btn" )
    {
        actionType = "game_state_change";
        conformationMsg = "Are you sure you|want to continue|on to the next state?";
        executionAction = "level_1_state";
    }
    else if( control.getName() == "main_menu_btn" )
    {
        actionType = "game_state_change";
        conformationMsg = "Are you sure you|want to go back to|the main menu?";
        executionAction = "title_screen_state";
    }
    
    yesBtn.setActionType( actionType );
    yesBtn.setExecutionAction( executionAction );
    msgLbl.createFontString( conformationMsg );
}
