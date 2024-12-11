
//
//  FILE NAME:  level1state.as
//
//  DESC:       Level 1 state
//

enum ELevelState
{
    ELS_IDLE,
    ELS_ACTIVE,
    ELS_GAME_OVER,
    ELS_GAME_RESTART
};


final class CRunState : CCommonState
{
    bool DEBUG_AUTOPLAY = false;
    CTimer debugTimer(300);

    // Strategy array of names for easy creation and destruction of stratigies
    array<string> mStrategyAry = {"_level_stage_","_level_multi_","_level_ball_","_level_ui_"};

    // Ball list
    /*array<string> mBallAry = 
        {"square_red","square_green","square_blue",
        "triangle_red","triangle_blue","triangle_green",
        "circle_red","circle_blue","circle_green"};*/

    array<string> mBallAry = 
        {"triangle_red","triangle_blue","triangle_green",
        "circle_red","circle_blue","circle_green"};

    // Multiplier positions
    array<int> mMultiXPosAllAry = {-640,-480,-320,-160,0,160,320,480,640};

    // A multidimensional to hold the spots to randomly place the multiplier based on it's current position.
    array<array<int>> mMultiXPosAry = {
        {-480,-320,-160,0,160,320,480,640},
        {-640,-320,-160,0,160,320,480,640},
        {-640,-480,-160,0,160,320,480,640},
        {-640,-480,-320,0,160,320,480,640},
        {-640,-480,-320,-160,160,320,480,640},
        {-640,-480,-320,-160,0,320,480,640},
        {-640,-480,-320,-160,0,160,480,640},
        {-640,-480,-320,-160,0,160,320,640},
        {-640,-480,-320,-160,0,160,320,480} };

    // Initial start timer
    int mInitialStartTime = 1000 * 60 + 900;
    int mLevelUpIncMultiTime = 1000 * 30 + 900;
    int mMultiUpIncMultiTime = 1000 * 2;
    int mInitialMultiGoal = 10;
    int mIncMultiGoal = 10;

    // Multiplier Y position
    int mMultiY = 1450;

    // Index of multiplier position
    int mMultiIndexPos;
    
    // Physics world object
    CPhysicsWorld2D @mPhysicsWorld;

    // Strategy reference(s)
    Strategy @mMultiStrategy;
    Strategy @mBallStrategy;
    Strategy @mUIStrategy;

    // Multipler sprite
    CSprite @mMultiSprite;

    // Level camera for orthographic point calculations
    CCamera @mCamera;

    // Timer text sprite
    CSprite @mUITimerSprite;

    // Player UI sprites
    uiControl @mUIPlayerWinMeterCtrl;
    CSprite @mUIPlayerMultiTxtSprite;
    CSprite @mUIPlayerLevelTxtSprite;
    CSprite @mUIPlayerStrawberrySprite;

    // Goal Multi sprite
    CSprite @uiGoalMultiSprite;

    // Points multiplier
    uint mMultiplier;

    // Goal multiplier
    uint mGoalMultiplier;

    // Level
    int mLevel;

    // Next ball index
    int mNextBallIndex = -1;

    // Start of the timer time point
    CTimePoint mTimePointStart;

    // Game active flag
    ELevelState mLevelState = ELS_IDLE;
    
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
        SoundMgr.stopAllSound();
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
        // Determine debug and autoplay mode
        if( Settings.isDebugMode() && Settings.isAutoplay() )
            DEBUG_AUTOPLAY = true;

        // Unblock the menu messaging and activate needed trees(s)
        MenuMgr.allow();
        MenuMgr.activateTree( "game_start_tree" );
        MenuMgr.transitionMenu( "game_start_tree" );
        
        // Enable the needed strategies
        StrategyMgr.activateStrategyAry( mStrategyAry );
        @mMultiStrategy = StrategyMgr.getStrategy( "_level_multi_" );
        @mBallStrategy = StrategyMgr.getStrategy( "_level_ball_" );
        @mUIStrategy = StrategyMgr.getStrategy( "_level_ui_" );

        // Get the needed nodes/sprites/controls
        @mUIPlayerWinMeterCtrl = mUIStrategy.getNode("uiPlayerWinMeter").getControl();
        @mUIPlayerMultiTxtSprite = mUIStrategy.getNode("uiPlayerMultiText").getSprite();
        @mUIPlayerLevelTxtSprite = mUIStrategy.getNode("uiPlayerLevelText").getSprite();
        @mUIPlayerStrawberrySprite = mUIStrategy.getNode("uiPlayerStrawberry").getSprite();
        @mUITimerSprite = mUIStrategy.getNode("uiTimerText").getSprite();
        @uiGoalMultiSprite = mUIStrategy.getNode("uiGoalMulti").getSprite();

