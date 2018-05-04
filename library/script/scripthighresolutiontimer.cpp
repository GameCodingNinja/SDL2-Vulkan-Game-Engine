
/************************************************************************
*    FILE NAME:       scripthighresolutiontimer.cpp
*
*    DESCRIPTION:     CHighResTimer script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsoundmanager.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptHighResolutionTimer
{
    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CHighResTimer", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "double getElapsedTime()", asMETHOD(CHighResTimer, getElapsedTime), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "void timerStart()",       asMETHOD(CHighResTimer, timerStart),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "float timerStop()",       asMETHOD(CHighResTimer, timerStop),      asCALL_THISCALL) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CHighResTimer HighResTimer", &CHighResTimer::Instance()) );
    }
}