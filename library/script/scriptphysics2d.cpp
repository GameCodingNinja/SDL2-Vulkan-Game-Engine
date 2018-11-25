
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

        Throw( pEngine->RegisterObjectMethod("CPhysicsWorld2D", "void fixedTimeStep()",    asMETHOD(CPhysicsWorld2D, fixedTimeStep),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorld2D", "void variableTimeStep()", asMETHOD(CPhysicsWorld2D, variableTimeStep),   asCALL_THISCALL) );
        
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CPhysicsWorldManager2D", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void loadListTable(string &in)",         asFUNCTION(LoadListTable), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void createWorld(string &in)",           asMETHOD(CPhysicsWorldManager2D, createWorld), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void destroyWorld(string &in)",          asMETHOD(CPhysicsWorldManager2D, destroyWorld), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "CPhysicsWorld2D & getWorld(string &in)", asMETHOD(CPhysicsWorldManager2D, destroyWorld), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPhysicsWorldManager2D", "void clear()",                           asMETHOD(CPhysicsWorldManager2D, clear), asCALL_THISCALL) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CPhysicsWorldManager2D PhysicsWorldManager2D", &CPhysicsWorldManager2D::Instance()) );
    }
}
