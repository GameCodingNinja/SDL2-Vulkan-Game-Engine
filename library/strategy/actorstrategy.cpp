
/************************************************************************
*    FILE NAME:       actorstrategy.cpp
*
*    DESCRIPTION:     Actor strategy 2d class
************************************************************************/

// Physical component dependency
#include <strategy/actorstrategy.h>

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
#include <node/spritenode.h>
#include <node/nodedatalist.h>
#include <node/nodedata.h>
#include <node/inode.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CActorStrategy::CActorStrategy()
{
}

CActorStrategy::CActorStrategy( std::vector<VkCommandBuffer> & commandBufVec ) :
    m_commandBufVec(commandBufVec)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CActorStrategy::~CActorStrategy()
{
    NDelFunc::DeleteVectorPointers( m_pNodeVec );
}


/************************************************************************
 *    DESC:  Load the node data from node
 ************************************************************************/
void CActorStrategy::loadFromFile( const std::string & file )
{
    // open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( file.c_str(), "node" );
    if( !node.isEmpty() )
    {
        std::string defGroup, defObjName, defAIName, nodeName;
        
        // Check for any defaults
        if( node.isAttributeSet( "defaultGroup" ) )
            defGroup = node.getAttribute( "defaultGroup" );

        if( node.isAttributeSet( "defaultObjectName" ) )
            defObjName = node.getAttribute( "defaultObjectName" );

        if( node.isAttributeSet( "defaultAIName" ) )
            defAIName = node.getAttribute( "defaultAIName" );
    
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            const XMLNode nodeLst = node.getChildNode( "node", i );

            // Get the node list id
            const std::string id = nodeLst.getAttribute( "name" );

            // Load the sprite data into the map
            bool duplicate = !m_dataMap.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(nodeLst, defGroup, defObjName, defAIName) ).second;

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
CNodeDataList & CActorStrategy::getData( const std::string & name )
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
iNode * CActorStrategy::create(
    const std::string & dataName,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // If the node defined a unique id then use that
    const int nodeId( ((m_idInc++) + m_idOffset) * m_idDir );
    
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

    const auto & rNodeDataList = getData( dataName ).getData();

    iNode * pHeadNode = nullptr;

    // Build the node list
    for( auto & iter : rNodeDataList )
    {
        if( iter.getNodeType() == NDefs::ENT_SPRITE )
        {
            CSpriteNode * pSpriteNode(nullptr);
            
            if( iter.getSpriteType() == NDefs::EST_SPRITE2D )
                pSpriteNode = new CSpriteNode( CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getObjectName() ), nodeId );
            
            else if( iter.getSpriteType() == NDefs::EST_SPRITE3D )
                pSpriteNode = new CSpriteNode( CObjectDataMgr::Instance().getData3D( iter.getGroup(), iter.getObjectName() ), nodeId );

            loadSprite( pSpriteNode->getSprite(), iter, pos, rot, scale );
            
            if( pHeadNode == nullptr )
                pHeadNode = pSpriteNode;
        }
        else if( iter.getNodeType() == NDefs::ENT_SPRITE_MULTI_LIST )
        {
            CSpriteNodeMultiLst * pSpriteNode(nullptr);
            
            if( iter.getSpriteType() == NDefs::EST_SPRITE2D )
                pSpriteNode = new CSpriteNodeMultiLst(
                        CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getObjectName() ),
                        nodeId,
                        iter.getNodeId(),
                        iter.getParentNodeId() );
            
            else if( iter.getSpriteType() == NDefs::EST_SPRITE3D )
                pSpriteNode = new CSpriteNodeMultiLst(
                        CObjectDataMgr::Instance().getData3D( iter.getGroup(), iter.getObjectName() ),
                        nodeId,
                        iter.getNodeId(),
                        iter.getParentNodeId() );
                
            loadSprite( pSpriteNode->getSprite(), iter, pos, rot, scale );
            
            if( pHeadNode == nullptr )
                pHeadNode = pSpriteNode;
            
            else if( !pHeadNode->addNode( pSpriteNode ) )
                throw NExcept::CCriticalException("Node Create Error!",
                    boost::str( boost::format("Parent node not found when adding child node (%s).\n\n%s\nLine: %s")
                        % dataName % __FUNCTION__ % __LINE__ ));
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

    return pHeadNode;
}

iNode * CActorStrategy::create(
    const std::string & dataName )
{
    return create( dataName, CPoint<CWorldValue>(), CPoint<float>(), CPoint<float>(1,1,1) );
}


/***************************************************************************
*    DESC:  Load the node
****************************************************************************/
void CActorStrategy::loadSprite(
    CSprite * sprite,
    const CSpriteData & rSpriteData,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // Load the rest from sprite data
    sprite->load( rSpriteData );

    // Use passed in transforms if specified
    if( !pos.isEmpty() )
        sprite->getObject()->setPos(pos);

    if( !rot.isEmpty() )
        sprite->getObject()->setRot(rot, false);

    if( scale != CPoint<float>(1,1,1) )
        sprite->getObject()->setScale(scale);

    // Init the physics
    sprite->initPhysics();

    // Init the sprite
    sprite->init();

    // Broadcast the signal to create the sprite AI
    if( !rSpriteData.getAIName().empty() )
        CSignalMgr::Instance().broadcast( rSpriteData.getAIName(), sprite );
}


/***************************************************************************
*    DESC:  Handle the deleting of any nodes
****************************************************************************/
void CActorStrategy::createObj( const std::string & name )
{
    create( name, CPoint<float>(), CPoint<float>(), CPoint<float>(1,1,1) );
}


/***************************************************************************
*    DESC:  Handle the deleting of any nodes
*           NOTE: Do not call from a destructor!
****************************************************************************/
void CActorStrategy::deleteObj( int id )
{
    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [id](const iNode * pNode) { return pNode->getId() == id;} );

    if( iter != m_pNodeVec.end() )
    {
        NDelFunc::Delete( *iter );
        m_pNodeVec.erase( iter );
    }
    else
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Node index can't be found (%s).\n\n%s\nLine: %s")
            % id % __FUNCTION__ % __LINE__ ) );
    }
}


