
/************************************************************************
*    FILE NAME:       scriptmenumanager.cpp
*
*    DESCRIPTION:     CMenuManager script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptmenumanager.h>

// Game lib dependencies
#include <gui/menumanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>
#include <system/device.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptMenuManager
{
    /************************************************************************
    *    DESC:  Load the data list table
    ************************************************************************/
    void LoadListTable( const std::string & filePath, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.loadListTable( filePath );
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
    *    DESC:  Load the menu group
    ************************************************************************/
    void LoadGroup( const std::string & group, bool doInit, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.loadGroup( group, doInit );
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
    *    DESC:  Load the menu
    ************************************************************************/
    void InitGroup( const std::string & group, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.initGroup( group );
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
    *    DESC:  Free the menu
    ************************************************************************/
    void FreeGroup( const std::string & group, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.freeGroup( group );
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
    *    DESC:  Set the command buffers                                                          
    ************************************************************************/
    void SetCommandBuffer( const std::string & cmdBufPoolId, CMenuMgr & rMenuMgr )
    {
        try
        {
            auto cmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( cmdBufPoolId );
            rMenuMgr.setCommandBuffers( cmdBufVec );
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
    *    DESC:  Active the menu
    ************************************************************************/
    void ActivateMenu1( const std::string & group, const std::string & treeStr, const std::string & menuName, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.activateMenu( group, treeStr, menuName );
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
    *    DESC:  Active the menu
    ************************************************************************/
    void ActivateMenu2( const std::string & treeStr, const std::string & menuName, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.activateMenu( treeStr, menuName );
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
    *    DESC:  Active the tree
    ************************************************************************/
    void ActivateTree1( const std::string & group, const std::string & treeStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.activateTree( group, treeStr );
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
    *    DESC:  Active the tree
    ************************************************************************/
    void ActivateTree2( const std::string & treeStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.activateTree( treeStr );
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
    *    DESC:  Transition the tree
    ************************************************************************/
    void TransitionMenu1( const std::string & group, const std::string & treeStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.transitionMenu( group, treeStr );
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
    *    DESC:  Transition the tree
    ************************************************************************/
    void TransitionMenu2( const std::string & treeStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.transitionMenu( treeStr );
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
    *    DESC:  Deactivate the tree
    ************************************************************************/
    void DeactivateTree1( const std::string & group, const std::string & treeStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.deactivateTree( group, treeStr );
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
    *    DESC:  Deactivate the tree
    ************************************************************************/
    void DeactivateTree2( const std::string & treeStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            rMenuMgr.deactivateTree( treeStr );
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
    *    DESC:  Get the menu
    ************************************************************************/
    CMenu * GetMenu( const std::string & nameStr, CMenuMgr & rMenuMgr )
    {
        try
        {
            return &rMenuMgr.getMenu( nameStr );
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
    *    DESC:  Get the active menu
    ************************************************************************/
    CMenu * GetActiveMenu( CMenuMgr & rMenuMgr )
    {
        try
        {
            return &rMenuMgr.getActiveMenu();
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
    *    DESC:  Update the menu
    *    NOTE:  Can't use macro due to function overloading so handling manually
    ************************************************************************/
    void Update( asIScriptGeneric * pScriptGen )
    {
        reinterpret_cast<CMenuMgr *>(pScriptGen->GetObject())->update();
    }

    /************************************************************************
    *    DESC:  Transform the menu
    *    NOTE:  Can't use macro due to function overloading so handling manually
    ************************************************************************/
    void Transform( asIScriptGeneric * pScriptGen )
    {
        reinterpret_cast<CMenuMgr *>(pScriptGen->GetObject())->transform();
    }

    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType( "CMenuMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void loadListTable(string &in)",                 WRAP_OBJ_LAST(LoadListTable),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void loadMenuAction(string &in)",                WRAP_MFN(CMenuMgr, loadMenuAction),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void loadGroup(string &in, bool doInit = true)", WRAP_OBJ_LAST(LoadGroup),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void initGroup(string &in)",                     WRAP_OBJ_LAST(InitGroup),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void freeGroup(string &in)",                     WRAP_OBJ_LAST(FreeGroup),              asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateMenu(string &in, string &in, string &in)", WRAP_OBJ_LAST(ActivateMenu1),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateMenu(string &in, string &in)",             WRAP_OBJ_LAST(ActivateMenu2),   asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateTree(string &in, string &in)",             WRAP_OBJ_LAST(ActivateTree1),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateTree(string &in)",                         WRAP_OBJ_LAST(ActivateTree2),   asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void deactivateTree(string &in, string &in)",           WRAP_OBJ_LAST(DeactivateTree1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void deactivateTree(string &in)",                       WRAP_OBJ_LAST(DeactivateTree2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void transitionMenu(string &in, string &in)",             WRAP_OBJ_LAST(TransitionMenu1),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void transitionMenu(string &in)",                         WRAP_OBJ_LAST(TransitionMenu2),   asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void clearActiveTrees()",                               WRAP_MFN(CMenuMgr, clearActiveTrees), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "CMenu & getMenu(string &in)",                           WRAP_OBJ_LAST(GetMenu),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "CMenu & getActiveMenu()",                               WRAP_OBJ_LAST(GetActiveMenu),   asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isActive()",                   WRAP_MFN(CMenuMgr, isActive),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isMenuActive()",               WRAP_MFN(CMenuMgr, isMenuActive),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isMenuItemActive()",           WRAP_MFN(CMenuMgr, isMenuItemActive),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isInterfaceItemActive()",      WRAP_MFN(CMenuMgr, isInterfaceItemActive), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void resetTransform()",             WRAP_MFN(CMenuMgr, resetTransform),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void resetDynamicOffset()",         WRAP_MFN(CMenuMgr, resetDynamicOffset),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void allow(bool allow = true)",     WRAP_MFN(CMenuMgr, allow),                 asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void update()",                     asFUNCTION(Update),                        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void transform()",                  asFUNCTION(Transform),                     asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void setCommandBuffer(string &in)", WRAP_OBJ_LAST(SetCommandBuffer),           asCALL_GENERIC) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CMenuMgr MenuMgr", &CMenuMgr::Instance()) );
    }
}
