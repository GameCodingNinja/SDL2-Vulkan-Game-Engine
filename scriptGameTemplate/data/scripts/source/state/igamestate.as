
//
//  FILE NAME:  igamestate.as
//
//  DESC:       Game state interface class
//

class iGameState
{
    // Flag to indicate state change
    protected bool mChangeState = false;

    // Message to send to next state
    protected CStateMessage mStateMessage;

    // This objects current game state value
    protected NStateDefs::EGameState mGameState = NStateDefs::EGS_NULL;

    // This objects next game state value
    protected NStateDefs::EGameState mNextState = NStateDefs::EGS_NULL;

    //
    //  Default Constructor - required if defining a constructor with parameters
    //
    iGameState()
    {}

    //
    //  Constructor
    //
    iGameState( const NStateDefs::EGameState gameState, const NStateDefs::EGameState nextState )
    {
        mGameState = gameState;
        mNextState = nextState;
    }
    
    //
    //  Constructor
    //
    iGameState( const NStateDefs::EGameState gameState, const CStateMessage & stateMsg )
    {
        mGameState = gameState;
        mNextState = NStateDefs::EGS_GAME_LOAD;
        mStateMessage = stateMsg;
    }

    void init()
    {
        // Empty member to be overwritten
    }
    
    void destroy()
    {
        // Empty member to be overwritten
    }
    
    void handleEvent( const CEvent & event )
    {
        // Empty member to be overwritten
    }
    
    void physics()
    {
        // Empty member to be overwritten
    }

    void update()
    {
        // Empty member to be overwritten
    }
    
    void transform()
    {
        // Empty member to be overwritten
    }

    // Is the state done
    bool doStateChange()
    {
        return mChangeState;
    }

    // Get the current game state
    NStateDefs::EGameState getState()
    { return mGameState; }

    // Get the next game state
    NStateDefs::EGameState getNextState()
    { return mNextState; }

    // Get the message
    CStateMessage & getStateMessage()
    { return mStateMessage; }


    // Physics callbacks
    void beginContact( CSprite & spriteA, CSprite & spriteB )
    {
        // Empty member to be overwritten
    }

    void endContact( CSprite & spriteA, CSprite & spriteB )
    {
        // Empty member to be overwritten
    }

    void destroyFixture( CSprite & sprite )
    {
        // Empty member to be overwritten
    }

    void destroyJoint( CSprite & sprite )
    {
        // Empty member to be overwritten
    }
};
