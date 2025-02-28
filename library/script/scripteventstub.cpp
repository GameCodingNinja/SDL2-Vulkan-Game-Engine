
/************************************************************************
*    FILE NAME:       scripteventstub.h
*
*    DESCRIPTION:     Event script Stub object registration
*                     NOTE: Stub to provide the dependancy when not used
************************************************************************/

// Physical component dependency
#include <script/scripteventstub.h>

// Game lib dependencies
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

// SDL lib dependencies
#include <SDL3/SDL_events.h>

namespace NScriptEventStub
{
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType( "CEvent", 0, asOBJ_REF|asOBJ_NOCOUNT) );
     }
}