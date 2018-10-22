
/************************************************************************
*    FILE NAME:       nodestrategy.cpp
*
*    DESCRIPTION:     Node strategy 2d class
************************************************************************/

// Physical component dependency
#include <strategy/nodestrategy.h>

// Game lib dependencies
#include <sprite/sprite.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>
#include <managers/cameramanager.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <node/spritenodemultilist.h>
#include <node/nodedatalist.h>
#include <node/nodedata.h>
#include <node/inode.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CNodeStrategy::CNodeStrategy()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CNodeStrategy::~CNodeStrategy()
{
    NDelFunc::DeleteVectorPointers( m_pNodeVec );
}


/************************************************************************
 *    DESC:  Load the node data from node
 ************************************************************************/
void CNodeStrategy::loadFromFile( const std::string & file )
{
    // open and parse the XML file:
    XMLNode node = XMLNode::openFileHelper( file.c_str(), "nodeStrategy" );
    if( !node.isEmpty() )
    {
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            const XMLNode nodeLst = node.getChildNode( i );

            // Get the node list id
            const std::string id = nodeLst.getAttribute( "id" );

            // Load the sprite data into the map
            bool duplicate = !m_dataMap.emplace( id, nodeLst ).second;

            // Check for duplicate names
            if( duplicate )
            {
                throw NExcept::CCriticalException("Sprite Load Error!",
                    boost::str( boost::format("Duplicate sprite name (%s).\n\n%s\nLine: %s")
                        % id % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}


/************************************************************************
 *    DESC:  Get the node data container by name
 ************************************************************************/
CNodeDataList & CNodeStrategy::getData( const std::string & name )
{
    auto iter = m_dataMap.find( name );
    if( iter == m_dataMap.end() )
        throw NExcept::CCriticalException("Node Data Error!",
            boost::str( boost::format("Error finding node data (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));

    return iter->second;
}


/************************************************************************
*    DESC:  create the node
*           NOTE: Function assumes radians
************************************************************************/
iNode * CNodeStrategy::create(
    const std::string & dataName,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // If the node defined a unique id then use that
    const int nodeId( ((m_idInc++) + m_idOffset) * m_idDir );

    const auto & rNodeDataList = getData( dataName ).getData();

    iNode * pHeadNode = nullptr;

    // Build the node list
    for( auto & iter : rNodeDataList )
    {
        if( iter.getNodeType() == NDefs::ENT_SPRITE )
        {
            
        }
        else if( iter.getNodeType() == NDefs::ENT_SPRITE_MULTI_LIST )
        {
            if( pHeadNode == nullptr )
            {
                auto * pSpriteNode = new CSpriteHeadNodeMultiLst( nodeId, CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getName() ), iter.getNodeId() );
                pHeadNode = pSpriteNode;
                loadSprite( pSpriteNode->getSprite(), iter, pos, rot, scale );
            }
            else
            {
                auto * pSpriteNode = new CSpriteNodeMultiLst( CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getName() ), iter.getNodeId(), iter.getParentNodeId() );
                pHeadNode->addNode( pSpriteNode );
                loadSprite( pSpriteNode->getSprite(), iter );
            }
        }
        else
        {
            throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Node type not defined (%s).\n\n%s\nLine: %s")
                    % dataName % __FUNCTION__ % __LINE__ ));
        }
    }

    // Add the node pointer to the vector for rendering
    m_pNodeVec.push_back( pHeadNode );

    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [nodeId](const iNode * pNode){ return pNode->getId() == nodeId; });

    if( iter != m_pNodeVec.end() )
    {
        throw NExcept::CCriticalException("Node Create Error!",
            boost::str( boost::format("Duplicate node id (%s - %d).\n\n%s\nLine: %s")
                % dataName % nodeId % __FUNCTION__ % __LINE__ ));
    }

    return pHeadNode;
}

iNode * CNodeStrategy::create(
    const std::string & dataName )
{
    return create( dataName, CPoint<CWorldValue>(), CPoint<float>(), CPoint<float>(1,1,1) );
}


/***************************************************************************
*    DESC:  Load the node
****************************************************************************/
void CNodeStrategy::loadSprite(
    CSprite & sprite,
    const CSpriteData & rSpriteData,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // Load the rest from sprite data
    sprite.load( rSpriteData );

    // Use passed in transforms if specified
    if( !pos.isEmpty() )
        sprite.getObject()->setPos(pos);

    if( !rot.isEmpty() )
        sprite.getObject()->setRot(rot, false);

    if( scale != CPoint<float>(1,1,1) )
        sprite.getObject()->setScale(scale);

    // Init the physics
    sprite.initPhysics();

    // Init the sprite
    sprite.init();

    // Broadcast the signal to create the sprite AI
    if( !rSpriteData.getAIName().empty() )
        CSignalMgr::Instance().broadcast( rSpriteData.getAIName(), &sprite );
}


/***************************************************************************
*    DESC:  Handle the deleting of any nodes
****************************************************************************/
void CNodeStrategy::createObj( const std::string & name )
{
    create( name, CPoint<float>(), CPoint<float>(), CPoint<float>(1,1,1) );
}


/***************************************************************************
*    DESC:  Handle the deleting of any nodes
*           NOTE: Do not call from a destructor!
****************************************************************************/
void CNodeStrategy::deleteObj( int index )
{
    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [index](const iNode * pNode) { return pNode->getId() == index;} );

    if( iter != m_pNodeVec.end() )
    {
        NDelFunc::Delete( *iter );
        m_pNodeVec.erase( iter );
    }
    else
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Node index can't be found (%s).\n\n%s\nLine: %s")
            % index % __FUNCTION__ % __LINE__ ) );
    }
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CNodeStrategy::update()
{
    for( auto iter : m_pNodeVec )
        iter->update();
}


/************************************************************************
*    DESC:  Transform the nodes
************************************************************************/
void CNodeStrategy::transform()
{
    for( auto iter : m_pNodeVec )
        iter->transform();
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CNodeStrategy::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    for( auto iter : m_pNodeVec )
        iter->recordCommandBuffer( index, cmdBuffer, viewProj );
}


/************************************************************************
*    DESC:  Get the pointer to the node
************************************************************************/
/*CSprite * CBasicSpriteStrategy::getSprite( const int id )
{
    // See if this node has already been created
    auto iter = m_pNodeMap.find( id );
    if( iter == m_pNodeMap.end() )
        throw NExcept::CCriticalException("Sprite Request Error!",
            boost::str( boost::format("Requested node has not been created! (%d).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;
}*/


/************************************************************************
 *    DESC:  Find if the node exists
 ************************************************************************/
/*bool CBasicSpriteStrategy::find( CSprite * pSprite )
{
    // See if this node has already been created
    auto iter = m_pNodeMap.find( pSprite->getId() );
    if( iter != m_pNodeMap.end() && (iter->second == pSprite) )
        return true;

    return false;
}*/
