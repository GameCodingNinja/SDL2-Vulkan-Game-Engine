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
            deviceAry[i].changeState( NControlState::DISABLE );
        else
            btnDisabled = false;
    }

    // If all devices are not configuable, disable the button
    if( btnDisabled )
        control.changeState( NControlState::DISABLE );
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
        uint eventType = 0;
        int eventCode = 0;
        int data = 0;
        uint index = 0;
        while( (index = ActionMgr.enumerateButtonEvents( eventType, eventCode, data, index )) > 0 )
        {
            if( eventType == NEvents::SDL_KEYUP )
            {
                Print("Type: KEYBOARD, code: "+eventCode+", data: "+data+", index: "+index);
                KeyBindBtn_bindButtonPress( control, type, NDeviceId::KEYBOARD, eventCode );
                break;
            }
            else if( eventType == NEvents::SDL_MOUSEBUTTONUP )
            {
                Print("Type: MOUSE, code: "+eventCode+", data: "+data+", index: "+index);
                KeyBindBtn_bindButtonPress( control, type, NDeviceId::MOUSE, eventCode );
                break;
            }
            else if( eventType == NEvents::SDL_CONTROLLERBUTTONUP )
            {
                Print("Type: GAMEPAD, code: "+eventCode+", data: "+data+", index: "+index);
                KeyBindBtn_bindButtonPress( control, type, NDeviceId::GAMEPAD, eventCode );
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
    if( deviceId == NDeviceId::MOUSE )
        labelStr = "mouse";
    else if( deviceId == NDeviceId::GAMEPAD )
        labelStr = "gamepad";

    // Check for escape to disable key mapping process
    if( code != NKeyCodes::KEYCODE_RETURN )
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
    DispatchEvent( NMenuEvent::REACTIVATE );
}

//
//  Handle resettings of key bind buttons
//
void KeyBindBtn_reset( uiControl & control )
{
    // Reset the key bindings for all controls and save
    ActionMgr.resetKeyBindingsToDefault();

    // Since the above reset all the keybinding to the default value,
    // Need to do the below to reload the data into the scroll box

    // Get the scroll box control and the number of controls it contains
    uiControl @scrollBoxCtrl = MenuMgr.getMenu( "key_bindings_menu" ).getControl( "key_binding_scroll_box" );
    uint size = scrollBoxCtrl.size();

    for( uint i = 0; i < size; ++i )
    {
        uiControl @scrollCtrl = scrollBoxCtrl.getScrollBoxControl(i);
        if( scrollCtrl.getFaction() == "key_binding_btn" )
        {
            Print(scrollCtrl.getName());
            KeyBindBtn_init( scrollCtrl );
        }
    }

    // Dispatch the message to reactivate the menu
    DispatchEvent( NMenuEvent::REACTIVATE );
}
