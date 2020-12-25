
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
        SoundMgr.stopAllSound();
        Device.deleteCommandPoolGroup( "(title)" );
        
        StrategyMgr.deleteStrategy( "_title_" );
        StrategyMgr.deleteStrategy( "_cube_" );
        
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
        
        // Activate the strategies
        StrategyMgr.activateStrategy( "_title_" );
        StrategyMgr.activateStrategy( "_cube_" );
        
        // Do the fade in
        Spawn("State_FadeIn");
    }
    
    //
    //  Handle events
    //
    void handleEvent( const CEvent & event ) override
    {
        CCommonState::handleEvent( event );

        if( event.type > NEvents::SDL_USEREVENT )
        {
            // Check for the "game change state" message
            if( (event.type == NMenuEvent::GAME_STATE_CHANGE) && (event.user.code == NTransCode::BEGIN) )
                Spawn("State_FadeOut");
            
            else if( event.type == NStateDefs::ESE_FADE_IN_COMPLETE )
                MenuMgr.allow();
            
            else if( event.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
            {
                // Clear out all the trees
                MenuMgr.clearActiveTrees();

                // Set the flag to change the state
                mChangeState = true;
            }
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
    StrategyMgr.loadStrategy( "data/objects/strategy/state/titlescreen.loader" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}
    