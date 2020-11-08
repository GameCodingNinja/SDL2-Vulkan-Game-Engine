
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
    void handleEvent( const CEvent & event ) override
    {
        // Check for the "game change state" message
        if( (event.type == NMenuEvent::GAME_STATE_CHANGE) && (event.user.code == NTransCode::BEGIN) )
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


/************************************************************************
*    DESC:  Do a fade in and out
************************************************************************/
void State_StartUpFadeIn()
{
    // This is initialized in startupstate.as
    CVisual visual;

    Hold( 500 );
            
    CColor final(1,1,1,1);
    NVisualUtils::ColorTo( 500, final, visual );
    
    DispatchEvent( NStateDefs::ESE_FADE_IN_COMPLETE );
}


void State_FadeIn()
{
    CVisual visual;
    visual.setAdditiveColor(0,0,0,1);
            
    CColor final(1,1,1,1);
    NVisualUtils::ColorTo( 500, final, visual );
    
    DispatchEvent( NStateDefs::ESE_FADE_IN_COMPLETE );
}

void State_FadeOut()
{
    CVisual visual;
    
    CColor current(0,0,0,1);
    NVisualUtils::ColorTo( 500, current, visual );
    
    DispatchEvent( NStateDefs::ESE_FADE_OUT_COMPLETE );
}


/************************************************************************
*    DESC:  Looping Animation
************************************************************************/
void State_LoopAnimation( CSprite & sprite )
{
    NSpriteUtils::Play( 12, sprite, true );
}


/************************************************************************
*    DESC:  Cube rotate
************************************************************************/
void State_RotateCube( CSprite & sprite )
{
    while( true )
    {
        float rot = HighResTimer.getElapsedTime() * 0.04;
        sprite.incRot( rot, rot, 0 );

        Suspend();
    }
}