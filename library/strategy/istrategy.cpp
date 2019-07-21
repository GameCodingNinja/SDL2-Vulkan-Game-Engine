
/************************************************************************
*    FILE NAME:       istrategy.cpp
*
*    DESCRIPTION:     Strategy Interface Class - Represents a layer of sprites
************************************************************************/

// Physical component dependency
#include <strategy/istrategy.h>

// Game lib dependencies
#include <managers/cameramanager.h>

int iStrategy::m_idInc = 0;

/************************************************************************
*    DESC:  Constructor
************************************************************************/
iStrategy::iStrategy() :
    m_pCamera( &CCameraMgr::Instance().getDefault() )
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
void iStrategy::setCamera( const std::string & cameraId )
{
    m_pCamera = &CCameraMgr::Instance().get( cameraId );
}

CCamera & iStrategy::getCamera()
{
    return *m_pCamera;
}
