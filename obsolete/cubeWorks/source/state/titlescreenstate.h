
/************************************************************************
 *    FILE NAME:       titlescreenstate.h
 *
 *    DESCRIPTION:     CRunState Class State
 ************************************************************************/

#ifndef __title_screen_state_h__
#define __title_screen_state_h__

// Physical component dependency
#include "commonstate.h"

// Standard lib dependencies
#include <map>
#include <memory>
#include <random>

// Game lib dependencies
#include <2d/sprite2d.h>
#include <3d/sprite3d.h>

// Forward declaration(s)
class CPhysicsWorld3d;

class CTitleScreenState : public CCommonState {
public:

    // Constructor
    CTitleScreenState();

    // Destructor
    virtual ~CTitleScreenState();

    // Handle the physics
    void Physics();

    // Update objects that require them
    void Update();

    // Transform the game objects
    void Transform();

    // 2D/3D Render of game content
    void PreRender();

    // Handle events
    virtual void HandleEvent(const SDL_Event & rEvent);

private:

    // Handle sprite signals
    void HandleSpriteSignal(int msg, int code1, int code2);

    // Create the box sprite explosion
    void CreateExplosion(const int spriteInde, const int consecutiveCount);

    // Generate a random color
    CColor RandomizeColor(uint seed);

    // Free the sprites in the queue
    void FreeSprites();

private:

    // title screen background
    CSprite2D m_background;

    // Map of allocated sprites
    std::map<int, CSprite3D *> pSpriteMap;

    // Sprite Delete queue
    std::vector<int> spriteDeleteVec;

    // Vector of sprites controlled by physics
    std::vector<CSprite3D *> pSpriteVec;

    // Sprite plane safe pointers
    std::unique_ptr<CSprite3D> m_upLeftPlane;
    std::unique_ptr<CSprite3D> m_upRightPlane;
    std::unique_ptr<CSprite3D> m_upFrontPlane;

    // Sprite Id inc
    int m_SpriteIdInc;

    // Allow for the state change
    bool m_allowStateChange;

    // The physics world
    CPhysicsWorld3d & m_physicsWorld;

    // Random number generator
    std::default_random_engine m_generator;

    // Max consecutive
    int m_consecutiveMax;

    // Time for random number seeding
    uint colorSeed;
};

#endif  // __title_screen_state_h__


