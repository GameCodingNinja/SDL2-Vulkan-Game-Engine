
/************************************************************************
*    FILE NAME:       scriptglobals.cpp
*
*    DESCRIPTION:     Global function registration
************************************************************************/

// Physical component dependency
#include <script/scriptglobals.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <managers/signalmanager.h>
#include <script/scriptmanager.h>
#include <scriptarray/scriptarray.h>
#include <common/size.h>

// SDL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptGlobals
{
    /************************************************************************
    *    DESC:  Throw an exception for values less then 0
    ************************************************************************/
    void Throw( int value )
    {
        if( value < 0 )
            throw NExcept::CCriticalException("Error Registering AngelScript Function!",
                boost::str( boost::format("Function could not be created.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
    }

    /************************************************************************
    *    DESC:  Suspend the script to the game loop
    ************************************************************************/
    void Suspend()
    {
        asIScriptContext *ctx = asGetActiveContext();

        // Suspend the context so the game loop can resumed
        if( ctx )
            ctx->Suspend();
    }
    
    
    /************************************************************************
    *    DESC:  Get Resolutions
    ************************************************************************/
    CScriptArray * GetScreenResolutions()
    {
        // The script array needs to know its type to properly handle the elements.
        // Note that the object type should be cached to avoid performance issues
        // if the function is called frequently.
        asITypeInfo * arrayType = CScriptMgr::Instance().getEnginePtr()->GetTypeInfoByDecl("array<CSize>");
        
        std::vector< CSize<int> > resVec;
        int displayCount = SDL_GetNumDisplayModes(0);
        for( int i = 0; i < displayCount; i++ )
        {
            SDL_DisplayMode mode;

            if( SDL_GetDisplayMode(0, i, &mode) == 0 )
            {
                CSize<int> size(mode.w, mode.h);

                // Keep out any duplicates
                if( std::find(resVec.begin(), resVec.end(), size) == resVec.end() )
                    resVec.push_back( size );
            }
        }

        // The resolutions are in greatest to smallest. We need the order reversed
        std::reverse(resVec.begin(), resVec.end());
        
        CScriptArray* ary = CScriptArray::Create(arrayType, resVec.size());
        for( size_t i = 0; i < resVec.size(); ++i )
        {
            CSize<float> mode( resVec[i] );
            ary->SetValue(i, &mode);
        }
        
        return ary;
    }
 

    /************************************************************************
    *    DESC:  Register the global functions
    ************************************************************************/
    void Register()
    {
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        Throw( pEngine->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(NGenFunc::PostDebugMsg), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Suspend()", asFUNCTION(Suspend), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("int UniformRandomInt(int startRange, int endRange, int seed = 0)", asFUNCTION( NGenFunc::UniformRandomInt), asCALL_CDECL ) );
        Throw( pEngine->RegisterGlobalFunction("float UniformRandomFloat(float startRange, float endRange, int seed = 0)", asFUNCTION( NGenFunc::UniformRandomFloat), asCALL_CDECL ) );
        // The DispatchEvent function has 4 parameters and because they are not defined here, they only return garbage
        // AngelScript is not allowing the other two voided pointers
        Throw( pEngine->RegisterGlobalFunction("void DispatchEvent(int type, int code = 0)", asFUNCTION(NGenFunc::DispatchEvent), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, string &in = '')", asMETHOD(CScriptMgr, prepareSpawn), asCALL_THISCALL_ASGLOBAL, &CScriptMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void SpawnByThread(string &in, string &in = '')", asMETHOD(CScriptMgr, spawnByThread), asCALL_THISCALL_ASGLOBAL, &CScriptMgr::Instance()) );
        
        Throw( pEngine->RegisterGlobalFunction("array<CSize> @ GetScreenResolutions()", asFUNCTION(GetScreenResolutions), asCALL_CDECL) );
    }
}
