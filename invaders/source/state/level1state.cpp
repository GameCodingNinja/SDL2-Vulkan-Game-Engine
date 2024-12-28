
//
//  FILE NAME:       level1state.cpp
//
//  DESCRIPTION:     CLevel1State Class State
//

// Physical component dependency
#include "level1state.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <script/scriptmanager.h>
#include <sprite/sprite.h>
#include <strategy/strategy.h>
#include <strategy/strategymanager.h>
#include <strategy/strategyloader.h>
#include <managers/cameramanager.h>
#include <managers/actionmanager.h>
#include <node/inode.h>

// Standard lib dependencies
#include <vector>

#define PLAYER_SHIP_TOP_SPEED   0.12f
#define RADAR_SCALE             0.1f
#define CAMERA_EASING_SPEED     0.1f

//
//  DESC:  Constructor
//
CLevel1State::CLevel1State() :
    CCommonState( NStateDefs::EGS_LEVEL_1, NStateDefs::EGS_GAME_LOAD ),
    m_moveActionVec( {"left", "right", "up", "down"} ),
    m_lastMoveAction( EActionPress::_NULL_ ),
    m_moveDirX( MOVE_NULL ),
    m_moveDirY( MOVE_NULL ),
    m_lastMoveDirX( MOVE_NULL ),
    m_pPlayerShipStrategy(nullptr),
    m_pPlayerShipNode(nullptr),
    m_playerShipBoostSpeed(0)
{
}

//
//    DESC:  destructor
//
CLevel1State::~CLevel1State()
{
    CStrategyMgr::Instance().deleteStrategyLst( {"_background_", "_buildingsback_", "_buildingsfront_", "_buildings_", "_forground_"} );
    CDevice::Instance().deleteCommandPoolGroup( "(level_1)" );
    CObjectDataMgr::Instance().freeGroup( "(level_1)" );
    CScriptMgr::Instance().freeGroup("(level_1)");
}

//
//  DESC:  Do any pre-game loop init's
//
void CLevel1State::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().activateTree("pause_tree");

    // Activaye the strategies
    CStrategyMgr::Instance().activateStrategyLst(
        {"_background_", "_buildingsback_", "_buildingsfront_", "_buildings_", "_forground_"} );

    m_buildingsStrategy = CStrategyMgr::Instance().getStrategy("_buildings_");

    m_buildingsbackCamera = &CCameraMgr::Instance().get("buildingsbackCamera");
    m_buildingsfrontCamera = &CCameraMgr::Instance().get("buildingsfrontCamera");
    m_buildingsCamera = &CCameraMgr::Instance().get("buildingsCamera");
    m_forgroundCamera = &CCameraMgr::Instance().get("forgroundCamera");
    m_levelCamera = &CCameraMgr::Instance().get("levelCamera");
    m_wrapAroundCamera = &CCameraMgr::Instance().get("wrapAroundCamera");
    m_radarCamera1 = &CCameraMgr::Instance().get("radarCamera1");
    m_radarCamera2 = &CCameraMgr::Instance().get("radarCamera2");

    // Init the player ship
    initPlayerShip();

    // Start the fade
    m_scriptComponent.prepare( "(state)", "State_FadeIn" );

    m_gameReady = true;
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}

//
//  DESC:  Handle events
//
void CLevel1State::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );
    
    // Event sent from menu
    if( rEvent.type == NMenuEvent::GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen.
        if( rEvent.user.code == NTransCode::BEGIN )
            m_scriptComponent.prepare( "(state)", "State_FadeOut" );
    }
    // Event sent from script
    else if( rEvent.type == NStateDefs::ESE_FADE_IN_COMPLETE )
    {
        CMenuMgr::Instance().allow();
    }
    // Event sent from script
    else if( rEvent.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
    {
        // Clear out all the trees
        CMenuMgr::Instance().clearActiveTrees();
            
        // Set the flag to change the state
        m_changeState = true;
    }
    else
    {
        // Handle the ship movement
        handleShipMovement( rEvent );
    }
}

// 
//  DESC: Init the player ship
//
void CLevel1State::initPlayerShip()
{
    m_pPlayerShipStrategy = CStrategyMgr::Instance().activateStrategy("_player_ship_");
    m_pPlayerShipNode = m_pPlayerShipStrategy->getNode("player_ship");
}

