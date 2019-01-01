
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
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>
#include <system/device.h>
#include <node/inode.h>
#include <sprite/sprite.h>
#include <common/point.h>

// AngelScript lib dependencies
#include <angelscript.h>

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
    iNode * Create( const std::string & id, const std::string & instance, iStrategy & rStrategy )
    {
        try
        {
            return rStrategy.create( id, instance );
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
    *    DESC:  Clear all the strategies
    ************************************************************************/
    void Clear( CStrategyMgr & rStrategyMgr )
    {
        try
        {
            rStrategyMgr.clear();
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
        Throw( pEngine->RegisterObjectType(  "iNode", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // iNode specific functions
        Throw( pEngine->RegisterObjectMethod("iNode", "CSprite & getSprite()",            asMETHOD(iNode, getSprite), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & next()",                   asMETHOD(iNode, next), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getType()",                    asMETHOD(iNode, getType), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getId()",                      asMETHOD(iNode, getId), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "void resetIterators()",            asMETHOD(iNode, resetIterators), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & getChildNode(string &in)", asMETHOD(iNode, getChildNode), asCALL_THISCALL) );

        // Register type
        Throw( pEngine->RegisterObjectType("iStrategy", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("iStrategy", "void setCommandBuffer(string &in)",           asFUNCTION(SetCommandBuffer), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "iNode & create(string &in, string &in = '')", asFUNCTION(Create), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iStrategy", "void destroy(int)",                           asMETHOD(iStrategy, destroy),   asCALL_THISCALL) );
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void loadListTable(string &in)",               asMETHOD(CStrategyMgr, loadListTable), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createActorStrategy(string &in)",  asFUNCTION(CreateActorStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & createStageStrategy(string &in)",  asFUNCTION(CreateStageStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & activateStrategy(string &in)",     asMETHOD(CStrategyMgr, activateStrategy), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & deactivateStrategy(string &in)",   asMETHOD(CStrategyMgr, deactivateStrategy), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iStrategy & getStrategy(string &in)",          asFUNCTION(GetStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",              asFUNCTION(DeleteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void update()",                                asMETHOD(CStrategyMgr, update), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void transform()",                             asMETHOD(CStrategyMgr, transform), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void setCamera(string &in, string &in)",       asMETHOD(CStrategyMgr, setCamera), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void clear()",                                 asFUNCTION(Clear), asCALL_CDECL_OBJLAST) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CStrategyMgr::Instance()) );
    }
}
