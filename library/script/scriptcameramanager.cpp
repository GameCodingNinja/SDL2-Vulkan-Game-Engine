
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

        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "CCamera & getDefault()",               asMETHOD(CCameraMgr, getDefault), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "CCamera & get(string &in)",            asMETHOD(CCameraMgr, get), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void addToTransList(string &in)",      asMETHOD(CCameraMgr, addToTransList), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void removeFromTransList(string &in)", asMETHOD(CCameraMgr, removeFromTransList), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void transform()",                     asMETHOD(CCameraMgr, transform), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void clear()",                         asMETHOD(CCameraMgr, clear), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void clearTransList()",                asMETHOD(CCameraMgr, clearTransList), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void rebuildProjectionMatrix()",       asMETHOD(CCameraMgr, rebuildProjectionMatrix), asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CCameraMgr CameraMgr", &CCameraMgr::Instance()) );
    }
}
