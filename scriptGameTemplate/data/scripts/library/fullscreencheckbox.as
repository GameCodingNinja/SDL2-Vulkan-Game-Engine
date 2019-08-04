//
//  FILE NAME:  fullscreencheckbox.as
//

final class CFullScreenCheckbox
{
    // control handle
    uiControl @mControl;
    
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
        return mControl.getToggleState() != Settings.getFullScreen();
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
    control.setToggleState( Settings.getFullScreen() );
}

//
//  Handle select event
//
void FullScreenCheckbox_execute( uiControl & control )
{
    gApplyBtn.enableIfChanged();
}
