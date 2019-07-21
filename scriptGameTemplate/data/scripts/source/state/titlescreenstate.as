
//
//  FILE NAME:  titlescreenstate.as
//
//  DESC:       Title screen state
//

final class CTitleScreenState : CCommonState
{
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
        
        StrategyMgr.deleteStrategy( "(title)" );
        StrategyMgr.deleteStrategy( "(cube)" );
        
        ObjectDataMgr.freeGroup( "(title)" );
        ObjectDataMgr.freeGroup( "(cube)" );
    }

    //
    //  Init the state
    //
    void init() override
    {
        // Unblock the menu messaging and activate needed trees(s)
        MenuMgr.activateTree( "title_screen_tree" );
        
        // Activate the strategy and set their command buffers
        StrategyMgr.activateStrategy( "(title)" ).setCommandBuffer( "(title)" );
        StrategyMgr.activateStrategy( "(cube)" ).setCommandBuffer( "(title)" );
        
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
        if( ActionMgr.wasEvent( NMenuDefs::EME_MENU_GAME_STATE_CHANGE, NMenuDefs::ETC_BEGIN ) )
            Spawn("State_FadeOut", "(state)");
        
        else if( ActionMgr.wasEvent( NStateDefs::ESE_FADE_IN_COMPLETE ) )
            MenuMgr.allow();
        
        else if( ActionMgr.wasEvent( NStateDefs::ESE_FADE_OUT_COMPLETE ) )
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
    ObjectDataMgr.loadGroup( "(cube)" );
    
    // Create the needed strategies
    StrategyMgr.createActorStrategy( "(title)" ).create( "background" );
    iStrategy @cubeStrategy = StrategyMgr.createActorStrategy( "(cube)" );
    cubeStrategy.create( "cube" );
    cubeStrategy.setCamera( "cubeCamera" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}
    