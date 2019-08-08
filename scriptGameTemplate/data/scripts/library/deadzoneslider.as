//
//  FILE NAME:  deadzoneslider.as
//

final class CDeadZoneSlider
{
    // control handle
    uiControl @mControl;
    
    //
    //  Constructor
    //
    CDeadZoneSlider( uiControl & control )
    {
        @mControl = control;
    }

    //
    //  Has this button changed?
    //
    bool hasChanged()
    {
        return int(mControl.getSliderPosValue()) != Settings.getGamePadStickDeadZone();
    }

    //
    //  Set the dead zone value
    //
    void setGamePadStickDeadZone()
    {
        Settings.setGamePadStickDeadZone( int(mControl.getSliderPosValue()) );
    }
};

CDeadZoneSlider @gDeadZoneSlider;

//
//  Handle init
//
void DeadZoneSlider_init( uiControl & control )
{
    @gDeadZoneSlider = CDeadZoneSlider( control );
}

//
//  Handle trans-in event
//
void DeadZoneSlider_transIn( uiControl & control )
{
    control.setSliderPosValue( Settings.getGamePadStickDeadZone() );
}


//
//  Handle select event
//
void DeadZoneSlider_select( uiControl & control )
{
    SoundMgr.play( "(menu)", "select" );

    gApplyBtn.enableIfChanged();
}

//
//  Handle change event
//
void DeadZoneSlider_change( uiControl & control )
{
    gApplyBtn.enableIfChanged();
}