// 
//  DESC: Handle the ship movement
//
void CLevel1State::handleShipMovement( const SDL_Event & rEvent )
{
    //float dir = -m_rLevelCamera.getPos().x - m_pPlayerShipNode->getSprite()->getPos().x;

    int i = 0;
    for(auto iter: m_moveActionVec)
    {
        const EActionPress actionResult = CActionMgr::Instance().wasAction( rEvent, iter );

        if( actionResult != EActionPress::IDLE )
        {
            if( i < MOVE_LEFT_RIGHT )
            {
                // Only act on action press for the last button down
                if( (actionResult == EActionPress::UP) && (i != m_lastMoveDirX) )
                    continue;

                if( i == MOVE_LEFT )
                {
                    // Flip the ship facing left
                    m_pPlayerShipNode->getObject()->setRot( 0, 180 );

                    // The camera easing positions the player ship at then end of the screen facing inwards
                    if( actionResult == EActionPress::DOWN )
                    {
                        NGenFunc::PostDebugMsg( "Move Left DOWN" );

                        m_easingX.init( m_easingX.getValue(), -(PLAYER_SHIP_TOP_SPEED + m_playerShipBoostSpeed), 2, NEasing::linear );

                        // Camera easing has to move slower or faster then the elements on the screen to avoid movement studder
                        // Don't allow any more camera easing, in this direction, after a certain point
                        //if(dir > -CAMERA_EASING_OFFSET)
                        //    m_cameraEasingX.init( m_cameraEasingX.getValue(), -CAMERA_EASING_SPEED, 1, NEasing::linear );
                    }
                    else
                    {
                        NGenFunc::PostDebugMsg( "Move Left UP" );

                        m_easingX.init( m_easingX.getValue(), 0, 3, NEasing::linear );

                        m_cameraEasingX.init( m_cameraEasingX.getValue(), 0, 1, NEasing::linear );
                    }

                    m_moveDirX = MOVE_LEFT;
                }
                else if( i == MOVE_RIGHT )
                {
                    // Flip the ship facing right
                    m_pPlayerShipNode->getObject()->setRot();

                    // The camera easing positions the player ship at then end of the screen facing inwards
                    if( actionResult == EActionPress::DOWN )
                    {
                        NGenFunc::PostDebugMsg( "Move Right DOWN" );

                        m_easingX.init( m_easingX.getValue(), PLAYER_SHIP_TOP_SPEED + m_playerShipBoostSpeed, 2, NEasing::linear );

                        // Camera easing has to move slower or faster then the elements on the screen to avoid movement studder
                        // Don't allow any more camera easing, in this direction, after a certain point
                        //if(dir < CAMERA_EASING_OFFSET)
                        //    m_cameraEasingX.init( m_cameraEasingX.getValue(), CAMERA_EASING_SPEED, 1, NEasing::linear );
                    }
                    else
                    {
                        NGenFunc::PostDebugMsg( "Move Right UP" );

                        m_easingX.init( m_easingX.getValue(), 0, 3, NEasing::linear );

                        m_cameraEasingX.init( m_cameraEasingX.getValue(), 0, 1, NEasing::linear );
                    }
                    
                    m_moveDirX = MOVE_RIGHT;
                }

                m_lastMoveDirX = m_moveDirX;
                m_lastMoveAction = actionResult;
            }
            else
            {
                if( i == MOVE_UP )
                {
                    if( actionResult == EActionPress::DOWN )
                    {
                        //NGenFunc::PostDebugMsg( "Move Up DOWN" );
                        //this.easingY.init( this.easingY.getValue(), 7, 0.5, easing.getLinear() );
                    }
                    else
                    {
                        //NGenFunc::PostDebugMsg( "Move Up UP" );
                        //this.easingY.init( this.easingY.getValue(), 0, 0.25, easing.getLinear() );
                    }

                    m_moveDirY = MOVE_UP;
                }
                else if( i == MOVE_DOWN )
                {
                    if( actionResult == EActionPress::DOWN )
                    {
                        //NGenFunc::PostDebugMsg( "Move Down DOWN" );
                        //this.easingY.init( this.easingY.getValue(), -7, 0.5, easing.getLinear() );
                    }
                    else
                    {
                        //NGenFunc::PostDebugMsg( "Move Down UP" );
                        //this.easingY.init( this.easingY.getValue(), 0, 0.25, easing.getLinear() );
                    }

                    m_moveDirY = MOVE_DOWN;
                }
            }
        }

        ++i;
    }
}