/***************************************************************************
*    DESC:  Do any init
****************************************************************************/
void CActorStrategy::init()
{
    for( auto iter : m_pNodeVec )
        iter->init();
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CActorStrategy::update()
{
    if( m_enable )
    {
        for( auto iter : m_pNodeVec )
            iter->update();
    }
}


/************************************************************************
*    DESC:  Transform the nodes
************************************************************************/
void CActorStrategy::transform()
{
    if( m_enable )
    {
        for( auto iter : m_pNodeVec )
            iter->transform();
        
        if( m_pCamera )
            m_pCamera->transform();
    }
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CActorStrategy::recordCommandBuffer( uint32_t index, const CCamera & camera )
{
    if( m_enable )
    {
        auto cmdBuf( m_commandBufVec[index] );

        CDevice::Instance().beginCommandBuffer( index, cmdBuf );

        if( m_pCamera )
        {
            const CCamera & strategyCamera = *m_pCamera;
            
            for( auto iter : m_pNodeVec )
                iter->recordCommandBuffer( index, cmdBuf, strategyCamera );
        }
        else
        {
            for( auto iter : m_pNodeVec )
                iter->recordCommandBuffer( index, cmdBuf, camera );
        }

        CDevice::Instance().endCommandBuffer( cmdBuf );
    }
}


/************************************************************************
*    DESC:  Get the pointer to the node
************************************************************************/
iNode * CActorStrategy::getNode( const int id )
{
    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [id](const iNode * pNode) { return pNode->getId() == id; } );

    if( iter != m_pNodeVec.end() )
        return *iter;

    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Node id can't be found (%s).\n\n%s\nLine: %s")
            % id % __FUNCTION__ % __LINE__ ) );

    return nullptr;
}


/************************************************************************
 *    DESC:  Find if the node is active
 ************************************************************************/
bool CActorStrategy::isActive( const int id )
{
    // See if this node has already been created
    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [id](const iNode * pNode) { return pNode->getId() == id; } );

    if( iter != m_pNodeVec.end() )
        return true;

    return false;
}
