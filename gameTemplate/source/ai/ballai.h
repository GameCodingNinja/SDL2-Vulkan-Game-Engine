
/************************************************************************
*    FILE NAME:       ballai.h
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

#ifndef __ball_ai_h__
#define __ball_ai_h__

// Physical component dependency
#include <common/iaibase.h>

// Standard lib dependencies
#include <random>

// Forward declaration(s)
class iSprite;
class CSprite2D;

class CBallAI : public iAIBase
{
public:

    // Constructor
    CBallAI( iSprite * pSprite );

    // Destructor
    virtual ~CBallAI();
    
    // Do any initializing
    void init() override;
    
    // Update animations, move sprites, etc.
    void update() override;
    
private:
    
    // Init the ball placement
    void initPhysics();
    
private:
    
    // Sprite reference
    CSprite2D & m_sprite;
    
    // For ball random position generation
    std::default_random_engine m_generator;
};

#endif  // __projectile_ai_h__


