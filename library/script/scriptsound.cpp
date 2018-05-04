
/************************************************************************
*    FILE NAME:       scriptsound.cpp
*
*    DESCRIPTION:     CSound script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsound.h>

// Game lib dependencies
#include <common/sound.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSound
{
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("CSound", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CSound", "void play( int channel = -1, int loopCount = 0 )", asMETHOD(CSound, play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void stop()",                                      asMETHOD(CSound, stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void pause()",                                     asMETHOD(CSound, pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void resume()",                                    asMETHOD(CSound, resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void setVolume(int)",                              asMETHOD(CSound, setVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "int getVolume()",                                  asMETHOD(CSound, getVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool isPlaying()",                                 asMETHOD(CSound, isPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool isPaused()",                                  asMETHOD(CSound, isPaused),  asCALL_THISCALL) );
    }
}
