
/************************************************************************
*    FILE NAME:       projectileai.h
*
*    DESCRIPTION:     Class projectile AI
************************************************************************/

#ifndef __projectile_ai_h__
#define __projectile_ai_h__

// Physical component dependency
#include <common/iaibase.h>

// Game lib dependencies
#include <common/point.h>

// Forward declaration(s)
class CSprite2D;
class iSprite;
class CBasicSpriteStrategy;

class CProjectileAI : public iAIBase
{
public:

    // Constructor
    CProjectileAI( iSprite * pSprite );

    // Destructor
    virtual ~CProjectileAI();
    
    // Do any initializing
    void init() override;
    
    // Update animations, move sprites, etc.
    void update() override;
    
private:
    
    // Type specific sprite
    CSprite2D & m_sprite;
    
    // The velocity and ship momentum of the projectile
    CPoint<float> m_velocity;
    
    // The initial position of the projectile
    CPoint<float> m_initialPos;
    
    // A flag for the first update of this projectile
    bool m_firstUpdate;
    
    // Reference to sprite strategy.
    CBasicSpriteStrategy & m_rStrategy;
};

#endif  // __projectile_ai_h__


