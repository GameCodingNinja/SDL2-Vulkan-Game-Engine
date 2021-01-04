//
//  FILE NAME:  confirmbtn.as
//

//
//  Execution script for a button control to change to the confirmation menu
//  NOTE: The control that initiated the confirmation is the passed in control
//
void ConfirmBtn_execute( uiControl & control )
{
    uiControl @yesBtn = MenuMgr.getMenu( "confirmation_menu" ).getControl( "yes_btn" );
    uiControl @noBtn = MenuMgr.getMenu( "confirmation_menu" ).getControl( "no_btn" );
    uiControl @msgLbl = MenuMgr.getMenu( "confirmation_menu" ).getControl( "message_lbl" );
    
    string actionType;
    string executionAction;
    string conformationMsg;
    
    if( control.getName() == "continue_btn" )
    {
        actionType = "game_state_change";
        conformationMsg = "Are you ready|to play|Pachinko Challenge?";
        executionAction = "level_1_state";
    }
    else if( control.getName() == "main_menu_btn" )
    {
        actionType = "game_state_change";
        conformationMsg = "Are you sure you|want to go back to|the title screen?";
        executionAction = "title_screen_state";
    }
    
    yesBtn.removeScriptFunction( "execute" );
    yesBtn.setActionType( actionType );
    yesBtn.setExecutionAction( executionAction );
    noBtn.setActionType( "back" );
    noBtn.setExecutionAction( "" );
    msgLbl.createFontString( conformationMsg );
}
