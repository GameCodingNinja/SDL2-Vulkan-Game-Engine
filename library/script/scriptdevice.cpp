
/************************************************************************
*    FILE NAME:       scriptdevice.cpp
*
*    DESCRIPTION:     CDevice script object registration
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
#include <autowrapper/aswrappedcall.h>

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
        
        Throw( pEngine->RegisterObjectMethod("CDevice", "void create(string &in)",                        WRAP_OBJ_LAST(Create),                     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void destroy()",                                 WRAP_MFN(CDevice, destroy),                asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void deleteCommandPoolGroup(string &in)",        WRAP_MFN(CDevice, deleteCommandPoolGroup), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void waitForIdle()",                             WRAP_MFN(CDevice, waitForIdle),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void showWindow(bool visible = true)",           WRAP_MFN(CDevice, showWindow),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void render()",                                  WRAP_MFN(CDevice, render),                 asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void changeResolution(CSize & in, bool)",        WRAP_MFN(CDevice, changeResolution),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void setFullScreen(bool)",                       WRAP_MFN(CDevice, setFullScreen),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CDevice", "void setClearColor(float, float, float, float)", WRAP_MFN(CDevice, setClearColor),          asCALL_GENERIC) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CDevice Device", &CDevice::Instance()) );
    }
}
