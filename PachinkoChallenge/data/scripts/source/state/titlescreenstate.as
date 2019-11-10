
//
//  FILE NAME:  titlescreenstate.as
//
//  DESC:       Title screen state
//

final class CTitleScreenState : CCommonState
{
    // An array of the strategies used in this state
    array<string> mStrategyAry = {"_title_background_", "_title_stage_", "_title_text_"};

    //
    //  Constructor
    //
    CTitleScreenState()
    {
        super( NStateDefs::EGS_TITLE_SCREEN, NStateDefs::EGS_GAME_LOAD );
    }
    
    //
    //  Destroy the state
    //
    void destroy() override
    {
        // Wait for all rendering to be finished
        Device.waitForIdle();
        
        Device.deleteCommandPoolGroup( "(title)" );
        
        StrategyMgr.deleteStrategyAry( mStrategyAry );
        
        ObjectDataMgr.freeGroup( "(title)" );
    }

    //
    //  Init the state
    //
    void init() override
    {
        // Unblock the menu messaging and activate needed trees(s)
        MenuMgr.activateTree( "title_screen_tree" );
        
        // Activate the strategies
        StrategyMgr.activateStrategyAry( mStrategyAry );
        
        // Do the fade in
        Spawn("State_FadeIn", "(state)");
    }
    
    //
    //  Handle events
    //
    void handleEvent() override
    {
        CCommonState::handleEvent();
        
        // Check for the "game change state" message
        if( ActionMgr.wasGameEvent( NMenuDefs::EME_MENU_GAME_STATE_CHANGE, NMenuDefs::ETC_BEGIN ) )
            Spawn("State_FadeOut", "(state)");
        
        else if( ActionMgr.wasGameEvent( NStateDefs::ESE_FADE_IN_COMPLETE ) )
            MenuMgr.allow();
        
        else if( ActionMgr.wasGameEvent( NStateDefs::ESE_FADE_OUT_COMPLETE ) )
        {
            // Clear out all the trees
            MenuMgr.clearActiveTrees();

            // Set the flag to change the state
            mChangeState = true;
        }
    }
};

//
//  Functions for loading the assets for this state
//
void LoadTitleScreenAssets()
{
    ObjectDataMgr.loadGroup( "(title)" );
    
    // Create the needed strategies
    StrategyMgr.loadStrategy( "data/objects/strategy/state/titlescreen.loader" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}
    