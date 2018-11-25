
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
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType( "CMenuMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "CUIControl & getMenuControl(string &in, string &in)", asMETHOD(CMenuMgr, getPtrToMenuControl), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void loadListTable(string &in)",                 asFUNCTION(LoadListTable),              asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void loadMenuAction(string &in)",                asMETHOD(CMenuMgr, loadMenuAction), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void loadGroup(string &in, bool doInit = true)", asFUNCTION(LoadGroup),              asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void initGroup(string &in)",                     asFUNCTION(InitGroup),              asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void freeGroup(string &in)",                     asFUNCTION(FreeGroup),              asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateMenu(string &in, string &in, string &in)", asFUNCTION(ActivateMenu1),   asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateMenu(string &in, string &in)",             asFUNCTION(ActivateMenu2),   asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateTree(string &in, string &in)",             asFUNCTION(ActivateTree1),   asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void activateTree(string &in)",                         asFUNCTION(ActivateTree2),   asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void deactivateTree(string &in, string &in)",           asFUNCTION(DeactivateTree1),   asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void deactivateTree(string &in)",                       asFUNCTION(DeactivateTree2),   asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void clearActiveTrees()",                               asMETHOD(CMenuMgr, clearActiveTrees),  asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "CMenu & getMenu(string &in)",                           asFUNCTION(GetMenu),   asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "CMenu & getActiveMenu()",                               asFUNCTION(GetActiveMenu),   asCALL_CDECL_OBJLAST) );
        
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isActive()",                asMETHOD(CMenuMgr, isActive), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isMenuActive()",            asMETHOD(CMenuMgr, isMenuActive), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isMenuItemActive()",        asMETHOD(CMenuMgr, isMenuItemActive), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "bool isInterfaceItemActive()",   asMETHOD(CMenuMgr, isInterfaceItemActive), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void resetDynamicOffset()",      asMETHOD(CMenuMgr, resetDynamicOffset), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void allow(bool allow = true)",  asMETHOD(CMenuMgr, allow), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void update()",                  asMETHOD(CMenuMgr, update), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void transform()",               asMETHOD(CMenuMgr, transform), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CMenuMgr", "void setCommandBuffer(string &in)",   asFUNCTION(SetCommandBuffer), asCALL_CDECL_OBJLAST) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CMenuMgr MenuMgr", &CMenuMgr::Instance()) );
    }
}
