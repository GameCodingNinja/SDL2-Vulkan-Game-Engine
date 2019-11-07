
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
#include <autowrapper/aswrappedcall.h>

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

        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "CCamera & getDefault()",               WRAP_MFN(CCameraMgr, getDefault),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "CCamera & get(string &in)",            WRAP_MFN(CCameraMgr, get),                     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void addToTransList(string &in)",      WRAP_MFN(CCameraMgr, addToTransList),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void removeFromTransList(string &in)", WRAP_MFN(CCameraMgr, removeFromTransList),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void transform()",                     WRAP_MFN(CCameraMgr, transform),               asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void clear()",                         WRAP_MFN(CCameraMgr, clear),                   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void clearTransList()",                WRAP_MFN(CCameraMgr, clearTransList),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCameraMgr", "void rebuildProjectionMatrix()",       WRAP_MFN(CCameraMgr, rebuildProjectionMatrix), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CCameraMgr CameraMgr", &CCameraMgr::Instance()) );
    }
}
