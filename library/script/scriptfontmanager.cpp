
/************************************************************************
*    FILE NAME:       scriptfontmanager.cpp
*
*    DESCRIPTION:     CMenuMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptfontmanager.h>

// Game lib dependencies
#include <managers/fontmanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptFontManager
{
    /************************************************************************
    *    DESC:  Load the data list table
    ************************************************************************/
    void Load( const std::string & filePath, CFontMgr & rFontMgr )
    {
        try
        {
            rFontMgr.load( filePath );
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

        Throw( pEngine->RegisterObjectMethod("CFontMgr", "void load(string &in)", WRAP_OBJ_LAST(Load), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CFontMgr FontMgr", &CFontMgr::Instance()) );
    }
}
