
//
//  FILE NAME:  commonstate.as
//
//  DESC:       Common game state
//

class CCommonState : iGameState
{
    dictionary mGameStateDict;
    
    //
    //  Constructor
    //
    CCommonState( const NStateDefs::EGameState gameState, const NStateDefs::EGameState nextState )
    {
        super( gameState, nextState );
        
        mGameStateDict.set("title_screen_state", NStateDefs::EGS_TITLE_SCREEN);
        mGameStateDict.set("level_1_state", NStateDefs::EGS_RUN);
    }
    
    //
    //  Constructor
    //
    CCommonState( const NStateDefs::EGameState gameState, const CStateMessage & stateMsg )
    {
        super( gameState, stateMsg );
    }
    
    //
    //  Handle the events
    //
    void handleEvent() override
    {
        // Check for the "game change state" message
        if( ActionMgr.wasGameEvent( NMenuDefs::EME_MENU_GAME_STATE_CHANGE, NMenuDefs::ETC_BEGIN ) )
        {
            // Block all message processing in the menu manager
            MenuMgr.allow( false );
            
            // Set the message to load and unload the states
            getStateMessage().setMsg(
                NStateDefs::EGameState(mGameStateDict[MenuMgr.getActiveMenu().getActiveControl().getExecutionAction()]),
                getState() );
        }
    }
    
    //
    //  Update animations, Move sprites, Check for collision
    //
    void update() override
    {
        if( !MenuMgr.isActive() || mGameState == NStateDefs::EGS_TITLE_SCREEN )
            StrategyMgr.update();
        
        MenuMgr.update();
    }
    
    //
    //  Transform game objects
    //
    void transform() override
    {
        MenuMgr.transform();
        StrategyMgr.transform();
    }
};
