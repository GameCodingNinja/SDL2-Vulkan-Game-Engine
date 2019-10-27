
//
//  FILE NAME:  loadstate.as
//
//  DESC:       Load up game state
//

final class CLoadState : CCommonState
{
    //
    //  Constructor
    //
    CLoadState( const CStateMessage & stateMsg )
    {
        super( NStateDefs::EGS_GAME_LOAD, stateMsg );
    }
    
    //
    //  Destroy the state
    //
    void destroy() override
    {
        // Wait for all rendering to be finished
        Device.waitForIdle();
        
        Device.deleteCommandPoolGroup( "(load)" );
        ObjectDataMgr.freeGroup( "(load)" );
        StrategyMgr.deleteStrategy( "_load_" );
    }

    //
    //  Init the state
    //
    void init() override
    {
        // Load group specific assets
        ObjectDataMgr.loadGroup( "(load)" );
        
        // Create the needed strategy
        StrategyMgr.loadStrategy( "data/objects/strategy/state/loadscreen.loader" );
        
        // Start the fade in and animation
        Spawn("State_FadeIn", "(state)");
    }
    
    //
    //  Handle the events
    //
    void handleEvent() override
    {
        if( ActionMgr.wasEvent( NStateDefs::ESE_FADE_IN_COMPLETE ) )
            assetLoad();
        
        else if( ActionMgr.wasEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE ) )
            Spawn("State_FadeOut", "(state)");
        
        else if( ActionMgr.wasEvent( NStateDefs::ESE_FADE_OUT_COMPLETE ) )
            mChangeState = true;
    }
    
    //
    //  Load the assets
    //
    void assetLoad()
    {
        if( mStateMessage.getLoadState() == NStateDefs::EGS_TITLE_SCREEN )
            SpawnByThread( "LoadTitleScreenAssets" );

        else if( mStateMessage.getLoadState() == NStateDefs::EGS_RUN )
            SpawnByThread( "LoadRunAssets" );
    }
    
    //
    //  Get the next game state
    //
    NStateDefs::EGameState getNextState()
    {
        return mStateMessage.getLoadState();
    }
};
