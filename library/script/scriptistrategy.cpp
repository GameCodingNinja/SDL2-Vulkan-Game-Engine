
/************************************************************************
*    FILE NAME:       scriptistrategy.cpp
*
*    DESCRIPTION:     iStrategy script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptistrategy.h>

// Game lib dependencies
#include <strategy/istrategy.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptiStrategy
{
    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    /*CSprite * CreateSprite( const std::string & strategyId, const std::string & dataName, iStrategy & rStrategy )
    {
        try
        {
            return rStrategy.create( dataName );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
    }*/
    
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("iStrategy", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setCameraId(string &in)",        asMETHOD(iStrategy, setCameraId),         asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setIdOffset(int)",               asMETHOD(iStrategy, setIdOffset),         asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setIdDir(int)",                  asMETHOD(iStrategy, setIdDir),            asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setToDestroy(int)",              asMETHOD(iStrategy, setToDestroy),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setToCreate(string &in)",        asMETHOD(iStrategy, setToCreate),         asCALL_THISCALL) );
        
        //Throw( pEngine->RegisterObjectMethod("iStrategy", "iSprite & createSprite(string &in)",  asFUNCTION(CreateSprite), asCALL_CDECL_OBJLAST) );
    }
}
