
/************************************************************************
*    FILE NAME:       playershipai.cpp
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

// Physical component dependency
#include "playershipai.h"

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <2d/sprite2d.h>
#include <utilities/settings.h>
#include <managers/actionmanager.h>
#include <strategy/strategymanager.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPlayerShipAI::CPlayerShipAI( iSprite * pSprite ) :
    m_sprite( *dynamic_cast<CActorSprite2D *>(pSprite) ),
    m_fireTailSprite( m_sprite.getSprite("player_fire_tail") ),
    m_gunSprite( m_sprite.getSprite("ship_gun") ),
    m_gunRotation(0),
    m_playerShootTimer(200, true)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CPlayerShipAI::~CPlayerShipAI()
{
}


/************************************************************************
*    DESC:  Do any initalizing
************************************************************************/
void CPlayerShipAI::init()
{
    m_fireTailSprite.prepare( "Play", {24, &m_fireTailSprite, true} );
}


/************************************************************************
*    DESC:  Handle player related messages
************************************************************************/
void CPlayerShipAI::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == SDL_MOUSEMOTION )
    {
        // Get the position of the gun
        const CPoint<float> pos = m_sprite.getSprite(-1).getTransPos();
        const float ratio = 1 / CSettings::Instance().getOrthoAspectRatio().getH();
        m_gunXY.x = ((ratio * (float)rEvent.motion.x) - pos.x) - CSettings::Instance().getDefaultSizeHalf().w;
        m_gunXY.y = CSettings::Instance().getDefaultSizeHalf().h - ((ratio * (float)rEvent.motion.y) + pos.y);
    }
    else if( rEvent.type == SDL_CONTROLLERAXISMOTION )
    {
        if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
        {
            m_gunXY.x = rEvent.caxis.value;
        }
        else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
        {
            m_gunXY.y = -rEvent.caxis.value;
        }
    }
    else if( CActionMgr::Instance().wasAction( rEvent, "Shoot", NDefs::EAP_DOWN ) )
    {
        handleShooting();
    }
}


/************************************************************************
*    DESC:  Update animations, move sprites, etc.
************************************************************************/
void CPlayerShipAI::update()
{
    m_gunRotation = atan2( m_gunXY.y, m_gunXY.x );
    m_gunSprite.setRot( 0, 0, m_gunRotation - m_sprite.getRot().z, false );
}


/************************************************************************
*    DESC:  Handle the ship's shooting
************************************************************************/
void CPlayerShipAI::handleShooting()
{
    // If the player hits the shoot button, and our time has expired, create a projectile actor
    if( m_playerShootTimer.expired() )
    {
        const CPoint<float> & centerPos = m_gunSprite.getCenterPos();
        const CPoint<float> & pos = m_gunSprite.getPos();

        CPoint<float> projectileOffset;
        const float OFFSET(60.f);
        projectileOffset.x = (OFFSET * cos( m_gunRotation )) - centerPos.x + pos.x;
        projectileOffset.y = (OFFSET * sin( m_gunRotation )) + centerPos.y + pos.y;

        // Create the projectile actor
        CStrategyMgr::Instance().create(
            "(actor)", "player_projectile", m_sprite.getPos() + projectileOffset, CPoint<float>(0,0,m_gunRotation) );

        m_playerShootTimer.reset();
    }
}
