
/************************************************************************
*    FILE NAME:       spritenode.cpp
*
*    DESCRIPTION:     Sprite node that allows for children
************************************************************************/

// Physical component dependency
#include <node/spritenode.h>

// Game lib dependencies
#include <common/object.h>
#include <common/ivisualcomponent.h>
#include <node/nodedata.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CSpriteNode::CSpriteNode( const CNodeData & rNodeData ) :
    CRenderNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
    CSprite( CObjectDataMgr::Instance().getData( rNodeData.getGroup(), rNodeData.getObjectName() ) ),
    m_radius(0.f)
{
    m_userId = rNodeData.getUserId();
    m_type = ENodeType::SPRITE;

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    // Load the rest from XML node
    CSprite::load( rNodeData.getXMLNode() );

    // Init the physics
    CSprite::initPhysics();

    // Init the sprite
    CSprite::init();
}

CSpriteNode::~CSpriteNode()
{}

/***************************************************************************
*    DESC:  Only called after node creation with all it's children
****************************************************************************/
void CSpriteNode::init()
{
    iNode::init();

    m_size = getSprite()->getVisualComponent()->getSize();
    calcSize(this, m_size);

    // Calculate the radius
    m_radius = sqrt( pow((float)m_size.w / 2, 2) + pow((float)m_size.h / 2, 2) );
}

/***************************************************************************
*    DESC:  Update the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CSpriteNode::update()
{
    CSprite::physicsUpdate();
    CSprite::update();
    
    // Call inherited for recursion of children
    CRenderNode::update();
}

/***************************************************************************
*    DESC:  Translate the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CSpriteNode::transform()
{
    CSprite::transform();

    // Call inherited for recursion of children
    CRenderNode::transform();
}

void CSpriteNode::transform( const CObject & object )
{
    CSprite::transform( object );

    CRenderNode::transform();
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CSpriteNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    CSprite::recordCommandBuffer( index, cmdBuffer, camera );
    
    // Call inherited for recursion of children
    CRenderNode::recordCommandBuffer( index, cmdBuffer, camera );
}

/***************************************************************************
*    DESC:  Destroy the physics
****************************************************************************/
void CSpriteNode::destroyPhysics()
{
    CSprite::destroyPhysics();
    
    // Call inherited for recursion of children
    CRenderNode::destroyPhysics();
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite * CSpriteNode::getSprite()
{
    return static_cast<CSprite *>(this);
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObject * CSpriteNode::getObject()
{
    return static_cast<CObject *>(this);
}

/***************************************************************************
*    DESC:  Get the radius. Need to add in the scale of the object
****************************************************************************/
float CSpriteNode::getRadius()
{
    if(m_headNode)
        return m_radius * getSprite()->getScale().x;

    return getSprite()->getObjectData().getRadius() * getSprite()->getScale().x;
}

/***************************************************************************
*    DESC:  Get the size
****************************************************************************/
CSize<float> CSpriteNode::getSize()
{
    if(m_headNode)
        return m_size * getSprite()->getScale();

    return m_size;
}

/***************************************************************************
*    DESC:  Calculate the total size based on all the children
****************************************************************************/
void CSpriteNode::calcSize( iNode * pNode, CSize<float> & size )
{
    if( pNode != nullptr )
    {
        iNode * pNextNode;
        auto nodeIter = pNode->getNodeIter();

        do
        {
            // get the next node
            pNextNode = pNode->next(nodeIter);

            if( pNextNode != nullptr )
            {
                auto _size = pNextNode->getSize();

                if( _size.w > size.w )
                    size.w = _size.w;

                if( _size.h > size.h )
                    size.h = _size.h;

                // Call a recursive function again
                calcSize( pNextNode, size );
            }
        }
        while( pNextNode != nullptr );
    }
}