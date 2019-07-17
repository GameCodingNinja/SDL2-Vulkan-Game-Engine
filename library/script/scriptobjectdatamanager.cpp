
/************************************************************************
*    FILE NAME:       scriptobjectdatamanager.cpp
*
*    DESCRIPTION:     CObjectDataMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptobjectdatamanager.h>

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptObjectDataManager
{
    /************************************************************************
    *    DESC:  Load the data list table                                                            
    ************************************************************************/
    void LoadListTable( const std::string & filePath, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.loadListTable( filePath );
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
    *    DESC:  Load all of the meshes and materials of a specific data group                                                           
    ************************************************************************/
    void LoadGroup( const std::string & group, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.loadGroup( group );
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
    *    DESC:  Free all of the meshes and materials of a specific data group                                                           
    ************************************************************************/
    void FreeGroup( const std::string & group, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.freeGroup( group );
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
        Throw( pEngine->RegisterObjectType( "CObjectDataMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadListTable(string &in)", asFUNCTION(LoadListTable), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadGroup(string &in)",     asFUNCTION(LoadGroup), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeGroup(string &in)",     asFUNCTION(FreeGroup), asCALL_CDECL_OBJLAST) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CObjectDataMgr ObjectDataMgr", &CObjectDataMgr::Instance()) );
    }
}