
/************************************************************************
*    FILE NAME:       objectnode.cpp
*
*    DESCRIPTION:     Object node that allows for children
************************************************************************/

// Physical component dependency
#include <node/objectnode.h>

// Game lib dependencies
#include <node/nodedata.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CObjectNode::CObjectNode( const CNodeData & rNodeData ) :
    CRenderNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
    m_radius(0.f)
{
    m_userId = rNodeData.getUserId();
    m_type = ENodeType::OBJECT;

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    // Load the transforms from XML node
    CObject::loadTransFromNode( rNodeData.getXMLNode() );

    // Load the script functions
    CObject::loadScriptFromNode( rNodeData.getXMLNode(), rNodeData.getGroup() );

    // Prepare any script functions that are flagged to prepareOnInit
    CObject::prepareOnInit();
}

CObjectNode::~CObjectNode()
{}

/***************************************************************************
*    DESC:  Only called after node creation with all it's children
****************************************************************************/
void CObjectNode::init()
{
    iNode::init();

    calcSize(this, m_size);

    // Calculate the radius
    m_radius = sqrt( pow((float)m_size.w / 2, 2) + pow((float)m_size.h / 2, 2) );
}

/***************************************************************************
*    DESC:  Transform the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CObjectNode::transform()
{
    CObject::transform();

    // Call inherited for recursion of children
    CRenderNode::transform();
}

void CObjectNode::transform( const CObject & object )
{
    CObject::transform( object );

    // Call inherited for recursion of children
    CRenderNode::transform();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObject * CObjectNode::getObject()
{
    return static_cast<CObject *>(this);
}

/***************************************************************************
*    DESC:  Get the radius. Need to add in the scale of the object
****************************************************************************/
float CObjectNode::getRadius()
{
    if(m_headNode)
        return m_radius * getObject()->getScale().x;

    return 0.f;
}

/***************************************************************************
*    DESC:  Get the size
****************************************************************************/
CSize<float> CObjectNode::getSize()
{
    if(m_headNode)
        return m_size * getObject()->getScale();

    return m_size;
}

/***************************************************************************
*    DESC:  Calculate the total size based on all the children
****************************************************************************/
void CObjectNode::calcSize( iNode * pNode, CSize<float> & size )
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