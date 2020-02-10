
/************************************************************************
*    FILE NAME:       objectnodemultilist.cpp
*
*    DESCRIPTION:     Object node multi link list class
************************************************************************/

// Physical component dependency
#include <node/objectnodemultilist.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectNodeMultiLst::CObjectNodeMultiLst( int objectId, int nodeId, int parentId ) :
    CNodeMultiLst(nodeId, parentId),
    m_objectId(objectId)
{
    m_type = NDefs::ENT_OBJECT;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CObjectNodeMultiLst::~CObjectNodeMultiLst()
{
}


/***************************************************************************
*    DESC:  Transform the nodes
****************************************************************************/
void CObjectNodeMultiLst::transform()
{
    m_object.transform();

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

// Used to transform object on a sector
void CObjectNodeMultiLst::transform( const CObjectTransform & object )
{
    m_object.transform( object );

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}


/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CObjectNodeMultiLst::getObject()
{
    return &m_object;
}


/************************************************************************
*    DESC:  Get the sprite id number
************************************************************************/
int CObjectNodeMultiLst::getId() const
{
    return m_objectId;
}
