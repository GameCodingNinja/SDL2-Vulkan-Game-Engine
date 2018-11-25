
/************************************************************************
*    FILE NAME:       istrategy.cpp
*
*    DESCRIPTION:     Strategy Interface Class - Represents a layer of sprites
************************************************************************/

// Physical component dependency
#include <strategy/istrategy.h>

int iStrategy::m_idInc = 0;

/************************************************************************
*    DESC:  Constructor
************************************************************************/
iStrategy::iStrategy() :
    m_idOffset(0),
    m_idDir(1),
    m_pCamera(nullptr)
{
}


/************************************************************************
 *    DESC:  Set the command buffers
 ************************************************************************/
void iStrategy::setCommandBuffers( std::vector<VkCommandBuffer> & commandBufVec )
{
    m_commandBufVec = commandBufVec;
}


/************************************************************************
*    DESC:  Set/Get the camera
************************************************************************/
void iStrategy::setCamera( CCamera * pCamera )
{
    m_pCamera = pCamera;
}

CCamera * iStrategy::getCamera()
{
    return m_pCamera;
}


/************************************************************************
*    DESC:  Clear the sprite Id counter
************************************************************************/
void iStrategy::clearSpriteCounter()
{
    m_idInc = 0;
}


/************************************************************************
*    DESC:  Customize sprite id generation by defining an offset
************************************************************************/
void iStrategy::setIdOffset( int offset )
{
    m_idOffset = offset;
}


/************************************************************************
*    DESC:  Customize sprite id generation by defining a direction
************************************************************************/
void iStrategy::setIdDir( int dir )
{
    m_idDir = dir;
}
