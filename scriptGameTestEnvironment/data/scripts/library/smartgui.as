
//
//  FILE NAME:  smartgui.as
//
//  DESC:       Smaert GUI menu scripts
//

//
//  Execution script for the menu control to change to the confirmation menu
//
void ControlConfirm_OnExecute( uiControl & control )
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
        executionAction = "run_state";
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

//
//  Execution script for the menu control to fill it with display resolutions
//
void ControlResolution_OnInit( uiControl & control )
{
    // Get the screen resolutions
    array<CSize> @ resAry = GetScreenResolutions();
    
    // Get the current game resolution
    CSize curRes = Settings.getResolution();
    
    // Add the strings to the control
    for( uint i = 0; i < resAry.length(); ++i )
        control.setStringToList( formatInt(int(resAry[i].w)) + " x "  + formatInt(int(resAry[i].h)) );
    
    // Find the current resolution and set it
    for( uint i = 0; i < resAry.length(); ++i )
    {
        // If we found our current game res, set it
        if( int(resAry[i].w) == int(curRes.w) && int(resAry[i].h) == int(curRes.h) )
        {
            control.setActiveIndex( i );
            break;
        }
    }
}

//
//  Execution script to set the saved position of the dead zone slider
//
void ControlDeadZoneSlider_OnInit( uiControl & control )
{
    control.setSliderPosValue( Settings.getGamePadStickDeadZone() );
}
