
//
//  FILE NAME:  level1state.as
//
//  DESC:       Level 1 state
//

final class CRunState : CCommonState
{
    // Strategy array of names for easy creation and destruction of stratigies
    array<string> mStrategyAry = {"_level_1_stage_","_level_1_ball_","_level_1_ui_"};

    // Ball list
    array<string> mBallAry = 
        {"square_red","square_green","square_blue",
        "triangle_red","triangle_blue","triangle_green",
        "circle_red","circle_blue","circle_green"};
    
    // Physics world object
    CPhysicsWorld2D @mPhysicsWorld;

    // Actor strategy reference(s)
    Strategy @mBallStrategy;
    Strategy @mUIStrategy;

    // Level camera for orthographic point calculations
    CCamera @mCamera;

    // Reference to meter control
    uiControl @mWinMeterCtrl;

    // Reference to timer text sprite
    CSprite @mUITimerSprite;

    // Multiplier sprite
    CSprite @mUIMultiSprite;

    // Points multiplier
    uint multiplier = 1;

    // Next ball index
    int mNextBallIndex = RandInt(0,mBallAry.length()-1);

    // Start of the timer time point
    CTimePoint mTimePointStart;

    // Game active flag
    bool mGameActive = false;
    
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
        MenuMgr.allow();
        MenuMgr.activateTree( "game_start_tree" );
        MenuMgr.transitionMenu( "game_start_tree" );
        
        // Enable the needed strategies
        StrategyMgr.activateStrategyAry( mStrategyAry );
        @mBallStrategy = StrategyMgr.getStrategy( "_level_1_ball_" );
        @mUIStrategy = StrategyMgr.getStrategy( "_level_1_ui_" );
        @mWinMeterCtrl = mUIStrategy.getNode("uiWinMeter").getControl();
        @mUITimerSprite = mUIStrategy.getNode("uiTimerText").getSprite();
        @mUIMultiSprite = mUIStrategy.getNode("uiMultplierText").getSprite();

        // Make the next ball visible
        mUIStrategy.activateNode(mBallAry[mNextBallIndex]);
        
        // Get the physics world
        @mPhysicsWorld = PhysicsWorldManager2D.getWorld( "(game)" );
        mPhysicsWorld.EnableContactListener();

        // Get the camera
        @mCamera = CameraMgr.get( "level_camera" );
        
        // Do the fade in
        Spawn("State_FadeIn");
    }
    
    //
    //  Handle events
    //
    void handleEvent( const CEvent & event ) override
    {
        CCommonState::handleEvent( event );
        
        if( event.type > NDefs::SDL_USEREVENT )
        {
            // Check for the "game change state" message
            if( (event.type == NMenuDefs::EME_MENU_GAME_STATE_CHANGE) && (event.user.code == NMenuDefs::ETC_BEGIN) )
                Spawn("State_FadeOut");

            else if( event.type == NLevelDefs::ELE_BANG_UP_AWARD )
                mWinMeterCtrl.incBangUp( multiplier );
            
            else if( event.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
            {
                // Clear out all the trees
                MenuMgr.clearActiveTrees();

                // Set the flag to change the state
                mChangeState = true;
            }
            else if( event.type == NMenuDefs::EME_MENU_TRANS_OUT )
            {
                if( event.user.code == NMenuDefs::ETC_END && !mGameActive )
                {
                    // Switch out the default menus
                    MenuMgr.deactivateTree( "game_start_tree" );
                    MenuMgr.activateTree( "pause_tree" );

                    // Start the game
                    mGameActive = true;
                    mTimePointStart.now( GetDurationMinutes(1) );
                }
            }
        }
        else if( event.type == NDefs::SDL_MOUSEBUTTONUP )
        {
            if( !MenuMgr.isActive() && ActionMgr.wasAction(event, "drop_ball", NDefs::EAP_UP) )
            {
                // Strictly a mouse only game which is why I'm using the button event x & y
                CPoint pos= mCamera.toOrthoCoord( event.button.x, event.button.y );
                CSprite @sprite = mBallStrategy.create(mBallAry[mNextBallIndex]).getSprite();
                sprite.setPhysicsTransform(pos.x, -1400);
                sprite.applyAngularImpulse(RandFloat(-0.5, 0.5));

                // Randomly generate the next ball
                int lastBallIndex = mNextBallIndex;
                mNextBallIndex = RandInt(0,mBallAry.length()-1);

                // Hide the last UI ball and show the next one
                if( lastBallIndex != mNextBallIndex )
                {
                    mUIStrategy.deactivateNode( mBallAry[lastBallIndex] );
                    mUIStrategy.activateNode( mBallAry[mNextBallIndex] );
                }
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
    //  Handle the update
    //
    void update() override
    {
        CCommonState::update();

        if( mGameActive )
        {
            CTimePoint timePoint;
            CTimeDuration duration( mTimePointStart - timePoint );
            if( duration.getNanoseconds() > 0 )
            {
                mUITimerSprite.createFontString( FormatTimeDuration( mTimePointStart - timePoint, NDefs::ETF_M_S ) );
            }
            else
            {
                mUITimerSprite.createFontString( "00:00" );
                mGameActive = false;

                // Switch out the default menus and activate
                MenuMgr.deactivateTree( "pause_tree" );
                MenuMgr.activateTree( "game_start_tree" );
                MenuMgr.transitionMenu( "game_start_tree" );
            }
        }
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
    StrategyMgr.loadStrategy( "data/objects/strategy/level_1/strategy.loader" );
    StrategyMgr.loadStrategy( "data/objects/strategy/level_1/stage.loader" );
    
    // Send a message to indicate the load is done
    DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
}


//
//  AI Update script
//
void Level_BallAI( CSprite & sprite )
{
    while(true)
    {
        if( sprite.getPos().y > 1700.f )
        {
            StrategyMgr.getStrategy("_level_1_ball_").destroy(sprite.getHandle());
            break;
        }
        else if(
            (sprite.getPos().y > 1600.f) && 
            (abs(sprite.getPos().x) < 720) && 
            !sprite.getParameters().isSet(NLevelDefs::ESS_BANG_UP_AWARD) )
        {
            // Set the state so as to not enter this if again
            sprite.getParameters().add(NLevelDefs::ESS_BANG_UP_AWARD);

            // Dispatch message to bang this one up
            DispatchEvent(NLevelDefs::ELE_BANG_UP_AWARD);
        }

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