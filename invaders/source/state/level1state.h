
/************************************************************************
*    FILE NAME:       level1state.h
*
*    DESCRIPTION:     CLevel1State Class State
************************************************************************/

#pragma once

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <common/camera.h>
#include <utilities/easing.h>
#include <utilities/timer.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
class CStrategy;
class iNode;
class CSprite;

enum
{
    MOVE_NULL = -1,
    MOVE_LEFT = 0,
    MOVE_RIGHT = 1,
    MOVE_LEFT_RIGHT = 2,
    MOVE_UP = 2,
    MOVE_DOWN = 3,
    CAMERA_EASING_DIVISOR = 4,
    CAMERA_EASING_OFFSET = 350,
    PLAYER_SHIP_BOOST_TOP_SPEED = 16,
    MAX_CLOUDS = 8,
    PLAYER_SHIP_ID = 0,
    ENEMY_SHOT_ID = -2,
    ENEMY_SHIP_ID = -3,
    CLOUD_MIN_Y = 150,
    CLOUD_MAX_Y = -300,
    LOOPING_BKG_WRAP_DIST = 1280,
    GAMEPLAY_LOOPING_WRAP_DIST = 5600,
};

class CLevel1State : public CCommonState
{
public:

    // Constructor
    CLevel1State();

    // Destructor
    virtual ~CLevel1State();
    
    // Do any pre-game loop init's
    void init() override;

    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform();

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t cmdBufIndex ) override;
    
    // Static load function
    static void load();

protected:

    // Init the player ship
    void initPlayerShip();

    // Init the misc objects
    void initMiscObjects();

    // Handle the ship movement
    void handleShipMovement( const SDL_Event & rEvent );

    // Handle the cloud movement
    void handleCloudMovement();

    // Handle the train spawn
    void handleTrainSpawn( float easingVal );

private:

    struct SCloud
    {
        iNode * pNode = nullptr;
        float speed = 0.f;
    };

    struct STrain
    {
        iNode * pNode = nullptr;
        CSprite * pSprite = nullptr;
        int dir = 0;
        CTimer timer;
        CStrategy * trainStrategy = nullptr;
    };
    
    bool m_gameReady;

    CStrategy * m_buildingsStrategy;
    CStrategy * m_backgroundStrategy;

    std::vector<SCloud> m_cloudVec;

    // The cameras
    CCamera * m_buildingsbackCamera;
    CCamera * m_buildingsfrontCamera;
    CCamera * m_buildingsCamera;
    CCamera * m_forgroundCamera;
    CCamera * m_levelCamera;
    CCamera * m_wrapAroundCamera;
    CCamera * m_radarCamera1;
    CCamera * m_radarCamera2;

    // Easing classes
    CEasing m_easingX;
    CEasing m_easingY;
    CEasing m_cameraEasingX;

    // Movement strings
    std::vector<std::string> m_moveActionVec;

    // Move direction
    EActionPress m_lastMoveAction;
    int m_moveDirX;
    int m_moveDirY;
    int m_lastMoveDirX;

    // Player ship members
    CStrategy * m_pPlayerShipStrategy;
    iNode * m_pPlayerShipNode;

    float m_playerShipBoostSpeed;

    STrain m_train;
};
