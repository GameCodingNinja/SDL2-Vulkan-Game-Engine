
/************************************************************************
*    FILE NAME:       scriptcameramanager.cpp
*
*    DESCRIPTION:     CCameraMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptcameramanager.h>

// Game lib dependencies
#include <managers/cameramanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptCameraManager
{
    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CCameraMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod(
            "CCameraMgr",
            "CCamera & createOrthographic(string &in, float minZDist = 5, float maxZDist = 1000, float scale = 1.f)",
            asMETHOD(CCameraMgr, createOrthographic), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CCameraMgr",
            "CCamera & createPerspective(string &in, float angle = 45, float minZDist = 5, float maxZDist = 1000, float scale = 1.f)",
            asMETHOD(CCameraMgr, createPerspective), asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CCameraMgr CameraMgr", &CCameraMgr::Instance()) );
    }
}
