
/************************************************************************
*    FILE NAME:       ballai.cpp
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

// Physical component dependency
#include "ballai.h"

// Game lib dependencies
#include <2d/sprite2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBallAI::CBallAI( iSprite * pSprite ) :
    m_sprite(*dynamic_cast<CSprite2D *>(pSprite)),
    m_generator(std::random_device{}())
{
}   // constructor


/************************************************************************
*    DESC:  destructor
************************************************************************/
CBallAI::~CBallAI()
{
}   // destructor


/************************************************************************
*    DESC:  Do any initializing                                                            
************************************************************************/
void CBallAI::init()
{
    initPhysics();
    
}   // Init


/************************************************************************
*    DESC:  Update animations, move sprites, etc.
************************************************************************/
void CBallAI::update()
{
    if( m_sprite.getPos().y < -600.f )
        initPhysics();
    
}   // Update


/************************************************************************
*    DESC:  Init the ball placement
************************************************************************/
void CBallAI::initPhysics()
{
    std::uniform_int_distribution<int> m_RandX(-700,700);
    std::uniform_int_distribution<int> m_RandY(600,1000);
    std::uniform_int_distribution<int> m_RandR(0,360);
    
    m_sprite.getPhysicsComponent().setTransform(
        m_RandX(m_generator),
        m_RandY(m_generator),
        m_RandR(m_generator) );
    
}   // InitPhysics
