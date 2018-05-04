
/************************************************************************
*    FILE NAME:       bigpaybackstate.cpp
*
*    DESCRIPTION:     The Big Pay Back game state
************************************************************************/

// Physical component dependency
#include "bigpaybackstate.h"

// Game dependencies
#include "../game/gamesave.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/xmlpreloader.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <gui/menumanager.h>
#include <gui/uilabel.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <gui/uiprogressbar.h>
#include <gui/uibuttonlist.h>
#include <slot/slotmathmanager.h>
#include <slot/symbolsetviewmanager.h>
#include <slot/betmanager.h>
#include <slot/slotgroup.h>
#include <slot/animatedcycleresults.h>
#include <slot/slotgroupfactory.h>
#include <slot/playresult.h>
#include <script/scriptmanager.h>
#include <managers/soundmanager.h>
#include <managers/spritesheetmanager.h>
#include <managers/cameramanager.h>
#include <common/build_defs.h>

// Boost lib dependencies
#include <boost/bind.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructer
************************************************************************/
CBigPayBackState::CBigPayBackState( const std::string & group ) :
    CCommonState( NGameDefs::EGS_BIG_CASH_BACK, NGameDefs::EGS_GAME_LOAD ),
        m_stateGroup( group ),
        m_background( CObjectDataMgr::Instance().getData2D( group, "background" ) ),
	m_pig( CObjectDataMgr::Instance().getData2D( group, "Payback Pig" ) ),
        m_baseGameMusic( group, "SlotGame_StartSpinMusic", "SlotGame_StopSpinMusic", "SlotGame_FastStopSpinMusic", 7000 )
{
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CBigPayBackState::init()
{
    CCommonState::init();

    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().allow();
    CMenuMgr::Instance().activateTree( "menu_tree");
    CMenuMgr::Instance().activateTree( "confirmation_tree");
    CMenuMgr::Instance().activateTree( "buy_tree");
    CMenuMgr::Instance().activateTree( "base_game_tree");

    // Hook the Play button to the reel group
    CUIButton & rPlayBtn = CMenuMgr::Instance().getMenuControl<CUIButton>( "base_game_menu", "play_btn" );
    rPlayBtn.connect_executionAction( boost::bind(&CSlotGame::playGame, &m_slotGame, _1) );

    // Hook the total bet call back function to the total bet meter
    auto & rTotalBetMeter = CMenuMgr::Instance().getMenuControl<CUIButtonList>( "base_game_menu", "total_bet_meter" );
    auto & rTotalBetDecBtn = *rTotalBetMeter.findSubControl( std::string("total_bet_dec_btn") );
    auto & rTotalBetIncBtn = *rTotalBetMeter.findSubControl( std::string("total_bet_inc_btn") );
    rTotalBetDecBtn.connect_executionAction( boost::bind(&CBigPayBackState::totalBetCallBack, this, _1) );
    rTotalBetIncBtn.connect_executionAction( boost::bind(&CBigPayBackState::totalBetCallBack, this, _1) );

    // Create a play result
    auto & rPlayResult = m_slotGame.createPlayResult();

    // Create the slot group
    m_slotGame.addSlotGroup(
        NSlotGroupFactory::Create(
            NSlotDefs::ED_REEL,
            "main_reel_strip",
            "main_paytable",
            CSlotMathMgr::Instance().getSlotMath( m_stateGroup, "slot" ),
            CXMLPreloader::Instance().getNode( std::get<0>(NBigPayBack::reelGrpCfg) ),
            CXMLPreloader::Instance().getNode( std::get<0>(NBigPayBack::spinProfileCfg) ),
            CSymbolSetViewMgr::Instance().get( m_stateGroup, "base_game" ),
            rPlayResult,
            std::move(std::unique_ptr<iCycleResults>(new CAnimatedCycleResults( &rPlayResult ))) ) );

    // Init the front panel
    std::vector<CUIControl *> btnVec = {
        &CMenuMgr::Instance().getMenuControl<CUIControl>( "base_game_menu", "home_btn" ),
        &CMenuMgr::Instance().getMenuControl<CUIControl>( "base_game_menu", "menu_btn" ),
        &CMenuMgr::Instance().getMenuControl<CUIControl>( "base_game_menu", "buy_btn" ),
        &rTotalBetMeter };
    m_frontPanel.setButtons( &rPlayBtn, btnVec );

    m_frontPanel.setMeters(
        &CMenuMgr::Instance().getMenuControl<CUIMeter>( "base_game_menu", "win_meter" ),
        &CMenuMgr::Instance().getMenuControl<CUIMeter>( "base_game_menu", "credit_meter" ) );

    // Add slot game component
    m_slotGame.setFrontPanel( &m_frontPanel );
    m_slotGame.setGameMusic( &m_baseGameMusic );

    m_pig.setPos( CPoint<float>(-875,-200,0) );

    // Init the credit meter
    CMenuMgr::Instance().getMenuControl<CUIMeter>( "base_game_menu", "credit_meter" ).set( CBetMgr::Instance().getCredits()  );

    // Clear any preloaded XML files
    CXMLPreloader::Instance().clear();

    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( m_stateGroup, "Screen_FadeIn" );

    allowMusic( CGameSave::Instance().getPlaySpinMusic() );
    allowStopSounds( CGameSave::Instance().getPlayStopSounds() );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Do we play the spin music
************************************************************************/
void CBigPayBackState::allowMusic( bool allow )
{
    if( !allow )
        m_baseGameMusic.fastFadeDown();

    m_baseGameMusic.allowMusic( allow );
}


/************************************************************************
*    DESC:  Do we allow the stop sounds?
************************************************************************/
void CBigPayBackState::allowStopSounds( bool allow )
{
    m_slotGame.allowStopSounds( allow );
}


/***************************************************************************
*    DESC:  Call back for when the total be is changed
****************************************************************************/
void CBigPayBackState::totalBetCallBack(CUIControl *)
{
    const CUIButtonList & rTotalBetMeter = CMenuMgr::Instance().getMenuControl<CUIButtonList>( "base_game_menu", "total_bet_meter" );
    CBetMgr::Instance().setLineBet( rTotalBetMeter.getActiveIndex() + 1 );
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CBigPayBackState::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
        {
            m_scriptComponent.prepare( m_stateGroup, "Screen_FadeOut" );
            m_baseGameMusic.fastFadeDown();
        }
    }
    else if( rEvent.type == SDL_APP_WILLENTERBACKGROUND )
    {
        if( m_slotGame.getState() == NSlotDefs::ESLOT_IDLE )
            CSoundMgr::Instance().stopAllSound();
    }
}


/***************************************************************************
*    DESC:  Handle any misc processing before the real work is started
****************************************************************************/
void CBigPayBackState::miscProcess()
{
    m_slotGame.processGameState();
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CBigPayBackState::update()
{
    CCommonState::update();

    CScriptMgr::Instance().update();

    m_scriptComponent.update();

    m_baseGameMusic.update();

    if( !CMenuMgr::Instance().isMenuActive() )
        m_slotGame.update();
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CBigPayBackState::transform()
{
    CCommonState::transform();

    m_background.transform();
    m_pig.transform();
    m_slotGame.transform();
}


/***************************************************************************
*    DESC:  2D/3D Render of game content
****************************************************************************/
void CBigPayBackState::preRender()
{
    const CMatrix & orthoMatrix = CCameraMgr::Instance().getDefaultProjMatrix();
    m_background.render( orthoMatrix );
    m_slotGame.render( orthoMatrix );
    m_pig.render( orthoMatrix );

    CCommonState::preRender();
}


/***************************************************************************
*    DESC: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NBigPayBack
{
    /***************************************************************************
    *    DESC:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().loadGroup2D( "(big_pay_back)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(big_pay_back)" );
    }

    void Load()
    {
        // Load the state specific menu group
        CMenuMgr::Instance().loadGroup("(big_pay_back)", CMenuMgr::DONT_INIT_GROUP);

        // Load sound resources for the game
        CSoundMgr::Instance().loadGroup("(big_pay_back)");

        // Load state specific AngelScript functions
        CScriptMgr::Instance().loadGroup("(big_pay_back)");

        // Load the slot group stuff
        CSymbolSetViewMgr::Instance().loadGroup( "(big_pay_back)" );
        CSlotMathMgr::Instance().loadGroup( "(big_pay_back)" );
        CSlotMathMgr::Instance().loadPaylineSetFromFile( "data/objects/2d/slot/payline_4x5.cfg" );

        // Set the line bet and the total number of lines bet
        CBetMgr::Instance().setLineBet(1);
        CBetMgr::Instance().setTotalLines( CSlotMathMgr::Instance().getPaylineSet("40_4x5").getLineData().size() );

        // Free the sprite sheet data manager because it's no longer needed
        CSpriteSheetMgr::Instance().clear();

        // Preload some needed XML files
        CXMLPreloader::Instance().clear();
        CXMLPreloader::Instance().load( std::get<0>(reelGrpCfg), std::get<1>(reelGrpCfg) );
        CXMLPreloader::Instance().load( std::get<0>(spinProfileCfg), std::get<1>(spinProfileCfg) );
    }

    void CriticalInit()
    {
        // Creates the font strings, run init scripts
        CMenuMgr::Instance().initGroup("(big_pay_back)");
    }


    /***************************************************************************
    *    DESC:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CMenuMgr::Instance().cleanUpGroup("(big_pay_back)");
        CObjectDataMgr::Instance().freeGroup2D( "(big_pay_back)" );

        // Unload the slot group stuff
        CSymbolSetViewMgr::Instance().clear();
    }

    void Unload()
    {
        // Unload the state specific menu group
        CMenuMgr::Instance().freeGroup("(big_pay_back)");

        // Unload the slot group stuff
        CSlotMathMgr::Instance().clear();

        // Unload state specific AngelScript functions
        CScriptMgr::Instance().freeGroup("(big_pay_back)");

        // Unload sound resources for the game
        CSoundMgr::Instance().freeGroup("(big_pay_back)");
    }

}
