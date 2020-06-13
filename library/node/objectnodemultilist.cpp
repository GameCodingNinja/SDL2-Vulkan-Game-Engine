
/************************************************************************
*    FILE NAME:       objectnodemultilist.cpp
*
*    DESCRIPTION:     Object node multi link list class
************************************************************************/

// Physical component dependency
#include <node/objectnodemultilist.h>

// Game lib dependencies
#include <node/nodedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectNodeMultiLst::CObjectNodeMultiLst( const CNodeData & rNodeData ) :
    CNodeMultiLst( rNodeData.getNodeId(), rNodeData.getParentNodeId() )
{
    m_id = rNodeData.getId();
    m_type = NDefs::ENT_OBJECT;

    // Load the transforms from XML node
    CObjectTransform::loadTransFromNode( rNodeData.getXMLNode() );
}

/***************************************************************************
*    DESC:  Transform the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CObjectNodeMultiLst::transform()
{
    CObjectTransform::transform();

    // Call inherited but it has to be last
    CNodeMultiLst::transform();
}

void CObjectNodeMultiLst::transform( const CObjectTransform & object )
{
    CObjectTransform::transform( object );

    // Call inherited but it has to be last
    CNodeMultiLst::transform();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CObjectNodeMultiLst::getObject()
{
    return static_cast<CObjectTransform *>(this);
}
