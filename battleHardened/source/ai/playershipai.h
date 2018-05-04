
/************************************************************************
*    FILE NAME:       playershipai.h
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

#ifndef __player_ship_ai_h__
#define __player_ship_ai_h__

// Physical component dependency
#include <common/iaibase.h>

// Game lib dependencies
#include <common/point.h>
#include <utilities/timer.h>

// Forward declaration(s)
class CActorSprite2D;
class CSprite2D;
class iSprite;

class CPlayerShipAI : public iAIBase
{
public:

    // Constructor
    CPlayerShipAI( iSprite * pSprite );

    // Destructor
    virtual ~CPlayerShipAI();
    
    // Do any initializing
    void init() override;
    
    // Handle player related messages
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Update animations, move sprites, etc.
    void update() override;
    
private:
    
    // Handle the ship's shooting 
    void handleShooting();
    
private:
    
    // Type specific sprite
    CActorSprite2D & m_sprite;
    
    // Fire tail sprite
    CSprite2D & m_fireTailSprite;
    
    // Gun sprite
    CSprite2D & m_gunSprite;
    
    // Gun rotation in radians
    float m_gunRotation;
    
    // Gun xy value to calculate rotation
    CPoint<float> m_gunXY;
    
    // Player shoot timer
    CTimer m_playerShootTimer;
};

#endif  // __player_ship_ai_h__


