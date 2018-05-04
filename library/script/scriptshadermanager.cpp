
/************************************************************************
*    FILE NAME:       scriptshadermanager.cpp
*
*    DESCRIPTION:     CShaderMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptshadermanager.h>

// Game lib dependencies
#include <managers/shadermanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptShaderManager
{
    /************************************************************************
    *    DESC:  Load the shader from xml file path                                                        
    ************************************************************************/
    void Load( const std::string & filePath, CShaderMgr & rShaderMgr )
    {
        try
        {
            rShaderMgr.load( filePath );
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
        Throw( pEngine->RegisterObjectType( "CShaderMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CShaderMgr", "void load(string &in)",                                                   asFUNCTION(Load), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CShaderMgr", "void setAllShaderColor(string &in, CColor &in)",                          asMETHODPR(CShaderMgr, setAllShaderColor, (const std::string &, const CColor &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CShaderMgr", "void setAllShaderColor(string &in, float, float, float, float)",          asMETHODPR(CShaderMgr, setAllShaderColor, (const std::string &, float, float, float, float), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CShaderMgr", "void setShaderColor(string &in, string &in, CColor &in)",                 asMETHODPR(CShaderMgr, setShaderColor, (const std::string &, const std::string &, const CColor &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CShaderMgr", "void setShaderColor(string &in, string &in, float, float, float, float)", asMETHODPR(CShaderMgr, setShaderColor, (const std::string &, const std::string &, float, float, float, float), void), asCALL_THISCALL) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CShaderMgr ShaderMgr", &CShaderMgr::Instance()) );
    }
}
