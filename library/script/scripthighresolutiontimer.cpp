
/************************************************************************
*    FILE NAME:       scripthighresolutiontimer.cpp
*
*    DESCRIPTION:     CHighResTimer script object registration
************************************************************************/

// Physical component dependency
#include <script/scripthighresolutiontimer.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

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
        
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "void calcElapsedTime()",  WRAP_MFN(CHighResTimer, calcElapsedTime), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "double getElapsedTime()", WRAP_MFN(CHighResTimer, getElapsedTime),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "void timerStart()",       WRAP_MFN(CHighResTimer, timerStart),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "float timerStop()",       WRAP_MFN(CHighResTimer, timerStop),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CHighResTimer", "double getTime()",        WRAP_MFN(CHighResTimer, getTime),         asCALL_GENERIC) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CHighResTimer HighResTimer", &CHighResTimer::Instance()) );
    }
}