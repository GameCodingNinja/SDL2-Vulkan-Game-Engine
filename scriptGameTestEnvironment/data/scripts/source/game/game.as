
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
        }
    }
    
    //
    //  Main game loop
    //
    bool gameLoop()
    {
        handleEvent();
        
        if( mGameRunning )
        {
            // Do the update
            StrategyMgr.update();
            
            // Do the transform
            StrategyMgr.transform();
            
            // Do the rendering
            Device.render();
        }
        
        return mGameRunning;
    }
};
