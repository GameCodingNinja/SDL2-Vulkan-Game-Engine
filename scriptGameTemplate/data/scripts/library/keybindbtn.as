//
//  FILE NAME:  keybindbtn.as
//

//
//  Handle init
//
void KeyBindBtn_init( uiControl & control )
{
    array<uiControl @> deviceAry = {
        control.findControl("keyboard"),
        control.findControl("mouse"),
        control.findControl("gamepad")};

    bool btnDisabled = true;

    for( uint i = 0; i < deviceAry.length(); ++i )
    {
        bool configurable = false;
        string actionStr = ActionMgr.getDeviceActionStr( i, control.getName(), configurable );

        if( actionStr.isEmpty() )
            deviceAry[i].createFontString( "NA" );
        else
            deviceAry[i].createFontString( actionStr );

        if( !configurable )
            deviceAry[i].changeState( NUIControlDefs::ECS_DISABLE );
        else
            btnDisabled = false;
    }

    // If all devices are not configuable, disable the button
    if( btnDisabled )
        control.changeState( NUIControlDefs::ECS_DISABLE );
}

//
//  Handle execute event
//
void KeyBindBtn_execute( uiControl & control )
{
    // Disable all action checking so that most buttons can be key mapped
    // without being acted on
    ActionMgr.enableAction(false);
}

//
//  Handle event
//
void KeyBindBtn_event( uiControl & control, uint type, int code )
{
    if( !ActionMgr.isAction() && control.isSelected() )
    {
        uint type = 0;
        int code = 0;
        int data = 0;
        uint index = 0;
        while( (index = ActionMgr.enumerateButtonEvents( type, code, data, index )) > 0 )
        {
            if( type == NDefs::SDL_KEYUP )
            {
                Print("Type: KEYBOARD, code: "+code+", data: "+data+", index: "+index);
                KeyBindBtn_bindButtonPress( control, type, NDefs::KEYBOARD, code );
                break;
            }
            else if( type == NDefs::SDL_MOUSEBUTTONUP )
            {
                Print("Type: MOUSE, code: "+code+", data: "+data+", index: "+index);
                KeyBindBtn_bindButtonPress( control, type, NDefs::MOUSE, code );
                break;
            }
            else if( type == NDefs::SDL_CONTROLLERBUTTONUP )
            {
                Print("Type: GAMEPAD, code: "+code+", data: "+data+", index: "+index);
                KeyBindBtn_bindButtonPress( control, type, NDefs::GAMEPAD, code );
                break;
            }
        }
    }
}

//
//  Bind button press to action
//
void KeyBindBtn_bindButtonPress( uiControl & control, uint type, int deviceId, int code )
{
    string labelStr = "keyboard";
    if( deviceId == NDefs::MOUSE )
        labelStr = "mouse";
    else if( deviceId == NDefs::GAMEPAD )
        labelStr = "gamepad";

    // Check for escape to disable key mapping process
    if( code != NDefs::KEYCODE_RETURN )
    {
        // Reset the action
        bool configurable = false;
        string componetIdStr = ActionMgr.resetAction( deviceId, control.getName(), code, configurable );

        if( configurable )
        {
            // Reset the string Id
            control.findControl( labelStr ).createFontString( componetIdStr );

            // Save the key binding changes to file
            ActionMgr.saveToFile();
        }
    }

    // Re-enable action checking
    ActionMgr.enableAction();

    // Dispatch a message to clear the selected control and put it back into active state
    DispatchEvent( NMenuDefs::EME_MENU_REACTIVATE );
}
