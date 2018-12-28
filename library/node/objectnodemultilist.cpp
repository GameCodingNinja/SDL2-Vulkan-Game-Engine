
/************************************************************************
*    FILE NAME:       objectnodemultilist.cpp
*
*    DESCRIPTION:     Object node multi link list class
************************************************************************/

// Physical component dependency
#include <node/objectnodemultilist.h>

// Game lib dependencies
#include <2d/object2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectNodeMultiLst::CObjectNodeMultiLst( int objectId, int nodeId, int parentId ) :
    CNodeMultiLst(nodeId, parentId),
    m_objectId(objectId)
{
    m_type = NDefs::ENT_OBJECT_MULTI_LIST;
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
void CObjectNodeMultiLst::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_object.transform( matrix, tranformWorldPos );

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}


/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObject2D * CObjectNodeMultiLst::getObject()
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
