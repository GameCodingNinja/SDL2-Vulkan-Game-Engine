
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
#include <autowrapper/aswrappedcall.h>

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
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getSize()",              WRAP_MFN(CSettings, getSize),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getDefaultSize()",       WRAP_MFN(CSettings, getDefaultSize),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getSizeHalf()",          WRAP_MFN(CSettings, getSizeHalf),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getNativeSize()",        WRAP_MFN(CSettings, getNativeSize),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getDefaultSizeHalf()",   WRAP_MFN(CSettings, getDefaultSizeHalf),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setSize(const CSize & in)",       WRAP_MFN(CSettings, setSize),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getScreenAspectRatio()", WRAP_MFN(CSettings, getScreenAspectRatio), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "const CSize & getOrthoAspectRatio()",  WRAP_MFN(CSettings, getOrthoAspectRatio),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void calcRatio()",                     WRAP_MFN(CSettings, calcRatio),            asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool getVSync()",                      WRAP_MFN(CSettings, getVSync),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setVSync(bool)",                  WRAP_MFN(CSettings, setVSync),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool getFullScreen()",                 WRAP_MFN(CSettings, getFullScreen),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setFullScreen(bool)",             WRAP_MFN(CSettings, setFullScreen),        asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getSectorSize()",                  WRAP_MFN(CSettings, getSectorSize),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getSectorSizeHalf()",              WRAP_MFN(CSettings, getSectorSizeHalf),    asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getAnisotropicLevel()",            WRAP_MFN(CSettings, getAnisotropicLevel),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setAnisotropicLevel(int)",        WRAP_MFN(CSettings, setAnisotropicLevel),  asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isDebugMode()",                   WRAP_MFN(CSettings, isDebugMode),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isDebugAsMobile()",               WRAP_MFN(CSettings, isDebugAsMobile),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isAutoplay()",                    WRAP_MFN(CSettings, isAutoplay),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "bool isMobileDevice()",                WRAP_MFN(CSettings, isMobileDevice),       asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "int getGamePadStickDeadZone()",        WRAP_MFN(CSettings, getGamePadStickDeadZone), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSettings", "void setGamePadStickDeadZone(int)",    WRAP_MFN(CSettings, setGamePadStickDeadZone), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSettings", "void saveSettings()",                  WRAP_MFN(CSettings, saveSettings), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CSettings Settings", &CSettings::Instance()) );
    }
}
