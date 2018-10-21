
/************************************************************************
*    FILE NAME:       spritedata.cpp
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

// Physical component dependency
#include <sprite/spritenode.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteHeadNode::CSpriteHeadNode(
    int nodeId,
    int parentId,
    const CObjectData2D & objectData,
    int spriteId ) :
        CSpriteNode(nodeId, parentId, objectData, spriteId)
{
}

CSpriteHeadNode::CSpriteHeadNode(
    int nodeId,
    int parentId,
    const CObjectData3D & objectData,
    int spriteId ) :
        CSpriteNode(nodeId, parentId, objectData, spriteId)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteHeadNode::~CSpriteHeadNode()
{
    NDelFunc::DeleteVectorPointers( m_allNodesVec );
}


/************************************************************************
*    DESC:  Add a node
************************************************************************/
bool CSpriteHeadNode::addNode( iNode * pNode )
{
    m_allNodesVec.push_back( pNode );

    bool result = CSpriteNode::addNode( pNode );

    resetIterators();

    return result;
}


/************************************************************************
*    DESC:  Reset the iterators
************************************************************************/
void CSpriteHeadNode::resetIterators()
{
    reset();

    for( auto iter : m_allNodesVec )
        iter->reset();
}
