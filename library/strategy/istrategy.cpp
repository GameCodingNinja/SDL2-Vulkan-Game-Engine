
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
