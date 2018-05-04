
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
    void LoadGroup2D( const std::string & group, bool createFromData, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.loadGroup2D( group, createFromData );
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

    void LoadGroup3D( const std::string & group, bool createFromData, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.loadGroup3D( group, createFromData );
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
    *    DESC:  Create the group's VBO, IBO, textures, etc                                                           
    ************************************************************************/
    void CreateFromData2D( const std::string & group, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.createFromData2D( group );
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
    
    void CreateFromData3D( const std::string & group, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.createFromData2D( group );
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
    void FreeGroup2D( const std::string & group, bool freeOpenGLObjects, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.freeGroup2D( group, freeOpenGLObjects );
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
    
    void FreeGroup3D( const std::string & group, bool freeOpenGLObjects, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.freeGroup3D( group, freeOpenGLObjects );
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
    void FreeOpenGL2D( const std::string & group, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.freeOpenGL2D( group );
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
    
    void FreeOpenGL3D( const std::string & group, CObjectDataMgr & rObjectDataMgr )
    {
        try
        {
            rObjectDataMgr.freeOpenGL3D( group );
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
        
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadListTable(string &in)",                              asFUNCTION(LoadListTable), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadGroup2D(string &in, bool createFromData = true)",    asFUNCTION(LoadGroup2D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void loadGroup3D(string &in, bool createFromData = true)",    asFUNCTION(LoadGroup3D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void createFromData2D(string &in)",                           asFUNCTION(CreateFromData2D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void createFromData3D(string &in)",                           asFUNCTION(CreateFromData3D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeGroup2D(string &in, bool freeOpenGLObjects = true)", asFUNCTION(FreeGroup2D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeGroup3D(string &in, bool freeOpenGLObjects = true)", asFUNCTION(FreeGroup3D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeOpenGL2D(string &in)",                               asFUNCTION(FreeOpenGL2D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CObjectDataMgr", "void freeOpenGL3D(string &in)",                               asFUNCTION(FreeOpenGL3D), asCALL_CDECL_OBJLAST) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CObjectDataMgr ObjectDataMgr", &CObjectDataMgr::Instance()) );
    }
}