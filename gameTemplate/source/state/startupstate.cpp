
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
#include <managers/vertexbuffermanager.h>
#include <managers/shadermanager.h>
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
#include <script/scriptisprite.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptuicontrol.h>
#include <script/scriptmenu.h>
#include <script/scriptshadermanager.h>
#include <script/scripthighresolutiontimer.h>
#include <2d/sprite2d.h>
#include <sprite/sprite.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/menu.h>
#include <gui/uicontrol.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworldmanager2d.h>
#include <slot/slotmathmanager.h>
#include <slot/symbolsetviewmanager.h>

#include <sprite/sprite.h>

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
}


/************************************************************************
*    DESC:  Do any pre-load init's
************************************************************************/
void CStartUpState::init()
{
    // Load the object data list table
    CObjectDataMgr::Instance().loadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );

    // Load the start up animation group
    CObjectDataMgr::Instance().loadGroup2D( "(startup)" );
    
    // Set code for new sprite
    //CSprite sprite( CObjectDataMgr::Instance().getData2D( "(startup)", "waffles" ) );

    // Allocate the sprite to fade in
    m_upSpriteLogo.reset( new CSprite( CObjectDataMgr::Instance().getData2D( "(startup)", "logo" ) ) );
    m_upSpriteWaffles.reset( new CSprite( CObjectDataMgr::Instance().getData2D( "(startup)", "waffles" ) ) );
    
    m_upSpriteLogo->getObject()->setPos( -200, 0, -10 );
    m_upSpriteWaffles->getObject()->setPos( 200, 0, -10 );
    
    

    // Reset the elapsed time before entering the render loop
    CHighResTimer::Instance().calcElapsedTime();
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CStartUpState::update()
{
    /*m_scriptComponent.update();

    float rot = CHighResTimer::Instance().getElapsedTime() * 0.04;
    m_cube.incRot( rot, rot, 0 );*/
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CStartUpState::transform()
{
    m_upSpriteLogo->getObject()->transform();
    m_upSpriteWaffles->getObject()->transform();
    
    
    
    
    /*m_background.transform();

    CCameraMgr::Instance().transform();

    //m_spriteSheetTest.Transform();

    m_cube.transform();*/
}


/***************************************************************************
*    decs:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CStartUpState::recordCommandBuffer( uint32_t cmdBufIndex )
{
    m_upSpriteLogo->recordCommandBuffers( cmdBufIndex, CCameraMgr::Instance().getDefaultProjMatrix() );
    m_upSpriteWaffles->recordCommandBuffers( cmdBufIndex, CCameraMgr::Instance().getDefaultProjMatrix() );
    
    
    
    
    /*m_background.render( CCameraMgr::Instance().getDefaultProjMatrix() );

    //m_spriteSheetTest.Render( orthoMatrix );

    auto & camera = CCameraMgr::Instance().getActiveCamera();

    m_cube.render( camera.getFinalMatrix(), camera.getRotMatrix() );*/
}


/***************************************************************************
*    DESC:  Load the assets
****************************************************************************/
void CStartUpState::assetLoad()
{
    // Load in any fonts
    /*CFontMgr::Instance().load( "data/textures/fonts/font.lst" );

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
    //CSoundMgr::Instance().LoadGroup("(effects)");

    // Load the script list table
    CScriptMgr::Instance().loadListTable( "data/objects/2d/scripts/scriptListTable.lst" );

    // Load the physics list table
    CPhysicsWorldManager2D::Instance().loadListTable( "data/objects/2d/physics/physicsListTable.lst" );

    // Register the script items
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptArray( CScriptMgr::Instance().getEnginePtr(), false );
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptSize::Register();
    NScriptiSprite::Register();
    NScriptSound::Register();
    NScriptPlayLst::Register();
    NScriptSoundManager::Register();
    NScriptUIControl::Register();
    NScriptMenu::Register();
    NScriptShaderManager::Register();
    NScriptHighResolutionTimer::Register();

    // Load group specific script items
    CScriptMgr::Instance().loadGroup("(menu)");

    // Load all of the meshes and materials in these groups
    CObjectDataMgr::Instance().loadGroup2D("(menu)");

    // Load the menu group
    CMenuMgr::Instance().loadGroup("(menu)");

    // Do the state specific load
    NTitleScreenState::ObjectDataLoad();
    NTitleScreenState::CriticalLoad();
    NTitleScreenState::Load();
    NTitleScreenState::CriticalInit();*/
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


/************************************************************************
*    DESC:  Fade to color
************************************************************************/
void CStartUpState::fade(
    CSprite2D & sprite,
    float time,
    const CColor & cur,
    const CColor & finalColor )
{
    /*CColor inc = (finalColor - cur) / time;
    CColor current = cur;

    do
    {
        // Get the elapsed time
        CHighResTimer::Instance().calcElapsedTime();

        time -= CHighResTimer::Instance().getElapsedTime();
        current += inc * CHighResTimer::Instance().getElapsedTime();

        if( time < 0 )
            current = finalColor;

        // Clear the screen
        //glClear( GL_COLOR_BUFFER_BIT );

        CShaderMgr::Instance().setShaderColor( "shader_2d", "additive", current );
        sprite.render( CCameraMgr::Instance().getDefaultProjMatrix() );

        SDL_GL_SwapWindow( CDevice::Instance().getWindow() );

        // Unbind everything after a round of rendering
        //CShaderMgr::Instance().unbind();
        //CTextureMgr::Instance().unbind();
        //CVertBufMgr::Instance().unbind();

        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
    }
    while( time > 0 );*/
}