//
//  DESC:  Update objects that require them
//
void CLevel1State::update()
{
    if( !CMenuMgr::Instance().isActive() && m_gameReady )
    {
        m_easingX.execute();
        m_easingY.execute();
        m_cameraEasingX.execute();

        float easingVal = m_easingX.getValue() + m_cameraEasingX.getValue();

        m_levelCamera->incPos( easingVal );
        m_forgroundCamera->incPos( easingVal );
        m_buildingsCamera->incPos( easingVal );
        m_buildingsbackCamera->incPos( easingVal * 0.25f );
        m_buildingsfrontCamera->incPos( easingVal * 0.5f );

        // Loop the static backgrounds
        if( m_buildingsbackCamera->getPos().x < -LOOPING_BKG_WRAP_DIST )
            m_buildingsbackCamera->incPos( -(LOOPING_BKG_WRAP_DIST * 2) );
        else if( m_buildingsbackCamera->getPos().x > LOOPING_BKG_WRAP_DIST )
            m_buildingsbackCamera->incPos( LOOPING_BKG_WRAP_DIST * 2 );

        if( m_buildingsfrontCamera->getPos().x < -LOOPING_BKG_WRAP_DIST )
            m_buildingsfrontCamera->incPos( -(LOOPING_BKG_WRAP_DIST * 2) );
        else if( m_buildingsfrontCamera->getPos().x > LOOPING_BKG_WRAP_DIST )
            m_buildingsfrontCamera->incPos( LOOPING_BKG_WRAP_DIST * 2 );

        if( m_forgroundCamera->getPos().x < -LOOPING_BKG_WRAP_DIST )
            m_forgroundCamera->incPos( -(LOOPING_BKG_WRAP_DIST * 2) );
        else if( m_forgroundCamera->getPos().x > LOOPING_BKG_WRAP_DIST )
            m_forgroundCamera->incPos( LOOPING_BKG_WRAP_DIST * 2 );

        // Set the wrap around camera when we are about to exceed the range of the buildings
        if( m_buildingsCamera->getPos().x > -6200 && m_buildingsCamera->getPos().x < -4900 )
        {
            m_wrapAroundCamera->setPos( -(m_buildingsCamera->getPos().x + (GAMEPLAY_LOOPING_WRAP_DIST * 2)) );
            m_buildingsStrategy->setExtraCamera( m_wrapAroundCamera );
        }
        else if( m_buildingsCamera->getPos().x > 5000 && m_buildingsCamera->getPos().x < 6350 )
        {
            m_wrapAroundCamera->setPos( -(m_buildingsCamera->getPos().x - (GAMEPLAY_LOOPING_WRAP_DIST * 2)) );
            m_buildingsStrategy->setExtraCamera( m_wrapAroundCamera );
        }

        // Reset the building camera once we are done with filling the gap with the wrap around camera
        if( m_buildingsCamera->getPos().x < -6200 )
            m_buildingsCamera->incPos( -(GAMEPLAY_LOOPING_WRAP_DIST * 2) );
        else if( m_buildingsCamera->getPos().x > 6350)
            m_buildingsCamera->incPos( GAMEPLAY_LOOPING_WRAP_DIST * 2 );
        

        m_pPlayerShipNode->getObject()->incPos( m_easingX.getValue(), m_easingY.getValue() );
    }

    CCommonState::update();
}

//
//  DESC:  Transform the game objects
//
void CLevel1State::transform()
{
    CCommonState::transform();

    m_levelCamera->transform();
    m_forgroundCamera->transform();
    m_buildingsCamera->transform();
    m_buildingsbackCamera->transform();
    m_buildingsfrontCamera->transform();
    m_wrapAroundCamera->transform();
    m_radarCamera1->transform();
    m_radarCamera2->transform();

    if( m_gameReady )
    {
    }
}

//
//  DESC:  Record the command buffer vector in the device
//         for all the sprite objects that are to be rendered
//
void CLevel1State::recordCommandBuffer( uint32_t cmdBufIndex )
{
    if( m_gameReady )
    {
        CCommonState::recordCommandBuffer( cmdBufIndex );

        m_buildingsStrategy->setExtraCamera( nullptr );
    }
}


//
//  DESC:  Static function for loading the assets for this state
//         NOTE: Only call when the class is not allocated
//
void CLevel1State::load()
{
    // Load the level 1 script group
    CScriptMgr::Instance().loadGroup("(level_1)");

    // Load the level 1 art
    CObjectDataMgr::Instance().loadGroup( "(level_1)");

    // Load the Strategy
    CStrategyloader::Instance().loadGroup( "-level1-" );
}
