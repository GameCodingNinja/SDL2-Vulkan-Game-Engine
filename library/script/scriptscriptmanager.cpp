
/************************************************************************
*    FILE NAME:       scriptscriptmanager.cpp
*
*    DESCRIPTION:     CScriptMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptscriptmanager.h>

// Game lib dependencies
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptScriptManager
{
    /************************************************************************
    *    DESC:  Load the script group                                                            
    ************************************************************************/
    void LoadGroup( const std::string & group, CScriptMgr & rScriptMgr )
    {
        try
        {
            rScriptMgr.loadGroup( group );
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
    *    DESC:  Free the script group                                                            
    ************************************************************************/
    void FreeGroup( const std::string & group, CScriptMgr & rScriptMgr )
    {
        try
        {
            rScriptMgr.freeGroup( group );
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
    *    DESC:  Clear all the scripts
    ************************************************************************/
    void Clear( CScriptMgr & rScriptMgr )
    {
        try
        {
            rScriptMgr.clear();
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
        Throw( pEngine->RegisterObjectType( "CScriptMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CScriptMgr", "void loadGroup(string &in)",       WRAP_OBJ_LAST(LoadGroup),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CScriptMgr", "void freeGroup(string &in)",       WRAP_OBJ_LAST(FreeGroup),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CScriptMgr", "void clear()",                     WRAP_OBJ_LAST(Clear),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CScriptMgr", "void setMaxPoolPercentage(float)", WRAP_MFN(CScriptMgr, setMaxPoolPercentage), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CScriptMgr ScriptMgr", &CScriptMgr::Instance()) );
    }
}
