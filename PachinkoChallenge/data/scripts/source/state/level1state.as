
//
//  FILE NAME:  level1state.as
//
//  DESC:       Level 1 state
//

enum ESpriteId
{
    SPRITE_PEG = -2,
    SPRITE_MULTI = 0
};

final class CRunState : CCommonState
{
    array<string> mStrategyAry = {"_level_1_stage_","_level_1_actor_","_level_1_ui_"};
    CPhysicsWorld2D @mPhysicsWorld;
    iStrategy @mActorStrategy;
    CCamera @mCamera;
    uiControl @mWinMeterCtrl;
    uint multiplier = 1;
    
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
        StrategyMgr.deleteStrategyAry( mStrategyAry );
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
        
        // Enable the needed strategies
        StrategyMgr.activateStrategyAry( mStrategyAry );
        @mActorStrategy = StrategyMgr.getStrategy( "_level_1_actor_" );
        iStrategy @mUIStrategy = StrategyMgr.getStrategy( "_level_1_ui_" );
        @mWinMeterCtrl = mUIStrategy.getNode("uiWinMeter").getControl();
        
        // Get the physics world
        @mPhysicsWorld = PhysicsWorldManager2D.getWorld( "(game)" );
        mPhysicsWorld.EnableContactListener();

        // Get the camera
        @mCamera = CameraMgr.get( "level_camera" );
        
        // Do the fade in
        Spawn("State_FadeIn", "(state)");
    }
    
    //
    //  Handle events
    //
    void handleEvent() override
    {
        CCommonState::handleEvent();
        
        if( !MenuMgr.isActive() && ActionMgr.wasMouseBtnEvent("LEFT MOUSE", NDefs::EAP_UP) )
        {
            CPoint pos= mCamera.toOrthoCoord( ActionMgr.getMouseAbsolutePos() );
            CSprite @sprite = mActorStrategy.create("square_red").getSprite();
            sprite.setPhysicsTransform(pos.x, -1500);
            sprite.applyAngularImpulse(RandFloat(-0.5, 0.5));
        }
        // Check for the "game change state" message
        else if( ActionMgr.wasGameEvent( NMenuDefs::EME_MENU_GAME_STATE_CHANGE, NMenuDefs::ETC_BEGIN ) )
            Spawn("State_FadeOut", "(state)");

        else if( ActionMgr.wasGameEvent( NLevelDefs::ELE_BANG_UP_AWARD ) )
            mWinMeterCtrl.incBangUp( multiplier );
        
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

    //
    //  Begin contact physics callback
    //
    void beginContact( CSprite & spriteA, CSprite & spriteB )
    {
        if( spriteA.getId() == SPRITE_PEG )
        {
            spriteA.resetAndRecycle();
            spriteA.setFrame(1);
        }
        else if( spriteB.getId() == SPRITE_PEG )
        {
            spriteB.resetAndRecycle();
            spriteB.setFrame(1);
        }
    }

    //
    //  End contact physics callback
    //
    void endContact( CSprite & spriteA, CSprite & spriteB )
    {
        if( spriteA.getId() == SPRITE_PEG )
        {
            spriteA.stopAndRestart( "peg_off" );
        }
        else if( spriteB.getId() == SPRITE_PEG )
        {
            spriteB.stopAndRestart( "peg_off" );
        }
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
    