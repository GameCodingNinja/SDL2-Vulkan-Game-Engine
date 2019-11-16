
//
//  FILE NAME:  level1state.as
//
//  DESC:       Level 1 state
//

final class CRunState : CCommonState
{
    CPhysicsWorld2D @mPhysicsWorld;
    
    //
    //  Constructor
    //
    CRunState()
    {
        super( NStateDefs::EGS_RUN, NStateDefs::EGS_GAME_LOAD );
    }
    
    //
    //  Destroy the state
    //
    void destroy() override
    {
        ScriptMgr.freeGroup( "(level_1)" );
        ObjectDataMgr.freeGroup( "(level_1)" );
        StrategyMgr.deleteStrategy( "_stage_" );
        StrategyMgr.deleteStrategy( "_level_1_" );
        Device.deleteCommandPoolGroup( "(level_1)" );
        PhysicsWorldManager2D.clear();
    }

    //
    //  Init the state
    //
    void init() override
    {
        // Unblock the menu messaging and activate needed trees(s)
        MenuMgr.activateTree( "pause_tree" );
        
        // Enable the strategy for rendering
        StrategyMgr.activateStrategy( "_stage_" );
        StrategyMgr.activateStrategy( "_level_1_" );
        
        // Get the physics world
        @mPhysicsWorld = PhysicsWorldManager2D.getWorld( "(game)" );
        
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
    
    //
    //  Handle the physics
    //
    void physics() override
    {
        if( !MenuMgr.isActive() )
            mPhysicsWorld.variableTimeStep();
    }
};

//
//  Functions for loading the assets for this state
//
void LoadRunAssets()
{
    ObjectDataMgr.loadGroup( "(level_1)" );
    ScriptMgr.loadGroup( "(level_1)" );
    
    // Create the physics world
    PhysicsWorldManager2D.createWorld( "(game)" );
    
    // Create the needed strategies
    StrategyMgr.loadStrategy( "data/objects/strategy/level_1/strategy.loader" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}
    