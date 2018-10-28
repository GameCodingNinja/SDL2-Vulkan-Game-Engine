
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "startupstate.h"

// Game dependencies
#include "titlescreenstate.h"

// Game lib dependencies
#include <managers/fontmanager.h>
#include <managers/soundmanager.h>
#include <managers/actionmanager.h>
#include <managers/spritesheetmanager.h>
#include <managers/cameramanager.h>
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
//#include <script/scriptuicontrol.h>
//#include <script/scriptmenu.h>
#include <script/scripthighresolutiontimer.h>
#include <sprite/sprite.h>
#include <system/device.h>
#include <gui/menumanager.h>
//#include <gui/menu.h>
//#include <gui/uicontrol.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworldmanager2d.h>
//#include <slot/slotmathmanager.h>
//#include <slot/symbolsetviewmanager.h>
#include <strategy/actorstrategy.h>
#include <node/inode.h>

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
    iGameState( NGameDefs::EGS_STARTUP, NGameDefs::EGS_TITLE_SCREEN )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CStartUpState::~CStartUpState()
{
    CDevice::Instance().deleteCommandPoolGroup( "(startup)" );
    CStrategyMgr::Instance().clear();
}


/************************************************************************
*    DESC:  Do any pre-load init's
************************************************************************/
void CStartUpState::init()
{
    // Load the object data list table
    CObjectDataMgr::Instance().loadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );

    // Load the script list table
    CScriptMgr::Instance().loadListTable( "data/objects/2d/scripts/scriptListTable.lst" );

    // Register the script items needed for startup animation
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    NScriptGlobals::Register();
    NScriptPoint::Register();
    NScriptSize::Register();
    NScriptColor::Register();
    NScriptHighResolutionTimer::Register();
    NScriptSprite::Register();

    // Load group specific script items
    CScriptMgr::Instance().loadGroup("(startup)");

    // Load the start up animation group
    CObjectDataMgr::Instance().loadGroup2D( "(startup)" );

    // Create the group command buffers
    m_commandBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(startup)" );

    // Reset the elapsed time before entering the render loop
    CHighResTimer::Instance().calcElapsedTime();

    // Add the actor strategy
    CStrategyMgr::Instance().addStrategy(
        "actorStrategy", new CActorStrategy( "data/objects/2d/spritestrategy/nodeList.lst" ) );
    
    // Start the fade in
    m_pLogo = CStrategyMgr::Instance().create( "actorStrategy", "spriteNode" );
    m_pLogo->getSprite()->prepare( "fadeIn" );
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CStartUpState::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == 0xA000 )
    {
        std::thread load(&CStartUpState::assetLoad, this);
        load.detach();
    }
    else if( rEvent.type == 0xA001 )
    {
        m_pLogo->getSprite()->prepare( "fadeOut" );
    }
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CStartUpState::update()
{
    CStrategyMgr::Instance().update();
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CStartUpState::transform()
{
    CStrategyMgr::Instance().transform();
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CStartUpState::recordCommandBuffer( uint32_t index )
{
    auto cmdBuf( m_commandBufVec[index] );

    CDevice::Instance().beginCommandBuffer( index, cmdBuf );

    CStrategyMgr::Instance().recordCommandBuffer( index, cmdBuf, CCameraMgr::Instance().getFinalMatrix() );

    CDevice::Instance().endCommandBuffer( cmdBuf );
}


/***************************************************************************
*    DESC:  Load the assets
****************************************************************************/
void CStartUpState::assetLoad()
{
    // Load in any fonts
    CFontMgr::Instance().load( "data/textures/fonts/font.lst" );

    // Load 3D object data list table
    CObjectDataMgr::Instance().loadListTable( "data/objects/3d/objectDataList/dataListTable.lst" );

    // Load the actor list table
    CStrategyMgr::Instance().loadListTable( "data/objects/2d/spritestrategy/strategyListTable.lst" );

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
    //NScriptUIControl::Register();
    //NScriptMenu::Register();

    // Load group specific script items
    //CScriptMgr::Instance().loadGroup("(menu)");

    // Load all of the meshes and materials in these groups
    //CObjectDataMgr::Instance().loadGroup2D("(menu)");

    // Load the menu group
    //CMenuMgr::Instance().loadGroup("(menu)");

    // Do the state specific load
    //NTitleScreenState::ObjectDataLoad();
    //NTitleScreenState::CriticalLoad();
    //NTitleScreenState::Load();
    //NTitleScreenState::CriticalInit();
    
    NGenFunc::DispatchEvent( 0xA001 );
}


/***************************************************************************
*    DESC:  Is the state done
****************************************************************************/
bool CStartUpState::doStateChange()
{
    // Do the fade in
    /*fade( *m_upSpriteLogo.get(), 500.f, CColor(0,0,0,1), CColor(1,1,1,1) );

    CHighResTimer::Instance().timerStart();

    assetLoad();

    const int time = static_cast<int>(CHighResTimer::Instance().timerStop());

    if( time < 2000.f )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 - time ) );

    // Do the fade out
    fade( *m_upSpriteLogo.get(), 500.f, CColor(1,1,1,1), CColor(0,0,0,1) );*/

    return false;
}

