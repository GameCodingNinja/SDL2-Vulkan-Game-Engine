
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
#include <script/scriptmanager.h>
#include <common/size.h>

// Standard lib dependencies
#include <random>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL3/SDL.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <scriptarray/scriptarray.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptGlobals
{
    std::default_random_engine defaultGenerator(std::random_device{}());
    
    /************************************************************************
    *    DESC:  Generate a random number
    *    param: int min, int max
    ************************************************************************/
    void RandInt( asIScriptGeneric * pScriptGen )
    {
        const int min = pScriptGen->GetArgDWord(0);
        const int max = pScriptGen->GetArgDWord(1);

        std::uniform_int_distribution<int> randInt(min, max);

        pScriptGen->SetReturnDWord( randInt(defaultGenerator) );
    }

    void RandFloat( asIScriptGeneric * pScriptGen )
    {
        const float min = pScriptGen->GetArgFloat(0);
        const float max = pScriptGen->GetArgFloat(1);

        std::uniform_real_distribution<float> randFloat(min, max);

        pScriptGen->SetReturnFloat( randFloat(defaultGenerator) );
    }
    
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
    void Suspend( asIScriptGeneric * pScriptGen )
    {
        asIScriptContext *ctx = asGetActiveContext();

        // Suspend the context so the game loop can resumed
        if( ctx )
            ctx->Suspend();
    }
    
    
    /************************************************************************
    *    DESC:  Get Resolutions
    ************************************************************************/
    void GetScreenResolutions( asIScriptGeneric * pScriptGen )
    {
        std::vector< CSize<int> > resVec;

        int displayCount = 0;
        SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
        SDL_DisplayMode ** mode = SDL_GetFullscreenDisplayModes(displayID, &displayCount);

        for( int i = 0; i < displayCount; i++ )
        {
            CSize<int> size(mode[i]->w, mode[i]->h);

            // Keep out any duplicates
            if( std::find(resVec.begin(), resVec.end(), size) == resVec.end() )
                resVec.push_back( size );
        }

        // The resolutions are in greatest to smallest. We need the order reversed
        std::reverse(resVec.begin(), resVec.end());

        // Get a pointer to type declaration
        asITypeInfo * arrayType = CScriptMgr::Instance().getPtrToTypeInfo( "array<CSize>" );
        
        CScriptArray* ary = CScriptArray::Create(arrayType, resVec.size());
        for( std::size_t i = 0; i < resVec.size(); ++i )
        {
            CSize<float> mode( resVec[i] );
            ary->SetValue(i, &mode);
        }

        // Set the return value
        pScriptGen->SetReturnObject( ary );
    }

    /************************************************************************
    *    DESC:  Dispatch Even Wrapper
    *    PARAM: int return; int type, int code
    ************************************************************************/
    void DispatchEvent( asIScriptGeneric * pScriptGen )
    {
        const int type = pScriptGen->GetArgDWord(0);
        const int code = pScriptGen->GetArgDWord(1);

        pScriptGen->SetReturnDWord( NGenFunc::DispatchEvent( type, code ) );
    }

    /************************************************************************
    *    DESC:  Spawn Wrapper
    *    PARAM: const std::string & funcName, const std::string & group
    ************************************************************************/
    void Spawn( asIScriptGeneric * pScriptGen )
    {
        const std::string *funcName = reinterpret_cast<std::string*>(pScriptGen->GetArgAddress(0));
        const std::string *group = reinterpret_cast<std::string*>(pScriptGen->GetArgAddress(1));

        CScriptMgr::Instance().prepareSpawn( *funcName, *group );
    }

    /************************************************************************
    *    DESC:  Spawn by thread Wrapper
    *    PARAM: const std::string & funcName, const std::string & group
    ************************************************************************/
    void SpawnByThread( asIScriptGeneric * pScriptGen )
    {
        const std::string *funcName = reinterpret_cast<std::string*>(pScriptGen->GetArgAddress(0));
        const std::string *group = reinterpret_cast<std::string*>(pScriptGen->GetArgAddress(1));

        CScriptMgr::Instance().spawnByThread( *funcName, *group );
    }

    /************************************************************************
    *    DESC:  Register the global functions
    ************************************************************************/
    void Register()
    {
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        Throw( pEngine->RegisterGlobalFunction("int RandInt(int, int)", asFUNCTION(RandInt), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("float RandFloat(float, float)", asFUNCTION(RandFloat), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("void Print(string &in)", WRAP_FN(NGenFunc::PostDebugMsg), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("void Suspend()", asFUNCTION(Suspend), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("int UniformRandomInt(int startRange, int endRange, int seed = 0)", WRAP_FN(NGenFunc::UniformRandomInt), asCALL_GENERIC ) );
        Throw( pEngine->RegisterGlobalFunction("float UniformRandomFloat(float startRange, float endRange, int seed = 0)", WRAP_FN(NGenFunc::UniformRandomFloat), asCALL_GENERIC ) );
        // The DispatchEvent function has 4 parameters and because they are not defined here, they only return garbage
        // AngelScript is not allowing the other two voided pointers
        Throw( pEngine->RegisterGlobalFunction("int DispatchEvent(int type, int code = 0)", asFUNCTION(DispatchEvent), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, string &in = '')", asFUNCTION(Spawn), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("void SpawnByThread(string &in, string &in = '')", asFUNCTION(SpawnByThread), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterGlobalFunction("array<CSize> @ GetScreenResolutions()", asFUNCTION(GetScreenResolutions), asCALL_GENERIC) );
    }
}
