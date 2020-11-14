
/************************************************************************
*    FILE NAME:       physicsworld3d.h
*
*    DESCRIPTION:     Wrapper class for Bullet physics
************************************************************************/

#pragma once

// Bullet Physics lib dependencies
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

// Game lib dependencies
#include <common/point.h>

// Forward declaration(s)
struct XMLNode;

class CPhysicsWorld3D
{
public:

    // Constructor
    CPhysicsWorld3D();

    // Destructor
    virtual ~CPhysicsWorld3D();

    // Load the physics world from the passed in node
    void loadFromNode( const XMLNode & node );

    // Get the physics world
    const btDiscreteDynamicsWorld & getWorld() const;

    // Add a rigid body to the world
    void addRigidBody( btRigidBody * pBody );

    // Add collision filtering
    void addRigidBody( btRigidBody * pBody, short filterGroup, short mask );

    // Remove a rigid body to the world
    void removeRigidBody( btRigidBody * pBody );

    // Perform fixed time step physics simulation
    void fixedTimeStep();
    
    // Perform variable time step physics simulation
    void variableTimeStep();

    // Set the fps to run the simulation at
    void setFPS( float fps );

    // The the time ratio
    float getTimeRatio() const;

    // Set-Get the activity of the physics world
    void setActive( bool value );
    bool isActive() const;

private:

    // Bullet Physics world members
    btDefaultCollisionConfiguration m_defColConf;
    btDbvtBroadphase m_broadphase;
    btSequentialImpulseConstraintSolver m_conSolv;
    btCollisionDispatcher m_colDisp;
    btDiscreteDynamicsWorld m_world;

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
};
