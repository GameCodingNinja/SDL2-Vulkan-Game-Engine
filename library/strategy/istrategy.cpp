
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
    m_idDir(1)
{
}


/************************************************************************
*    DESC:  Transform the sprites
************************************************************************/
void iStrategy::setCameraId( const std::string & cameraId )
{
    m_cameraId = cameraId;
}


/************************************************************************
*    DESC:  Enable event handling
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
