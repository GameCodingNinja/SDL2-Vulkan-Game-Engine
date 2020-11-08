
//
//  FILE NAME:  titlescreenstate.as
//
//  DESC:       Title screen state
//

final class CTitleScreenState : CCommonState
{
    // An array of the strategies used in this state
    array<string> mStrategyAry = {"_title_background_", "_title_stage_", "_title_actor_", "_title_text_"};

    CPhysicsWorld2D @mPhysicsWorld;

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
        Device.deleteCommandPoolGroup( "(title)" );
        StrategyMgr.deleteStrategyAry( mStrategyAry );
        ObjectDataMgr.freeGroup( "(title)" );
        PhysicsWorldManager2D.clear();
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
        mPhysicsWorld.variableTimeStep();
    }

    //
    //  Begin contact physics callback
    //
    void beginContact( CSprite & spriteA, CSprite & spriteB )
    {
        if( spriteA.getId() == NTitleDefs::SPRITE_PEG )
        {
            spriteA.resetAndRecycle();
            spriteA.setFrame(1);
        }
        else if( spriteB.getId() == NTitleDefs::SPRITE_PEG )
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
        if( spriteA.getId() == NTitleDefs::SPRITE_PEG )
        {
            spriteA.stopAndRestart( "peg_off" );
        }
        else if( spriteB.getId() == NTitleDefs::SPRITE_PEG )
        {
            spriteB.stopAndRestart( "peg_off" );
        }
    }
};

//
//  Functions for loading the assets for this state
//
void LoadTitleScreenAssets()
{
    ObjectDataMgr.loadGroup( "(title)" );

    // Create the physics world
    PhysicsWorldManager2D.createWorld( "(game)" );
    
    // Create the needed strategies
    StrategyMgr.loadStrategy( "data/objects/strategy/state/titlescreen.stage.loader" );
    StrategyMgr.loadStrategy( "data/objects/strategy/state/titlescreen.loader" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}

//
//  AI Update script
//
void Title_BallAI( CSprite & sprite )
{
    sprite.setPhysicsTransform( RandInt(-540,540), -RandInt(1100,1500), RandInt(0,360) );

    Suspend();

    while( true )
    {
        if( sprite.getPos().y > 1100.f )
            sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(1100,1300), RandInt(0,360) );

        Suspend();
    }
}

//
//  AI Update script
//
void Title_PegOff( CSprite & sprite )
{
    Hold( 200 );

    sprite.setFrame(0);
}