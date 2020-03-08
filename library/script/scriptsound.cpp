
/************************************************************************
*    FILE NAME:       scriptsound.cpp
*
*    DESCRIPTION:     CSound script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsound.h>

// Game lib dependencies
#include <sound/playlist.h>
#include <sound/soundmanager.h>
#include <sound/sound.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptSound
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
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CPlayList", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CPlayList", "void play( int channel = -1, int loopCount = 0 )", WRAP_MFN(CPlayList, play),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void stop()",                                      WRAP_MFN(CPlayList, stop),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void pause()",                                     WRAP_MFN(CPlayList, pause),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void resume()",                                    WRAP_MFN(CPlayList, resume),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void setVolume(int)",                              WRAP_MFN(CPlayList, setVolume), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "int getVolume() const",                            WRAP_MFN(CPlayList, getVolume), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool isPlaying() const",                           WRAP_MFN(CPlayList, isPlaying), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool isPaused() const",                            WRAP_MFN(CPlayList, isPaused),  asCALL_GENERIC) );

        
        // Register type
        Throw( pEngine->RegisterObjectType("CSound", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CSound", "void play( int channel = -1, int loopCount = 0 )", WRAP_MFN(CSound, play),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void stop()",                                      WRAP_MFN(CSound, stop),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void pause()",                                     WRAP_MFN(CSound, pause),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void resume()",                                    WRAP_MFN(CSound, resume),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void setVolume(int)",                              WRAP_MFN(CSound, setVolume), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "int getVolume() const",                            WRAP_MFN(CSound, getVolume), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool isPlaying() const",                           WRAP_MFN(CSound, isPlaying), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool isPaused() const",                            WRAP_MFN(CSound, isPaused),  asCALL_GENERIC) );
        
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CSoundMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void loadListTable(string &in)",                     WRAP_OBJ_LAST(LoadListTable), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void loadGroup(string &in)",                         WRAP_OBJ_LAST(LoadGroup),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void freeGroup(string &in)",                         WRAP_OBJ_LAST(FreeGroup),     asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void play(string &in, string &in, int loopCount=0)", WRAP_MFN(CSoundMgr, play),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void pause(string &in, string &in)",                 WRAP_MFN(CSoundMgr, pause),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void resume(string &in, string &in)",                WRAP_MFN(CSoundMgr, resume),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void stop(string &in, string &in)",                  WRAP_MFN(CSoundMgr, stop),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void setVolume(string &in, string &in, int)",        WRAP_MFN(CSoundMgr, setVolume),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "int getVolume(string &in, string &in) const",        WRAP_MFN(CSoundMgr, getVolume),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "bool isPlaying(string &in, string &in) const",       WRAP_MFN(CSoundMgr, isPlaying),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "bool isPaused(string &in, string &in) const",        WRAP_MFN(CSoundMgr, isPaused),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "CSound & getSound(string &in, string &in)",          WRAP_MFN(CSoundMgr, getSound),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "CPlayList & getPlayList(string &in, string &in)",    WRAP_MFN(CSoundMgr, getPlayList), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CSoundMgr SoundMgr", &CSoundMgr::Instance()) );
    }
}
