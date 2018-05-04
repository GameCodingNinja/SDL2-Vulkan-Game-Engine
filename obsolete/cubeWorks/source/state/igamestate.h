
/************************************************************************
*    FILE NAME:       igamestate.h
*
*    DESCRIPTION:     gamestate interface Class
************************************************************************/

#ifndef __i_game_state_h__
#define __i_game_state_h__

// STL lib dependencies
#include <vector>
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

// Forward declaration(s)
union SDL_Event;

enum EGameState
{
    EGS_NULL,
    EGS_STARTUP,
    EGS_TITLE_SCREEN,
    EGS_GAME_LOAD,
    EGS_RUN
};


class CStateMessage
{
public:

    CStateMessage() : m_nextState(EGS_NULL), m_lastState(EGS_NULL)
    {}

    std::vector<std::string> m_groupLoad;
    std::vector<std::string> m_groupUnload;
    
    std::string m_physicsWorldCreate;
    std::string m_physicsWorldDestroy;

    std::vector<std::string> m_createActor;
    std::vector<std::string> m_createActorVec;

    std::vector< boost::tuple<std::string, int> > m_createMegaTexture;

    std::vector<std::string> m_createInstanceMesh;

    std::string m_stage2DLoad;

    std::string m_hudLoad;

    std::string m_actorDataLoad;

    std::string m_generatorLoadPath;

    EGameState m_nextState;
    EGameState m_lastState;

    void Clear()
    {
        m_groupLoad.clear();
        m_groupUnload.clear();
        m_physicsWorldCreate.clear();
        m_physicsWorldDestroy.clear();
        
        m_createActor.clear();
        m_createActorVec.clear();
        m_createMegaTexture.clear();
        m_createInstanceMesh.clear();
        m_stage2DLoad.clear();
        m_hudLoad.clear();
        m_actorDataLoad.clear();
        m_generatorLoadPath.clear();
        
        m_nextState = EGS_NULL;
        m_lastState = EGS_NULL;
    }
};


class iGameState : public boost::noncopyable
{
public:

    // Constructor
    iGameState() : m_gameState(EGS_NULL), m_nextState(EGS_NULL){};

    // Constructor
    iGameState( const CStateMessage & stateMsg ){};

    // Destructor
    virtual ~iGameState(){};

    // Handle events
    virtual void HandleEvent( const SDL_Event & rEvent ){};

    // Act upon what the user is doing
    virtual void GetUserInput( bool hasFocus ){};

    // Handle the physics
    virtual void Physics(){};

    // Update objects that require them
    virtual void Update(){};

    // Transform the game objects
    virtual void Transform(){};

    // 2D/3D Render of game content
    virtual void PreRender(){};
    virtual void PostRender(){};

    // Is the state done
    virtual bool DoStateChange()
    { return false; };

    // Get the currect game state
    EGameState GetState()
    { return m_gameState; }

    // Get the next game state
    virtual EGameState GetNextState()
    { return m_nextState; }

    // Get the message
    const CStateMessage & GetStateMessage()
    { return m_stateMessage; }

protected:

    // Message to send to next state
    CStateMessage m_stateMessage;

    // This objects current game state value
    EGameState m_gameState;

    // This objects next game state value
    EGameState m_nextState;

};

#endif  // __i_game_state_h__


