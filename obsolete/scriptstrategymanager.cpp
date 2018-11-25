
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
#include <strategy/actorstrategy.h>
#include <utilities/exceptionhandling.h>
#include <node/inode.h>
#include <sprite/sprite.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptStrategyManager
{
    /************************************************************************
    *    DESC:  Create a basic sprite strategy                                                            
    ************************************************************************/
    iStrategy * CreateActorStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.addStrategy( strategyId, new CActorStrategy );
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
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void loadListTable(string &in)",                   asMETHOD(CStrategyMgr, loadListTable), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createActorStrategy(string &in)",      asFUNCTION(CreateActorStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createBasicStageStrategy(string &in)", asFUNCTION(CreateBasicStageStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & getStrategy(string &in)",              asFUNCTION(GetStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",                  asFUNCTION(DeleteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void update()",                                    asMETHOD(CStrategyMgr, update), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void transform()",                                 asMETHOD(CStrategyMgr, transform), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void setCamera(string &in, string &in)",           asMETHOD(CStrategyMgr, setCamera), asCALL_THISCALL) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CStrategyMgr::Instance()) );
    }
}