        // Get the multipler sprite
        @mMultiSprite = mMultiStrategy.getNode("strawberry").getSprite();
        mMultiIndexPos = RandInt(0,mMultiXPosAllAry.length()-1);
        mMultiSprite.setPhysicsTransform(mMultiXPosAllAry[mMultiIndexPos], mMultiY);
        
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
        
        if( event.type > NEvents::SDL_USEREVENT )
        {
            // Check for the "game change state" message
            if( (event.type == NMenuEvent::GAME_STATE_CHANGE) && (event.user.code == NTransCode::BEGIN) )
                Spawn("State_FadeOut");

            else if( event.type == NLevelDefs::ELE_BANG_UP_AWARD )
                mUIPlayerWinMeterCtrl.incBangUp( mMultiplier );

            else if( event.type == NLevelDefs::ELE_RESTART_GAME )
            {
                mLevelState = ELS_GAME_RESTART;
            }
            else if( event.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
            {
                // Clear out all the trees
                MenuMgr.clearActiveTrees();

                // Set the flag to change the state
                mChangeState = true;
            }
            else if( event.type == NMenuEvent::TRANS_OUT )
            {
                if( event.user.code == NTransCode::END )
                {
                    if( mLevelState == ELS_IDLE || mLevelState == ELS_GAME_RESTART )
                    {
                        // Switch out the default menus
                        MenuMgr.deactivateTree( "game_start_tree" );
                        MenuMgr.deactivateTree( "game_over_tree" );
                        MenuMgr.activateTree( "pause_tree" );

                        // Start the game
                        mLevel = 1;
                        mMultiplier = 1;
                        showNextBall();
                        moveMultiplier();
                        mBallStrategy.clear();
                        mUIPlayerWinMeterCtrl.setMeterValue(0);
                        mUIPlayerMultiTxtSprite.createFontString( "" + mMultiplier );
                        mUIPlayerLevelTxtSprite.createFontString( "Level " + mLevel );
                        mGoalMultiplier = mInitialMultiGoal;
                        uiGoalMultiSprite.createFontString("" + mGoalMultiplier);
                        mTimePointStart.now( GetDurationMilliseconds(mInitialStartTime) );

                        // Make the multiplier visible
                        if( mLevelState == ELS_IDLE )
                            mMultiStrategy.activateNode("strawberry");

                        if( DEBUG_AUTOPLAY )
                            debugTimer.reset();

                        mLevelState = ELS_ACTIVE;
                    }
                }
            }
        }
        else if( event.type == NEvents::SDL_MOUSEBUTTONUP )
        {
            if( !MenuMgr.isActive() && ActionMgr.wasAction(event, "drop_ball", NActionPress::UP) )
                dropBall( event.button.x );
        }
    }

    //
    //  Show the next ball
    //
    void dropBall( int x )
    {
        // Strictly a mouse only game which is why I'm using the button event x & y
        CPoint pos= mCamera.toOrthoCoord( x, 0 );
        CSprite @sprite = mBallStrategy.create(mBallAry[mNextBallIndex]).getSprite();
        sprite.setPhysicsTransform(pos.x, -1400);
        sprite.applyAngularImpulse(RandFloat(-0.5, 0.5));

        // Show the next ball
        showNextBall();
    }

