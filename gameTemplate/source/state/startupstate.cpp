
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "startupstate.h"

// Game dependencies
#include "titlescreenstate.h"
#include "gamedefs.h"

// Game lib dependencies
#include <managers/fontmanager.h>
#include <managers/soundmanager.h>
#include <managers/actionmanager.h>
#include <strategy/strategymanager.h>
#include <objectdata/objectdatamanager.h>
#include <common/color.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptplaylist.h>
#include <script/scriptpoint.h>
#include <script/scriptsize.h>
#include <script/scriptglobals.h>
#include <script/scriptsprite.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptuicontrol.h>
#include <script/scriptmenu.h>
#include <script/scriptvisual.h>
#include <script/scripthighresolutiontimer.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworldmanager2d.h>
#include <strategy/actorstrategy.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <thread>
#include <chrono>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStartUpState::CStartUpState() :
    CCommonState( NGameDefs::EGS_STARTUP, NGameDefs::EGS_TITLE_SCREEN )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CStartUpState::~CStartUpState()
{
    // Wait for all rendering to be finished
    CDevice::Instance().waitForIdle();
    
    CStrategyMgr::Instance().deleteStrategy( "(startup)" );
    CDevice::Instance().deleteCommandPoolGroup( "(startup)" );
    CObjectDataMgr::Instance().freeGroup2D( "(startup)" );
}


/************************************************************************
*    DESC:  Do any pre-load init's
************************************************************************/
void CStartUpState::init()
{
    // Load list table data
    CObjectDataMgr::Instance().loadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
    CScriptMgr::Instance().loadListTable( "data/objects/scripts/scriptListTable.lst" );
    CStrategyMgr::Instance().loadListTable( "data/objects/spritestrategy/strategyListTable.lst" );

    // Register the script items needed for startup animation
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    NScriptGlobals::Register();
    NScriptPoint::Register();
    NScriptSize::Register();
    NScriptColor::Register();
    NScriptVisual::Register();
    NScriptHighResolutionTimer::Register();
    NScriptSprite::Register();

    // Load group specific assets
    CScriptMgr::Instance().loadGroup("(state)");
    CObjectDataMgr::Instance().loadGroup2D( "(startup)" );
    
    // Add the command buffers to the menu manager
    auto menuCmdBuf = CDevice::Instance().createSecondaryCommandBuffers( "(menu)" );
    CMenuMgr::Instance().setCommandBuffers( menuCmdBuf );

    // Create the group command buffers and add the actor strategy
    // Command buffers can only be used in the thread they are created
    auto cmdBuf = CDevice::Instance().createSecondaryCommandBuffers( "(startup)" );
    CStrategyMgr::Instance().addStrategy( "(startup)", new CActorStrategy() )->init( cmdBuf, true );
    
    // Add the logo
    CStrategyMgr::Instance().create( "(startup)", "waffles" );

    // Start the fade in
    m_scriptComponent.prepare( "(state)", "State_StartUpFadeIn" );
    
    // This data no longer needed so free it
    CObjectDataMgr::Instance().freeDataGroup2D( "(startup)" );
    
    // Reset the elapsed time before entering the render loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CStartUpState::handleEvent( const SDL_Event & rEvent )
{
    // Event sent from script
    if( rEvent.type == NGameDefs::EGE_FADE_IN_COMPLETE )
    {
        std::thread load(&CStartUpState::assetLoad, this);
        load.detach();
    }
    // Event sent from thread
    else if( rEvent.type == NGameDefs::EGE_THREAD_LOAD_COMPLETE )
    {
        m_scriptComponent.prepare( "(state)", "State_FadeOut" );
    }
    // Event sent from script
    else if( rEvent.type == NGameDefs::EGE_FADE_OUT_COMPLETE )
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
        // Load in any fonts
        CFontMgr::Instance().load( "data/textures/fonts/font.lst" );

        // Load 3D object data list table
        CObjectDataMgr::Instance().loadListTable( "data/objects/3d/objectDataList/dataListTable.lst" );

        // Load the action manager
        CActionMgr::Instance().loadActionFromXML( "data/settings/controllerMapping.cfg" );

        // Load menu list table
        CMenuMgr::Instance().loadListTable( "data/objects/2d/menu/menuListTable.lst" );

        // Load the menu action list
        CMenuMgr::Instance().loadMenuAction( "data/objects/2d/menu/menu_action.list" );

        // Load sound resources for the menu
        CSoundMgr::Instance().loadListTable( "data/sound/soundListTable.lst" );
        CSoundMgr::Instance().loadGroup("(menu)");

        // Load the physics list table
        CPhysicsWorldManager2D::Instance().loadListTable( "data/objects/2d/physics/physicsListTable.lst" );

        // Register the script items
        NScriptSound::Register();
        NScriptPlayLst::Register();
        NScriptSoundManager::Register();
        NScriptUIControl::Register();
        NScriptMenu::Register();

        // Load group specific script items
        CScriptMgr::Instance().loadGroup("(menu)");

        // Load all of the meshes and materials in these groups
        CObjectDataMgr::Instance().loadGroup2D("(menu)");

        // Load the menu group
        CMenuMgr::Instance().loadGroup("(menu)");

        // Do the state specific load
        CTitleScreenState::load();

        NGenFunc::DispatchEvent( NGameDefs::EGE_THREAD_LOAD_COMPLETE );
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
