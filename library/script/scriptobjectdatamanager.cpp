
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
#include <autowrapper/aswrappedcall.h>
#include <scriptarray/scriptarray.h>

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

    void LoadGroupAry( const CScriptArray & groupAry, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.loadGroupAry( groupAry );
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

    void FreeGroupAry( const CScriptArray & groupAry, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.freeGroupAry( groupAry );
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
        
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadListTable(string &in)",       WRAP_OBJ_LAST(LoadListTable), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadGroup(string &in)",           WRAP_OBJ_LAST(LoadGroup),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadGroupAry(array<string> &in)", WRAP_OBJ_LAST(LoadGroupAry),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeGroup(string &in)",           WRAP_OBJ_LAST(FreeGroup),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeGroupAry(array<string> &in)", WRAP_OBJ_LAST(FreeGroupAry),  asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CObjectDataMgr ObjectDataMgr", &CObjectDataMgr::Instance()) );
    }
}