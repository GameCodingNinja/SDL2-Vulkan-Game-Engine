//
//  FILE NAME:  applybtn.as
//

final class CApplyBtn
{
    // control handle
    uiControl @mControl;

    //
    //  Constructor
    //
    CApplyBtn( uiControl & control )
    {
        @mControl = control;
    }

    //
    //  Should we enable/disable this button if the others have changed
    //
    bool shouldEnable()
    {
        if( gResolutionBtn.hasChanged() )
            return true;

        if( gFullScreenCheckbox.hasChanged() )
            return true;

        if( gDeadZoneSlider.hasChanged() )
            return true;

        return false;
    }

    //
    //  Enable or disable the button
    //
    void enableIfChanged()
    {
        if( shouldEnable() )
            mControl.changeState( NControlDefs::ECS_INACTIVE );
        else
            mControl.changeState( NControlDefs::ECS_DISABLE );
    }

    //
    //  Execute the button command
    //
    void execute()
    {
        // NOTE: Works best when resolution is changed. 
        //       Resets the display when coming back out of full screen
        
        // Change the full screen setting
        gFullScreenCheckbox.setFullScreen();

        // Set the dead zone value
        gDeadZoneSlider.setGamePadStickDeadZone();

        // Resolution has changed and possibly the full screen
        if( gResolutionBtn.hasChanged() )
        {
            gResolutionBtn.changeResolution();
        }
        // Only full screen has changed
        else if( gFullScreenCheckbox.hasChanged() )
        {
            gFullScreenCheckbox.changeFullScreen();
        }

        // Set the internal toggle to the current state
        gFullScreenCheckbox.setToggleState();

        Settings.saveSettings();

        // Reactivate the menu now that we are done
        mControl.changeState( NControlDefs::ECS_DISABLE );
        DispatchEvent( NMenuDefs::EME_MENU_REACTIVATE );
    }
};

CApplyBtn @gApplyBtn;

//
//  Handle init
//
void ApplyBtn_init( uiControl & control )
{
    @gApplyBtn = CApplyBtn( control );
}

//
//  Handle trans-in event
//
void ApplyBtn_transIn( uiControl & control )
{
    control.changeState( NControlDefs::ECS_DISABLE );
}

//
//  Handle execute event
//
void ApplyBtn_execute( uiControl & control )
{
    gApplyBtn.execute();
}
