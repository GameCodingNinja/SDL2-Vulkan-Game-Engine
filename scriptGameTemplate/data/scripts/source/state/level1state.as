
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
        mPhysicsWorld.EnableContactListener();
        
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
        if( spriteA.getId() == NLevelDefs::SPRITE_PEG )
        {
            spriteA.resetAndRecycle();
            spriteA.setFrame(1);
        }
        else if( spriteB.getId() == NLevelDefs::SPRITE_PEG )
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
        if( spriteA.getId() == NLevelDefs::SPRITE_PEG )
        {
            spriteA.stopAndRestart( "peg_off" );
        }
        else if( spriteB.getId() == NLevelDefs::SPRITE_PEG )
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
    
    // Create the physics world
    PhysicsWorldManager2D.createWorld( "(game)" );
    
    // Create the needed strategies
    StrategyMgr.loadStrategy( "data/objects/strategy/level_1/stage.strategy.loader" );
    StrategyMgr.loadStrategy( "data/objects/strategy/level_1/ball.strategy.loader" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}


//
//  AI Update script
//
void Level_BallAI( CSprite & sprite )
{
    sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );

    Suspend();

    while( true )
    {
        if( sprite.getPos().y > 600.f )
            sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );

        Suspend();
    }
}

//
//  AI Update script
//
void Level_PegOff( CSprite & sprite )
{
    Hold( 200 );

    sprite.setFrame(0);
}