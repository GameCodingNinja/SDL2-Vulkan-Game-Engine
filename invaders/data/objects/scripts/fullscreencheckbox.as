//
//  FILE NAME:  fullscreencheckbox.as
//

final class CFullScreenCheckbox
{
    // control handle
    uiControl @mControl;

    bool mToggleState = false;
    
    //
    //  Constructor
    //
    CFullScreenCheckbox( uiControl & control )
    {
        @mControl = control;
    }

    //
    //  Has this button changed?
    //
    bool hasChanged()
    {
        return mControl.getToggleState() != mToggleState;
    }

    //
    //  Change the full screen setting
    //
    void setFullScreen()
    {
        Settings.setFullScreen( mControl.getToggleState() );
    }

    //
    //  Change the full screen setting
    //
    void changeFullScreen()
    {
        Device.setFullScreen( Settings.getFullScreen() );
    }

    //
    //  Change the full screen setting
    //
    void transIn()
    {
        mToggleState = Settings.getFullScreen();
        mControl.setToggleState( mToggleState );
    }

    //
    //  Set the toggle state
    //
    void setToggleState()
    {
        mToggleState = mControl.getToggleState();
    }
};

CFullScreenCheckbox @gFullScreenCheckbox;

//
//  Handle init
//
void FullScreenCheckbox_init( uiControl & control )
{
    @gFullScreenCheckbox = CFullScreenCheckbox( control );
}

//
//  Handle trans-in event
//
void FullScreenCheckbox_transIn( uiControl & control )
{
    gFullScreenCheckbox.transIn();
}

//
//  Handle execute event
//
void FullScreenCheckbox_execute( uiControl & control )
{
    gApplyBtn.enableIfChanged();
}
