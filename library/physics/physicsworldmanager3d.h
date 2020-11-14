
/************************************************************************
*    FILE NAME:       physicsworldmanager3d.h
*
*    DESCRIPTION:     Physics manager class singleton
************************************************************************/

#pragma once

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <map>

// Forward declaration(s)
class CPhysicsWorld3D;

class CPhysicsWorldManager3D : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CPhysicsWorldManager3D & Instance()
    {
        static CPhysicsWorldManager3D worldManager;
        return worldManager;
    }

    // Create the physics world
    void createWorld( const std::string & group );
    
    // Destroy the physics world
    void destroyWorld( const std::string & group );

    // Get the physics world
    CPhysicsWorld3D & getWorld( const std::string & group );
    
    // Delete all worlds
    void clear();

private:

    // Constructor
    CPhysicsWorldManager3D();

    // Destructor
    ~CPhysicsWorldManager3D();

    // Load the physics worlds from an XML
    void load( const std::string & group, const std::string & filePath );

private:

    // Map of physics worlds
    std::map<const std::string, CPhysicsWorld3D> m_pWorld3dMap;
};
