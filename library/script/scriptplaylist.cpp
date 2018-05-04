
/************************************************************************
*    FILE NAME:       scriptplaylist.cpp
*
*    DESCRIPTION:     CPlayList script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptplaylist.h>

// Game lib dependencies
#include <common/playlist.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptPlayLst
{
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("CPlayList", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CPlayList", "void play( int channel = -1, int loopCount = 0 )", asMETHOD(CPlayList, play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void stop()",                                      asMETHOD(CPlayList, stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void pause()",                                     asMETHOD(CPlayList, pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void resume()",                                    asMETHOD(CPlayList, resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void setVolume(int)",                              asMETHOD(CPlayList, setVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "int getVolume()",                                  asMETHOD(CPlayList, getVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool isPlaying()",                                 asMETHOD(CPlayList, isPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool isPaused()",                                  asMETHOD(CPlayList, isPaused),  asCALL_THISCALL) );
    }
}
