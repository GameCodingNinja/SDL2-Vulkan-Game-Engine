
/************************************************************************
*    FILE NAME:       scriptdevice.cpp
*
*    DESCRIPTION:     CHighResTimer script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptdevice.h>

// Game lib dependencies
#include <system/device.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptDevice
{
    /************************************************************************
    *    DESC:  Create a basic sprite strategy                                                            
    ************************************************************************/
    void Create( const std::string & pipelineCfg, CDevice & rDevice )
    {
        try
        {
            rDevice.create( pipelineCfg );
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
        Throw( pEngine->RegisterObjectType( "CDevice", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CDevice", "void create(string &in)",                 asFUNCTION(Create), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void destroy()",                          asMETHOD(CDevice, destroy), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void deleteCommandPoolGroup(string &in)", asMETHOD(CDevice, deleteCommandPoolGroup), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void waitForIdle()",                      asMETHOD(CDevice, waitForIdle), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void showWindow(bool visible = true)",    asMETHOD(CDevice, showWindow), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void render()",                           asMETHOD(CDevice, render), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void changeResolution(CSize & in, bool)", asMETHOD(CDevice, changeResolution), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void setFullScreen(bool)",                asMETHOD(CDevice, setFullScreen), asCALL_THISCALL) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CDevice Device", &CDevice::Instance()) );
    }
}
