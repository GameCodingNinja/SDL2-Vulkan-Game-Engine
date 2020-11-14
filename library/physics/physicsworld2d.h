
/************************************************************************
*    FILE NAME:       physicsworld2d.h
*
*    DESCRIPTION:     Wrapper class for Box2D's b2World
************************************************************************/

#pragma once

// Game lib dependencies
#include <Box2D/Box2D.h>
#include <common/point.h>

// Standard lib dependencies
#include <string>
#include <tuple>
#include <unordered_set>

// Forward declaration(s)
struct XMLNode;

class CPhysicsWorld2D : public b2ContactListener, b2DestructionListener
{
public:

    // Constructor
    CPhysicsWorld2D();

    // Destructor
    virtual ~CPhysicsWorld2D();

    // Load the physics world from the passed in node
    void loadFromNode( const XMLNode & node );

    // Get the physics world
    const b2World & getWorld() const;
    b2World & getWorld();

    // Create and destroy a physics body
    b2Body * createBody( const b2BodyDef & pDef );
    void destroyBody( b2Body * pBody );

    // Perform fixed time step physics simulation
    void fixedTimeStep();
    
    // Perform variable time step physics simulation
    void variableTimeStep();

    // Get the focus
    const CPoint<int> & getFocus() const;

    // Set the fps to run the simulation at
    void setFPS( float fps );

    // The the time ratio
    float getTimeRatio() const;

    // Set-Get the activity of the physics world
    void setActive( bool value );
    bool isActive() const;
    
    // Get the pixels per meter
    double getPixelsPerMeter() const;

    // Enable/disable the listeners
    void EnableContactListener( bool enable = true );
    void EnableDestructionListener( bool enable = true );

private:

    // Called when two fixtures begin to touch
    void BeginContact(b2Contact* contact) override;
    
    // Called when two fixtures cease to touch
    void EndContact(b2Contact* contact) override;

    // Called when any fixture is about to be destroyed
    void SayGoodbye(b2Fixture* fixture) override;
    
    // Called when any joint is about to be destroyed
    void SayGoodbye(b2Joint* joint) override;

private:

    // Box2D world
    b2World m_world;

    // World focus point
    CPoint<int> m_focus;

    // All bodies that are handled by this physics world
    // NOTE: Class doesn't not own the data. Do Not Delete!
    std::unordered_set<b2Body *> m_pBodySet;

    // If we're actively running simulations
    bool m_active;

    // If we're going to start a step this frame
    bool m_beginStep;

    // Timer to let us know when to do another step
    float m_timer;

    // The ammount of time to simulate in milliseconds
    float m_stepTime;

	// The ammount of time to simulate in Seconds
    float m_stepTimeSec;

    // The ratio of time between steps
    float m_timeRatio;

    // The number of velocity and position steps to calculate
    int m_velStepCount;
    int m_posStepCount;
    
    // pixels per meter scaler
    float m_pixelsPerMeter;

    // Listener members
    std::tuple<std::string, std::string> m_beginContactTuple;
    std::tuple<std::string, std::string> m_endContactTuple;
    std::tuple<std::string, std::string> m_deleteFixtureTuple;
    std::tuple<std::string, std::string> m_deleteJointTuple;
};
