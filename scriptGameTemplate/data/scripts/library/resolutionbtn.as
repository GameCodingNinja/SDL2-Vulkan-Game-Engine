//
//  FILE NAME:  resolutionbtn.as
//

final class CResolutionBtn
{
    // control handle
    uiControl @mControl;

    // Resolution index
    int resIndex = 0;

    //
    //  Constructor
    //
    CResolutionBtn( uiControl & control )
    {
        @mControl = control;

        // Get the screen resolutions
        array<CSize> @ resAry = GetScreenResolutions();
        
        // Get the current game resolution
        CSize curRes = Settings.getResolution();
        
        // Add the strings to the control
        for( uint i = 0; i < resAry.length(); ++i )
            mControl.setStringToList( formatInt(int(resAry[i].w)) + " x "  + formatInt(int(resAry[i].h)) );
        
        // Find the current resolution and set it
        for( uint i = 0; i < resAry.length(); ++i )
        {
            // If we found our current game res, set it
            if( int(resAry[i].w) == int(curRes.w) && int(resAry[i].h) == int(curRes.h) )
            {
                resIndex = i;
                break;
            }
        }
    }

    //
    //  Has this button changed?
    //
    bool hasChanged()
    {
        return resIndex != mControl.getActiveIndex();
    }

    //
    //  Set the current resolution index
    //
    void setResIndex()
    {
        mControl.setActiveIndex( resIndex );
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
