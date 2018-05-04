
/************************************************************************
*    FILE NAME:       physicsworld.h
*
*    DESCRIPTION:     Box2D b2World wrapper class singleton
************************************************************************/

#ifndef __physics_world_manager_h__
#define __physics_world_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <map>

// Forward declaration(s)
class CPhysicsWorld2D;
class CPhysicsWorld3D;

class CPhysicsWorldManager : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CPhysicsWorldManager & Instance()
    {
        static CPhysicsWorldManager worldManager;
        return worldManager;
    }

    // Create the physics world
    void CreateWorld2D( const std::string & group );
    void CreateWorld3D( const std::string & group );
    
    // Destroy the physics world
    void DestroyWorld2D( const std::string & group );
    void DestroyWorld3D( const std::string & group );

    // Get the physics world
    CPhysicsWorld2D & GetWorld2D( const std::string & group );
    CPhysicsWorld3D & GetWorld3D( const std::string & group );
    
    // Delete all worlds
    void Clear();

private:

    // Constructor
    CPhysicsWorldManager();

    // Destructor
    ~CPhysicsWorldManager();

    // Load the physics worlds from an XML
    void LoadFromXML2D( const std::string & group, const std::string & LoadFromXML );
    void LoadFromXML3D( const std::string & group, const std::string & LoadFromXML );

private:

    // Map of physics worlds
    std::map<const std::string, CPhysicsWorld2D> m_pWorld2dMap;
    std::map<const std::string, CPhysicsWorld3D> m_pWorld3dMap;

};

#endif  // __physics_world_manager_h__