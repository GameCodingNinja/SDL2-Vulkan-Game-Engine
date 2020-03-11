
//
//  FILE NAME:  game.as
//
//  DESC:       CGame function
//

class CGame
{
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
        // Sprites allocate device data and need to be freed
        StrategyMgr.clear();
        
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
        
        // Load the data lists
        ObjectDataMgr.loadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
        StrategyMgr.loadListTable( "data/objects/strategy/strategyListTable.lst" );
        
        // Load the action manager config settings
        ActionMgr.load( "data/settings/controllerMapping.cfg" );
        
        // Load group specific assets
        ObjectDataMgr.loadGroup( "(main)" );

        // Create the needed strategies
        StrategyMgr.loadStrategy( "data/objects/strategy/strategy.loader" );
    }
    
    //
    //  Handle the events
    //
    void handleEvent()
    {
        PollEvents();

        while(true)
        {
            const CEvent @event = ActionMgr.pollEvent();

            if(event.type > 0)
            {
                if( event.type == NGameDefs::EGE_QUIT ||
                    event.type == NGameDefs::EGE_APP_TERMINATING )
                {
                    mGameRunning = false;

                    // Hide the window
                    Device.showWindow(false);
                }
            }
            else
                break;
        };
    }
    
    //
    //  Main game loop
    //
    bool gameLoop()
    {
        handleEvent();

        // Get our elapsed time
        HighResTimer.calcElapsedTime();
        
        if( mGameRunning )
        {
            // Do the update
            StrategyMgr.update();

            // Suspend to allow sprite and other update scripts to be executed
            Suspend();
            
            // Do the transform
            StrategyMgr.transform();
            
            // Do the rendering
            Device.render();

            // Inc the stat cycle
            if( Settings.isDebugMode() )
                StatCounter.incCycle();
        }
        
        return mGameRunning;
    }
};
