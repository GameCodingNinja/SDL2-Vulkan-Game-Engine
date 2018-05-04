
/************************************************************************
*    FILE NAME:       scriptfontmanager.cpp
*
*    DESCRIPTION:     CMenuMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptshadermanager.h>

// Game lib dependencies
#include <managers/fontmanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptFontManager
{
    /************************************************************************
    *    DESC:  Load the data list table
    ************************************************************************/
    void Load( const std::string & filePath, const bool createFromData, CFontMgr & rFontMgr )
    {
        try
        {
            rFontMgr.load( filePath, createFromData );
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
    *    DESC:  Create font from data
    ************************************************************************/
    void CreateFromData( CFontMgr & rFontMgr )
    {
        try
        {
            rFontMgr.createFromData();
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
        Throw( pEngine->RegisterObjectType( "CFontMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CFontMgr", "void load(string &in, bool createFromData = true)", asFUNCTION(Load), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CFontMgr", "void CreateFromData()", asFUNCTION(CreateFromData), asCALL_CDECL_OBJLAST) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CFontMgr FontMgr", &CFontMgr::Instance()) );
    }
}
