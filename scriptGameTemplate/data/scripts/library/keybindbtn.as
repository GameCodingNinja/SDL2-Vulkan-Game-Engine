//
//  FILE NAME:  keybindbtn.as
//

// final class CKeyBindBtn
// {
//     // control handles
//     uiControl @mControl;
//     uiControl @mKeyboardControl;
//     uiControl @mMouseControl;
//     uiControl @mGampadControl;
    
//     //
//     //  Constructor
//     //
//     CKeyBindBtn( uiControl & control )
//     {
//         @mControl = control;

//         array<uiControl @> deviceAry = {
//             mControl.findControl("keyboard"),
//             mControl.findControl("mouse"),
//             mControl.findControl("gamepad")};

//         @mKeyboardControl = mControl.findControl("keyboard");
//         @mMouseControl = mControl.findControl("mouse");
//         @mGampadControl = mControl.findControl("gamepad");

//         string key;
//         string mouse;
//         string gamepad;
//         bool configurable;

//         string test = ActionMgr.getDeviceActionStr( mControl.getName(), configurable );

//         Print(test +", " + configurable);
//     }
// };

// CKeyBindBtn @gCKeyBindBtn;

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
//  Handle select event
//
void KeyBindBtn_execute( uiControl & control )
{
    //Control_OnSelect( control );

    // Disable all action checking so that most buttons can be key mapped
    // without being acted on
    ActionMgr.enableAction(false);
}

//
//  Handle select event
//
void KeyBindBtn_event( uiControl & control, uint type, int code )
{
    if( control.isSelected() )
    {
        if( ActionMgr.wasEvent(NDefs::SDL_KEYUP) )
        {
            int keyCode;
            uint repeat;
            if( ActionMgr.enumerateKeyboardEvent( NDefs::SDL_KEYUP, keyCode, repeat ) > 0 )
            {
                // Check for escape to disable key mapping process
                if( keyCode != NDefs::KEYCODE_RETURN )
                {
                    // Reset the action
                    bool configurable = false;
                    string componetIdStr = ActionMgr.resetAction( NDefs::KEYBOARD, control.getName(), keyCode, configurable );
                    Print("Reset Action: " + control.getName() + ", key code: " + keyCode + ", Component Id: " + componetIdStr + ", Configurable: " + configurable);

                    if( configurable )
                    {
                        // Reset the string Id
                        control.findControl("keyboard").createFontString( componetIdStr );

                        // Save the key binding changes to file
                        ActionMgr.saveToFile();
                    }
                }

                // Re-enable action checking
                ActionMgr.enableAction();

                // Dispatch a message to clear the selected control and put it back into active state
                DispatchEvent( NMenuDefs::EME_MENU_REACTIVATE );
            }
        }
    }
}