    //
    //  Show the next ball
    //
    void showNextBall()
    {
        // Randomly generate the next ball
        int lastBallIndex = mNextBallIndex;
        mNextBallIndex = RandInt(0,mBallAry.length()-1);

        // Hide the last UI ball and show the next one
        if( lastBallIndex != mNextBallIndex )
        {
            if( lastBallIndex > -1 )
                mUIStrategy.deactivateNode( mBallAry[lastBallIndex] );
            mUIStrategy.activateNode( mBallAry[mNextBallIndex] );
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

        if( mLevelState == ELS_ACTIVE )
        {
            if(mMultiplier >= mGoalMultiplier)
            {
                mTimePointStart += GetDurationMilliseconds(mLevelUpIncMultiTime);
                mGoalMultiplier += mIncMultiGoal;
                uiGoalMultiSprite.createFontString("" + mGoalMultiplier);
                mUITimerSprite.prepare("reset_flash");
                uiGoalMultiSprite.prepare("reset_flash");
                mUIPlayerLevelTxtSprite.createFontString( "Level: " + ++mLevel );
                mUIPlayerLevelTxtSprite.prepare("reset_flash");
            }

            CTimePoint timePoint;
            CTimeDuration duration( mTimePointStart - timePoint );
            if( duration.getNanoseconds() > 0 )
            {
                mUITimerSprite.createFontString( FormatTimeDuration( mTimePointStart - timePoint, NTimeFormat::ETF_M_S ) );

                if( DEBUG_AUTOPLAY && debugTimer.expired(true) )
                {
                    int pos = RandInt(120, 590);
                    dropBall( pos + (mMultiXPosAllAry[mMultiIndexPos] / 4) );
                }
            }
            else
            {
                mUITimerSprite.createFontString( "00:00" );
                mLevelState = ELS_GAME_OVER;

                // Switch out the default menus, activate and transition tree's default menu
                MenuMgr.deactivateTree( "pause_tree" );
                MenuMgr.activateTree( "game_over_tree" );
                gameOverConfirmMenuInit();
                MenuMgr.transitionMenu( "game_over_tree" );
            }
        }
    }

    //
    //  Player Multiplier increase
    //
    void playerMultiInc( CSprite & sprite )
    {
        mMultiplier += sprite.getId();
        mUIPlayerMultiTxtSprite.createFontString( "" + mMultiplier );
        mUIPlayerMultiTxtSprite.prepare("inc_flash");
        mUIPlayerWinMeterCtrl.prepare("inc_flash");

        mTimePointStart += GetDurationMilliseconds(mMultiUpIncMultiTime);

        // Move the multiplier to a new position on the screen
        moveMultiplier();
    }

    //
    //  Move the multiplier to a new position on the screen
    //
    void moveMultiplier()
    {
        array<int> @posAry = mMultiXPosAry[mMultiIndexPos];
        int index = RandInt(0,posAry.length()-1);
        int offsetX = posAry[index];
        mMultiIndexPos = mMultiXPosAllAry.find(offsetX);
        mMultiSprite.setPhysicsTransform(offsetX, mMultiY);
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
        else if( spriteA.getId() == NLevelDefs::SPRITE_MULTI )
        {
            playerMultiInc( spriteB );
        }
        else if( spriteB.getId() == NLevelDefs::SPRITE_MULTI )
        {
            playerMultiInc( spriteA );
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

    //
    //  Setup confirm menu for game over
    //
    void gameOverConfirmMenuInit()
    {
        uiControl @yesBtn = MenuMgr.getMenu( "confirmation_menu" ).getControl( "yes_btn" );
        uiControl @noBtn = MenuMgr.getMenu( "confirmation_menu" ).getControl( "no_btn" );
        uiControl @msgLbl = MenuMgr.getMenu( "confirmation_menu" ).getControl( "message_lbl" );

        msgLbl.createFontString( "Game Over!|Do you want to|try again?" );

        yesBtn.addScriptFunction("execute", "(main)", "RestartGameMsg", false, false, true);
        yesBtn.setActionType( "back" );
        yesBtn.setExecutionAction( "" );

        noBtn.setActionType( "game_state_change" );
        noBtn.setExecutionAction( "title_screen_state" );
    }
};

//
//  Send message to restart game
//
void RestartGameMsg( uiControl & control )
{
    DispatchEvent( NLevelDefs::ELE_RESTART_GAME );
}

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
//  Ball AI Update script
//
void Level_BallAI( CSprite & sprite )
{
    while(true)
    {
        if( sprite.getPos().y > 1800.f )
        {
            StrategyMgr.getStrategy("_level_ball_").destroy(sprite.getHandle());
            break;
        }
        else if( (sprite.getPos().y > 1600.f) && !sprite.getParameters().isSet(NObjectParams::STATE1) )
        {
            // This will disable collision detection.
            // Need to do this so that this sprite, which is about to be deleted, 
            // isn't returned by the physics callback functions because it could be a dead pointer by then
            sprite.setPhysicsContactFilter(0);

            // Set the state so as to not enter this if again
            sprite.getParameters().add(NObjectParams::STATE1);

            // Dispatch message to bang this one up
            if( abs(sprite.getPos().x) < 720 )
                DispatchEvent(NLevelDefs::ELE_BANG_UP_AWARD);
        }

        Suspend();
    }
}

//
//  Time reset flash script
//
void Level_TimeResetFlash( CSprite & sprite )
{
    CColor hiColor = sprite.getDefaultColor();
    hiColor.transformHSV( 0, 1, 2 );

    CColor lowColor = sprite.getDefaultColor();
    lowColor.transformHSV( 0, 1, .6 );

    sprite.setColor( hiColor );

    NSpriteUtils::ColorTo( 120, lowColor, sprite );
    NSpriteUtils::ColorTo( 100, hiColor, sprite );
    NSpriteUtils::ColorTo( 200, sprite.getDefaultColor(), sprite );
}

//
//  Time reset flash script
//
void Level_MultiIncFlash( CSprite & sprite )
{
    CColor hiColor = sprite.getDefaultColor();
    hiColor.transformHSV( 0, 1, 2 );

    CColor lowColor = sprite.getDefaultColor();
    lowColor.transformHSV( 0, 1, .6 );

    sprite.setColor( hiColor );

    NSpriteUtils::ColorTo( 120, lowColor, sprite );
    NSpriteUtils::ColorTo( 100, hiColor, sprite );

    sprite.setColor( sprite.getDefaultColor() );
}

//
//  AI Update script
//
void Level_PegOff( CSprite & sprite )
{
    Hold( 200 );

    sprite.setFrame(0);
}