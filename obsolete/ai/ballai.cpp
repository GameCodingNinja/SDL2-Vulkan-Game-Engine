
/************************************************************************
*    FILE NAME:       ballai.cpp
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

// Physical component dependency
#include "ballai.h"

// Game lib dependencies
#include <sprite/sprite.h>
#include <2d/object2d.h>
#include <physics/iphysicscomponent.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBallAI::CBallAI( CSprite * pSprite ) :
    m_sprite(*pSprite),
    m_generator(std::random_device{}())
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CBallAI::~CBallAI()
{
}


/************************************************************************
*    DESC:  Do any initializing                                                            
************************************************************************/
void CBallAI::init()
{
    initPhysics();
}


/************************************************************************
*    DESC:  Update animations, move sprites, etc.
************************************************************************/
void CBallAI::update()
{
    if( m_sprite.getObject()->getPos().y > 600.f )
        initPhysics();
}


/************************************************************************
*    DESC:  Init the ball placement
************************************************************************/
void CBallAI::initPhysics()
{
    std::uniform_int_distribution<int> m_RandX(-700,700);
    std::uniform_int_distribution<int> m_RandY(600,1000);
    std::uniform_int_distribution<int> m_RandR(0,360);
    
    m_sprite.getPhysicsComponent()->setTransform(
        m_RandX(m_generator),
        -m_RandY(m_generator),
        m_RandR(m_generator) );
}
