//
//  FILE NAME:  resolutionbtn.as
//

final class CResolutionBtn
{
    // control handle
    uiControl @mControl;

    // Resolution index
    int mResIndex = 0;

    // Screen resolutions
    array<CSize> @mResAry;

    //
    //  Constructor
    //
    CResolutionBtn( uiControl & control )
    {
        @mControl = control;

        // Get the screen resolutions
        @mResAry = GetScreenResolutions();
        
        // Get the current game size/resolution
        CSize curRes = Settings.getSize();
        
        // Add the strings to the control
        for( uint i = 0; i < mResAry.length(); ++i )
            mControl.setStringToList( formatInt(int(mResAry[i].w)) + " x "  + formatInt(int(mResAry[i].h)) );
        
        // Find the current resolution and record the index
        for( uint i = 0; i < mResAry.length(); ++i )
        {
            // If we found our current game res, set it
            if( int(mResAry[i].w) == int(curRes.w) && int(mResAry[i].h) == int(curRes.h) )
            {
                mResIndex = i;
                break;
            }
        }
    }

    //
    //  Has this button changed?
    //
    bool hasChanged()
    {
        return mResIndex != mControl.getActiveIndex();
    }

    //
    //  Set the current resolution index
    //
    void setResIndex()
    {
        mControl.setActiveIndex( mResIndex );
    }

    //
    //  Change the resolution
    //
    void changeResolution()
    {
        mResIndex = mControl.getActiveIndex();

        Device.changeResolution( mResAry[mResIndex], Settings.getFullScreen() );
    }
};

CResolutionBtn @gResolutionBtn;

//
//  Handle init
//
void ResolutionBtn_init( uiControl & control )
{
    @gResolutionBtn = CResolutionBtn( control );
}

//
//  Handle trans-in event
//
void ResolutionBtn_transIn( uiControl & control )
{
    gResolutionBtn.setResIndex();
}

//
//  Handle change event
//
void ResolutionBtn_change( uiControl & control )
{
    gApplyBtn.enableIfChanged();
}
