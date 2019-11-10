
/************************************************************************
*    FILE NAME:       scriptistrategy.cpp
*
*    DESCRIPTION:     iStrategy script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptstrategy.h>

// Game lib dependencies
#include <strategy/strategymanager.h>
#include <strategy/istrategy.h>
#include <strategy/stagestrategy.h>
#include <strategy/actorstrategy.h>
#include <strategy/strategyloader.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>
#include <system/device.h>
#include <node/inode.h>
#include <sprite/sprite.h>
#include <common/point.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptStrategy
{
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
    iNode * Create( const std::string & id, const std::string & instance, bool active, const std::string & group, iStrategy & rStrategy )
    {
        try
        {
            return rStrategy.create( id, instance, active, group );
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
    iStrategy * CreateStageStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        iStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.addStrategy( strategyId, new CStageStrategy );
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
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType(  "iNode", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // iNode specific functions
        Throw( pEngine->RegisterObjectMethod("iNode", "CSprite & getSprite()",            WRAP_MFN(iNode, getSprite),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & next()",                   WRAP_MFN(iNode, next),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getType()",                    WRAP_MFN(iNode, getType),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getId()",                      WRAP_MFN(iNode, getId),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "void resetIterators()",            WRAP_MFN(iNode, resetIterators), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & getChildNode(string &in)", WRAP_MFN(iNode, getChildNode),   asCALL_GENERIC) );

        // Register type
        Throw( pEngine->RegisterObjectType("iStrategy", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setCommandBuffer(string &in)",           WRAP_OBJ_LAST(SetCommandBuffer), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "iNode & create(string &in, string &in = '', bool active = true, string &in = '')", WRAP_OBJ_LAST(Create), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void destroy(int)",                           WRAP_MFN(iStrategy, destroy),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setCamera(string &in)",                  WRAP_MFN(iStrategy, setCamera),  asCALL_GENERIC) );
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void loadListTable(string &in)",                WRAP_MFN(CStrategyMgr, loadListTable),         asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createActorStrategy(string &in)",   WRAP_OBJ_LAST(CreateActorStrategy),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createStageStrategy(string &in)",   WRAP_OBJ_LAST(CreateStageStrategy),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & activateStrategy(string &in)",      WRAP_MFN(CStrategyMgr, activateStrategy),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void activateStrategyAry(array<string> &in)",   WRAP_MFN(CStrategyMgr, activateStrategyAry),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deactivateStrategy(string &in)",           WRAP_MFN(CStrategyMgr, deactivateStrategy),    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deactivateStrategyAry(array<string> &in)", WRAP_MFN(CStrategyMgr, deactivateStrategyAry), asCALL_GENERIC) );


        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & getStrategy(string &in)",           WRAP_OBJ_LAST(GetStrategy),                    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",               WRAP_MFN(CStrategyMgr, deleteStrategy),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategyAry(array<string> &in)",     WRAP_MFN(CStrategyMgr, deleteStrategyAry),     asCALL_GENERIC) );


        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void update()",                                 WRAP_MFN(CStrategyMgr, update),                asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void transform()",                              WRAP_MFN(CStrategyMgr, transform),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void clear()",                                  WRAP_MFN(CStrategyMgr, clear),                 asCALL_GENERIC) );

        // Load the strategy
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void loadStrategy(string &in)",                 WRAP_FN(NStrategyloader::load),                 asCALL_GENERIC) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CStrategyMgr::Instance()) );
    }
}
