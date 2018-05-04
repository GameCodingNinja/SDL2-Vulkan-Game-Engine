
/************************************************************************
*    FILE NAME:       scriptsoundmanager.cpp
*
*    DESCRIPTION:     CSoundMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsoundmanager.h>

// Game lib dependencies
#include <managers/soundmanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSoundManager
{
    /************************************************************************
    *    DESC:  Load the data list table                                                            
    ************************************************************************/
    void LoadListTable( const std::string & filePath, CSoundMgr & rSoundMgr )
    {
        try
        {
            rSoundMgr.loadListTable( filePath );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    DESC:  Load all of the sounds specific to this group                                                           
    ************************************************************************/
    void LoadGroup( const std::string & group, CSoundMgr & rSoundMgr )
    {
        try
        {
            rSoundMgr.loadGroup( group );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }

    /************************************************************************
    *    DESC:  Free all of the sounds specific to this group                                                           
    ************************************************************************/
    void FreeGroup( const std::string & group, CSoundMgr & rSoundMgr )
    {
        try
        {
            rSoundMgr.freeGroup( group );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CSoundMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void loadListTable(string &in)",                     asFUNCTION(LoadListTable), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void loadGroup(string &in)",                         asFUNCTION(LoadGroup),     asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void freeGroup(string &in)",                         asFUNCTION(FreeGroup),     asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void play(string &in, string &in, int loopCount=0)", asMETHOD(CSoundMgr, play),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void pause(string &in, string &in)",                 asMETHOD(CSoundMgr, pause),       asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void resume(string &in, string &in)",                asMETHOD(CSoundMgr, resume),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void stop(string &in, string &in)",                  asMETHOD(CSoundMgr, stop),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void setVolume(string &in, string &in, int)",        asMETHOD(CSoundMgr, setVolume),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "int getVolume(string &in, string &in)",              asMETHOD(CSoundMgr, getVolume),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "bool isPlaying(string &in, string &in)",             asMETHOD(CSoundMgr, isPlaying),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "bool isPaused(string &in, string &in)",              asMETHOD(CSoundMgr, isPaused),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "CSound & getSound(string &in, string &in)",          asMETHOD(CSoundMgr, getSound),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "CPlayList & getPlayList(string &in, string &in)",    asMETHOD(CSoundMgr, getPlayList), asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CSoundMgr SoundMgr", &CSoundMgr::Instance()) );
    }
}