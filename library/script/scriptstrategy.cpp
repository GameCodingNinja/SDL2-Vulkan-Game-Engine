
/************************************************************************
*    FILE NAME:       scriptistrategy.cpp
*
*    DESCRIPTION:     CStrategy script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptstrategy.h>

// Game lib dependencies
#include <strategy/strategymanager.h>
#include <strategy/strategy.h>
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
    void SetCommandBuffer( const std::string & cmdBufPoolId, CStrategy & rStrategy )
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
    *    DESC:  Get the pointer to the node
    ************************************************************************/
    iNode * GetNode( const std::string & instanceName, CStrategy & rStrategy )
    {
        try
        {
            return rStrategy.getNode(instanceName);
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
    *    DESC:  Create an sprite
    ************************************************************************/
    iNode * Create( const std::string & id, const std::string & instance, bool active, const std::string & group, CStrategy & rStrategy )
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
    CStrategy * CreateStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        CStrategy * pStrategy = nullptr;
        
        try
        {
            pStrategy = rStrategyMgr.addStrategy( strategyId, new CStrategy );
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
    CStrategy * GetStrategy( const std::string & strategyId, CStrategyMgr & rStrategyMgr )
    {
        CStrategy * pStrategy = nullptr;
        
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

    iNode * GetiNodeFromSprite(CSprite & sprite)
    {
        return dynamic_cast<iNode *>(&sprite);
    }

    iNode * GetiNodeFromObject(CObject & object)
    {
        return dynamic_cast<iNode *>(&object);
    }
    
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "iNode", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // iNode specific functions
        Throw( pEngine->RegisterObjectMethod("iNode", "CSprite & getSprite()",            WRAP_MFN(iNode, getSprite),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "CObject & getObject()",            WRAP_MFN(iNode, getObject),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "uiControl & getControl()",         WRAP_MFN(iNode, getControl),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & next()",                   WRAP_MFN(iNode, next),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getType()",                    WRAP_MFN(iNode, getType),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getId()",                      WRAP_MFN(iNode, getId),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "handle getHandle()",               WRAP_MFN(iNode, getHandle),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & getChildNode(string &in)", WRAP_MFN(iNode, getChildNode),   asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "iNode & getNode()",                                     WRAP_OBJ_LAST(GetiNodeFromSprite), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CObject", "iNode & getNode()",                                   WRAP_OBJ_LAST(GetiNodeFromObject), asCALL_GENERIC) );

        // Register type
        Throw( pEngine->RegisterObjectType("Strategy", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("Strategy", "void setCommandBuffer(string &in)",           WRAP_OBJ_LAST(SetCommandBuffer), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "iNode & create(string &in, string &in = '', bool active = true, string &in = '')", WRAP_OBJ_LAST(Create), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "void destroy(handle)",                        WRAP_MFN(CStrategy, destroy),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "void setCamera(string &in)",                  WRAP_MFN(CStrategy, setCamera),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "iNode & getNode(string &in)",                 WRAP_OBJ_LAST(GetNode),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "iNode & activateNode(string &in)",            WRAP_MFN(CStrategy, activateNode),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "void deactivateNode(string &in)",             WRAP_MFN(CStrategy, deactivateNode),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("Strategy", "void clear()",                                WRAP_MFN(CStrategy, clear),  asCALL_GENERIC) );
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void loadListTable(string &in)",                WRAP_MFN(CStrategyMgr, loadListTable),         asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "Strategy & createActorStrategy(string &in)",    WRAP_OBJ_LAST(CreateStrategy),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "Strategy & activateStrategy(string &in)",       WRAP_MFN(CStrategyMgr, activateStrategy),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void activateStrategyAry(array<string> &in)",   WRAP_MFN(CStrategyMgr, activateStrategyAry),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deactivateStrategy(string &in)",           WRAP_MFN(CStrategyMgr, deactivateStrategy),    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deactivateStrategyAry(array<string> &in)", WRAP_MFN(CStrategyMgr, deactivateStrategyAry), asCALL_GENERIC) );


        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "Strategy & getStrategy(string &in)",            WRAP_OBJ_LAST(GetStrategy),                    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",               WRAP_MFN(CStrategyMgr, deleteStrategy),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategyAry(array<string> &in)",     WRAP_MFN(CStrategyMgr, deleteStrategyAry),     asCALL_GENERIC) );


        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void update()",                                 WRAP_MFN(CStrategyMgr, update),                asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void transform()",                              WRAP_MFN(CStrategyMgr, transform),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void clear()",                                  WRAP_MFN(CStrategyMgr, clear),                 asCALL_GENERIC) );

        // Load the strategy
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void loadStrategy(string &in)",                 WRAP_MFN(CStrategyloader, load),               asCALL_GENERIC) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CStrategyMgr::Instance()) );
    }
}
