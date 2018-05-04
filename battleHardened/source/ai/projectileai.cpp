
/************************************************************************
*    FILE NAME:       projectileai.cpp
*
*    DESCRIPTION:     Class projectile AI
************************************************************************/

// Physical component dependency
#include "projectileai.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <strategy/basicspritestrategy.h>
#include <strategy/strategymanager.h>
#include <managers/soundmanager.h>
#include <utilities/settings.h>
#include <utilities/highresolutiontimer.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CProjectileAI::CProjectileAI( iSprite * pSprite ) :
    m_sprite( *dynamic_cast<CSprite2D *>(pSprite) ),
    m_firstUpdate(true),
    m_rStrategy(CStrategyMgr::Instance().get<CBasicSpriteStrategy>("(actor)"))
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CProjectileAI::~CProjectileAI()
{
}


/************************************************************************
*    DESC:  Do any initalizing
************************************************************************/
void CProjectileAI::init()
{
    // Speed of the projectile
    const float PROJECTILE_SPEED = 2.6f;

    // Get the sprite group
    m_initialPos = m_sprite.getPos();

    // Set the velocity of the projectile
    float rotation = m_sprite.getRot().z;
    m_velocity.x = cos( rotation );
    m_velocity.y = sin( rotation );
    m_velocity *= PROJECTILE_SPEED;
    //m_velocity += shooterVel; // value not needed sonce ship is not moving

    // Play the laser01 sound effect
    //CSoundMgr::Instance().Play( "(effects)", "laser" );
}


/************************************************************************
*    DESC:  Update animations, move sprites, etc.
************************************************************************/
void CProjectileAI::update()
{
    // If it's the projectile's first update, we don't want to increment
    // it's position by the velocity
    if( !m_firstUpdate )
        m_sprite.incPos( m_velocity * CHighResTimer::Instance().getElapsedTime() );
    else
        m_firstUpdate = false;

    // Did we collide with an enemy ship?
    //if( m_rStrategy.IsCollision( m_sprite ) != nullptr )
    //    m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, m_sprite.GetId() );

    // Delete if goes out of view
    if( m_sprite.getTransPos().getLengthSquared2D() > 250000.f )
        m_rStrategy.setToDestroy( m_sprite.getId() );
}
