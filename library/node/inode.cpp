
/************************************************************************
*    FILE NAME:       inode.cpp
*
*    DESCRIPTION:     Node interface class. Also used as a leaf node
************************************************************************/

// Physical component dependency
#include <node/inode.h>

std::atomic<handle16_t> iNode::m_hAtomicIter = defs_DEFAULT_HANDLE;

// Dummy reuseable variables
float dummyRadius = 0.f;
CSize<float> dummySize;

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
iNode::iNode( uint8_t nodeId, uint8_t parentId ) :
    m_headNode(false),
    m_type(ENodeType::_NULL_),
    m_handle(m_hAtomicIter++),
    m_userId(defs_DEFAULT_ID),
    m_nodeId(nodeId),
    m_parentId(parentId),
    m_crcUserId(0)
{}

iNode::~iNode()
{}

/***************************************************************************
*    DESC:  Only called after node creation to indicate head node
****************************************************************************/
void iNode::init()
{
    m_headNode = true;
}

/************************************************************************
*    DESC:  get the child node
*           NOTE: This is a recursive function
************************************************************************/
iNode * iNode::findChild( const uint16_t crcValue )
{
    if( crcValue == m_crcUserId )
        return this;

    return nullptr;
}

/***************************************************************************
*    DESC:  Get the radius
****************************************************************************/
float iNode::getRadius()
{
    return dummyRadius;
}

/***************************************************************************
*    DESC:  Get the size
****************************************************************************/
CSize<float> iNode::getSize()
{
    return dummySize;
}