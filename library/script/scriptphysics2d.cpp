
/************************************************************************
*    FILE NAME:       scriptphysics2d.cpp
*
*    DESCRIPTION:     CPhysicsWorld2D & CPhysicsWorldManager2D script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptphysics2d.h>

// Game lib dependencies
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <scriptarray/scriptarray.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptPhysics2d
{
    /************************************************************************
    *    DESC:  Load the data list table                                                            
    ************************************************************************/
    void LoadListTable( const std::string & filePath, CPhysicsWorldManager2D & rPhysicsMgr )
    {
        try
        {
            rPhysicsMgr.loadListTable( filePath );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CPhysicsWorld2D", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CPhysicsWorld2D", "void fixedTimeStep()",                                 WRAP_MFN(CPhysicsWorld2D, fixedTimeStep),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorld2D", "void variableTimeStep()",                              WRAP_MFN(CPhysicsWorld2D, variableTimeStep),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorld2D", "void EnableContactListener( bool enable = true )",     WRAP_MFN(CPhysicsWorld2D, EnableContactListener),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorld2D", "void EnableDestructionListener( bool enable = true )", WRAP_MFN(CPhysicsWorld2D, EnableDestructionListener), asCALL_GENERIC) );

        // Register type
        Throw( pEngine->RegisterObjectType( "CPhysicsWorldManager2D", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void loadListTable(string &in)",         WRAP_OBJ_LAST(LoadListTable),                   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void createWorld(string &in)",           WRAP_MFN(CPhysicsWorldManager2D, createWorld),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void destroyWorld(string &in)",          WRAP_MFN(CPhysicsWorldManager2D, destroyWorld), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "CPhysicsWorld2D & getWorld(string &in)", WRAP_MFN(CPhysicsWorldManager2D, getWorld),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void clear()",                           WRAP_MFN(CPhysicsWorldManager2D, clear),        asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CPhysicsWorldManager2D PhysicsWorldManager2D", &CPhysicsWorldManager2D::Instance()) );
    }
}
