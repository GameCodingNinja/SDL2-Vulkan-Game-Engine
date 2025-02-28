
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "startupstate.h"

// Game dependencies
#include "titlescreenstate.h"
#include "statedefs.h"

// Game lib dependencies
#include <managers/fontmanager.h>
#include <managers/actionmanager.h>
#include <sound/soundmanager.h>
#include <strategy/strategymanager.h>
#include <objectdata/objectdatamanager.h>
#include <common/color.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptpoint.h>
#include <script/scriptsize.h>
#include <script/scriptglobals.h>
#include <script/scriptsprite.h>
#include <script/scriptuicontrol.h>
#include <script/scriptmenu.h>
#include <script/scriptvisual.h>
#include <script/scripthighresolutiontimer.h>
#include <script/scriptmenumanager.h>
#include <script/scriptactionmanager.h>
#include <script/scriptdevice.h>
#include <script/scriptsettings.h>
#include <script/scriptbitmask.h>
#include <script/scripteventstub.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworldmanager2d.h>
#include <strategy/strategyloader.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <thread>
#include <chrono>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStartUpState::CStartUpState() :
    CCommonState( NStateDefs::EGS_STARTUP, NStateDefs::EGS_TITLE_SCREEN )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CStartUpState::~CStartUpState()
{
    CStrategyMgr::Instance().deleteStrategy( "_startup_" );
    CDevice::Instance().deleteCommandPoolGroup( "(startup)" );
    CObjectDataMgr::Instance().freeGroup( "(startup)" );
}


/************************************************************************
*    DESC:  Do any pre-load init's
************************************************************************/
void CStartUpState::init()
{
    // Register the script items
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptArray( CScriptMgr::Instance().getEnginePtr(), true );
    NScriptSize::Register();
    NScriptPoint::Register();
    NScriptBitmask::Register();
    NScriptEventStub::Register();
    NScriptColor::Register();
    NScriptGlobals::Register();
    NScriptVisual::Register();
    NScriptHighResolutionTimer::Register();
    NScriptSprite::Register();
    NScriptSound::Register();
    NScriptUIControl::Register();
    NScriptMenu::Register();
    NScriptMenuManager::Register();
    NScriptActionManager::Register();
    NScriptDevice::Register();
    NScriptSettings::Register();
    
    // Load list table data
    CObjectDataMgr::Instance().loadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
    CScriptMgr::Instance().loadListTable( "data/objects/scripts/scriptListTable.lst" );
    CStrategyMgr::Instance().loadListTable( "data/objects/strategy/strategyListTable.lst" );

    // Load group specific assets
    CScriptMgr::Instance().loadGroup("(state)");
    CObjectDataMgr::Instance().loadGroup( "(startup)" );
    
    // Add the command buffers to the menu manager
    CMenuMgr::Instance().setCommandBuffers( "(menu)" );

    // Load the Strategy
    CStrategyloader::Instance().load( "data/objects/strategy/state/startup.loader" );

    // Start the fade in
    m_scriptComponent.prepare( "(state)", "State_StartUpFadeIn" );
    
    // Reset the elapsed time before entering the render loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CStartUpState::handleEvent( const SDL_Event & rEvent )
{
    // Event sent from script
    if( rEvent.type == NStateDefs::ESE_FADE_IN_COMPLETE )
    {
        std::thread load(&CStartUpState::assetLoad, this);
        load.detach();
    }
    // Event sent from thread
    else if( rEvent.type == NStateDefs::ESE_THREAD_LOAD_COMPLETE )
    {
        m_scriptComponent.prepare( "(state)", "State_FadeOut" );
    }
    // Event sent from script
    else if( rEvent.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
    {
        m_changeState = true;
    }
}


/***************************************************************************
*    DESC:  Load the assets
****************************************************************************/
void CStartUpState::assetLoad()
{
    try
    {
        // Load the data list tables
        CObjectDataMgr::Instance().loadListTable( "data/objects/3d/objectDataList/dataListTable.lst" );
        CMenuMgr::Instance().loadListTable( "data/objects/2d/menu/menuListTable.lst" );
        CMenuMgr::Instance().loadMenuAction( "data/objects/2d/menu/menu_action.list" );
        CSoundMgr::Instance().loadListTable( "data/sound/soundListTable.lst" );
        CPhysicsWorldManager2D::Instance().loadListTable( "data/objects/2d/physics/physicsListTable.lst" );

        // Load in any fonts
        CFontMgr::Instance().load( "data/textures/fonts/font.lst" );

        // Load the action manager
        CActionMgr::Instance().loadActionFromXML( "data/settings/controllerMapping.cfg" );

        // Load the menu sounds
        CSoundMgr::Instance().loadGroup("(menu)");

        // Load group specific script items
        CScriptMgr::Instance().loadGroup("(menu)");

        // Load all of the meshes and materials in these groups
        CObjectDataMgr::Instance().loadGroup("(menu)");

        // Load the menu group
        CMenuMgr::Instance().loadGroup("(menu)");

        // Do the state specific load
        CTitleScreenState::load();

        NGenFunc::DispatchEvent( NStateDefs::ESE_THREAD_LOAD_COMPLETE );
    }
    catch( NExcept::CCriticalException & ex )
    {
        m_errorTitle = ex.getErrorTitle();
        m_errorMsg = ex.getErrorMsg();
    }
    catch( std::exception const & ex )
    {
        m_errorTitle = "Standard Exception";
        m_errorMsg = ex.what();
    }
    catch(...)
    {
        m_errorTitle = "Unknown Error";
        m_errorMsg = "Something bad happened and I'm not sure what it was.";
    }
}


/***************************************************************************
*    DESC:  Is the state done
****************************************************************************/
bool CStartUpState::doStateChange()
{
    // Re-throw any threaded exceptions
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException( m_errorTitle, m_errorMsg );
    
    return m_changeState;
}
