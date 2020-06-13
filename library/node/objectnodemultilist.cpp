
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
}

/***************************************************************************
*    DESC:  Transform the nodes
****************************************************************************/
void CObjectNodeMultiLst::transform()
{
    CObjectTransform::transform();

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

// Used to transform object on a sector
void CObjectNodeMultiLst::transform( const CObjectTransform & object )
{
    CObjectTransform::transform( object );

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CObjectNodeMultiLst::getObject()
{
    return static_cast<CObjectTransform *>(this);
}
