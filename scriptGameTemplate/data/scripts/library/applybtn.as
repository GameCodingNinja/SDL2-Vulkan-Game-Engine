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
        gFullScreenCheckbox.setFullScreen();

        if( gResolutionBtn.hasChanged() )
            gResolutionBtn.changeResolution();

        else if( gFullScreenCheckbox.hasChanged() )
            gFullScreenCheckbox.changeFullScreen();

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
