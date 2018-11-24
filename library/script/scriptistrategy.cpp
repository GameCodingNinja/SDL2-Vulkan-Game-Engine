
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
#include <system/device.h>
#include <node/inode.h>
#include <sprite/sprite.h>

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
    *    DESC:  Set the command buffers
    ************************************************************************/
    void SetCommandBuffer( const std::string & cmdBufPoolId, iStrategy & rStrategy )
    {
        try
        {
            auto cmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( cmdBufPoolId );
            rStrategy.setCommandBuffers( cmdBufVec );
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
    *    DESC:  Create an actor sprite                                                            
    ************************************************************************/
    CSprite * Create( const std::string & id, iStrategy & rStrategy )
    {
        try
        {
            return rStrategy.create( id )->getSprite();
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
    }
    
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("iStrategy", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setIdOffset(int)",               asMETHOD(iStrategy, setIdOffset),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setIdDir(int)",                  asMETHOD(iStrategy, setIdDir),      asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setToDestroy(int)",              asMETHOD(iStrategy, setToDestroy),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setToCreate(string &in)",        asMETHOD(iStrategy, setToCreate),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void enable(bool enable = true)",     asMETHOD(iStrategy, enable),        asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setCommandBuffer(string &in)",   asFUNCTION(SetCommandBuffer), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iStrategy", "CSprite & create(string &in)",         asFUNCTION(Create), asCALL_CDECL_OBJLAST) );
    }
}
