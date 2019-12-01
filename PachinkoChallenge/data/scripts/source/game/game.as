
//
//  FILE NAME:  game.as
//
//  DESC:       CGame function
//

class CGame
{
    // State handle
    iGameState @mGameState;
    
    // Game running flag
    bool mGameRunning = true;
    
    //
    //  Constructor
    //
    CGame()
    {
    }
    
    //
    //  Do the cleanup.
    //  NOTE: Can't be called from destructor
    //
    void destroy()
    {
        // Destroy the game state
        mGameState.destroy();
        
        // Free the menu assets
        MenuMgr.freeGroup( "(menu)" );
        
        // Destroy the window and render device instance
        Device.destroy();
    }
    
    //
    //  Init the game
    //
    void init()
    {
        // Create the rendering device
        Device.create( "data/shaders/pipeline.cfg" );
        
        // Show the window
        Device.showWindow();

        // Create the startup state
        @mGameState = CStartUpState();
        mGameState.init();
    }
    
    //
    //  Handle the state change
    //
    void doChangeState()
    {
        if( mGameState.doStateChange() )
        {
            // Get the game state we are moving to
            const NStateDefs::EGameState curState = mGameState.getState();

            // Get the game state we are moving to
            const NStateDefs::EGameState nextState = mGameState.getNextState();

            // Get the message to the next state
            const CStateMessage stateMessage = mGameState.getStateMessage();
            
            // Free the current state assets
            mGameState.destroy();

            if( nextState == NStateDefs::EGS_TITLE_SCREEN )
                @mGameState = CTitleScreenState();

            else if( nextState == NStateDefs::EGS_GAME_LOAD )
                @mGameState = CLoadState( stateMessage );

            else if( nextState == NStateDefs::EGS_RUN )
                @mGameState = CRunState();

            // Do any pre-game loop init's
            mGameState.init();
        }
    }
    
    //
    //  Handle the events
    //
    void handleEvent()
    {
        PollEvents();

        if( !ActionMgr.isQueueEmpty() )
        {
            // Is it time to quit?
            if( ActionMgr.wasEvent( NGameDefs::EGE_QUIT ) ||
                ActionMgr.wasEvent( NGameDefs::EGE_APP_TERMINATING ) )
            {
                mGameRunning = false;

                // Show the window
                Device.showWindow(false);
            }

            mGameState.handleEvent();
        }
    }
    
    //
    //  Main game loop
    //
    bool gameLoop()
    {
        doChangeState();
        
        handleEvent();

        // Get our elapsed time
        HighResTimer.calcElapsedTime();
        
        if( mGameRunning )
        {
            // Handle the physics
            mGameState.physics();

            // Suspend to allow physics callback scripts to be executed
            Suspend();
            
            // Update animations, Move sprites, Check for collision
            mGameState.update();

            // Suspend to allow sprite and other update scripts to be executed
            Suspend();
            
            // Transform game objects
            mGameState.transform();
            
            // Do the rendering
            Device.render();

            // Inc the stat cycle
            if( Settings.isDebugMode() )
                StatCounter.incCycle();
        }
        
        return mGameRunning;
    }

    /************************************************************************
    *    DESC:  Physics callbacks
    ************************************************************************/
    void beginContact( CSprite & spriteA, CSprite & spriteB )
    {
        mGameState.beginContact( spriteA, spriteB );
    }

    void endContact( CSprite & spriteA, CSprite & spriteB )
    {
        mGameState.endContact( spriteA, spriteB );
    }

    void destroyFixture( CSprite & sprite )
    {
        mGameState.destroyFixture( sprite );
    }

    void destroyJoint( CSprite & sprite )
    {
        mGameState.destroyJoint( sprite );
    }
};
