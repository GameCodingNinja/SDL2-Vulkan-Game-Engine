
/************************************************************************
*    FILE NAME:       scriptsettings.cpp
*
*    DESCRIPTION:     CSoundMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsettings.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSettings
{
    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType( "CSettings", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getResolution()",        asMETHOD(CSettings, getResolution),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getSize()",              asMETHOD(CSettings, getSize),              asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getDefaultSize()",       asMETHOD(CSettings, getDefaultSize),       asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getSizeHalf()",          asMETHOD(CSettings, getSizeHalf),          asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getNativeSize()",        asMETHOD(CSettings, getNativeSize),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getDefaultSizeHalf()",   asMETHOD(CSettings, getDefaultSizeHalf),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setSize(const CSize & in)",       asMETHOD(CSettings, getDefaultSizeHalf),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getScreenAspectRatio()", asMETHOD(CSettings, getScreenAspectRatio), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getOrthoAspectRatio()",  asMETHOD(CSettings, getOrthoAspectRatio),  asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool getVSync()",                      asMETHOD(CSettings, getVSync),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setVSync(bool)",                  asMETHOD(CSettings, setVSync),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool getFullScreen()",                 asMETHOD(CSettings, getFullScreen),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setFullScreen(bool)",             asMETHOD(CSettings, setFullScreen),        asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getSectorSize()",                  asMETHOD(CSettings, getSectorSize),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getSectorSizeHalf()",              asMETHOD(CSettings, getSectorSizeHalf),    asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getAnisotropicLevel()",            asMETHOD(CSettings, getAnisotropicLevel),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setAnisotropicLevel(int)",        asMETHOD(CSettings, setAnisotropicLevel),  asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isDebugMode()",                   asMETHOD(CSettings, isDebugMode),          asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isDebugAsMobile()",               asMETHOD(CSettings, isDebugAsMobile),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isMobileDevice()",                asMETHOD(CSettings, isMobileDevice),       asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getGamePadStickDeadZone()",        asMETHOD(CSettings, getGamePadStickDeadZone), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setGamePadStickDeadZone(int)",    asMETHOD(CSettings, setGamePadStickDeadZone), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "void saveSettings()",                  asMETHOD(CSettings, saveSettings), asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CSettings Settings", &CSettings::Instance()) );
    }
}
