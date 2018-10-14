
/************************************************************************
*    FILE NAME:       strategymanager.cpp
*
*    DESCRIPTION:     CStrategyMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptstrategymanager.h>

// Game lib dependencies
#include <strategy/strategymanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <strategy/basicstagestrategy.h>
#include <strategy/basicspritestrategy.h>
#include <strategy/menustrategy.h>
#include <common/isprite.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptStrategyManager
{
    /************************************************************************
    *    DESC:  Create a basic sprite strategy                                                            
    ************************************************************************/
    iStrategy * CreateBasicSpriteStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.addStrategy( strategyId, new CBasicSpriteStrategy );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pStrategy;
    }
    
    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    iStrategy * CreateBasicStageStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.addStrategy( strategyId, new CBasicStageStrategy );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pStrategy;
    }
    
    /************************************************************************
    *    DESC:  Create a menu strategy                                                            
    ************************************************************************/
    iStrategy * CreateMenuStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.addStrategy( strategyId, new CMenuStrategy );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pStrategy;
    }

    /************************************************************************
    *    DESC:  Get the sprite strategy via string search
    ************************************************************************/
    iStrategy * GetStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.getStrategy( strategyId );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pStrategy;
    }

    /************************************************************************
    *    DESC:  Get the sprite strategy via string find
    ************************************************************************/
    iStrategy * FindStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.findStrategy( strategyId );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pStrategy;
    }

    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    void DeleteStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        try
        {
            rStrategyMgr.deleteStrategy( strategyId );
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
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    CSprite * CreateSprite1( const std::string & strategyId, const std::string & group, const std::string & name, CStrategyMgr & rStrategyMgr )
    {
        try
        {
            return rStrategyMgr.create( strategyId, group, name );
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
    
    CSprite * CreateSprite2( const std::string & strategyId, const std::string & name, CStrategyMgr & rStrategyMgr )
    {
        try
        {
            return rStrategyMgr.create( strategyId, name );
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
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createBasicSpriteStrategy(string &in)",    asFUNCTION(CreateBasicSpriteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createBasicStageStrategy(string &in)",     asFUNCTION(CreateBasicStageStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createMenuStrategy(string &in)",           asFUNCTION(CreateMenuStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & getStrategy(string &in)",                  asFUNCTION(GetStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & findStrategy(string &in)",                 asFUNCTION(FindStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",                            asFUNCTION(DeleteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteSprite(string &in, int)",                         asMETHOD(CStrategyMgr, deleteSprite),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSprite & createSprite(string &in, string &in, string &in)", asFUNCTION(CreateSprite1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSprite & createSprite(string &in, string &in)",             asFUNCTION(CreateSprite2), asCALL_CDECL_OBJLAST) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CStrategyMgr::Instance()) );
    }
}
