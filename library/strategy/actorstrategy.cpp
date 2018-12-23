
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
#include <managers/signalmanager.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <node/nodefactory.h>
#include <node/nodedatalist.h>
#include <node/nodedata.h>
#include <node/inode.h>
#include <system/device.h>
#include <3d/object3d.h>
#include <common/camera.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CActorStrategy::CActorStrategy()
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
************************************************************************/
iNode * CActorStrategy::create(
    const std::string & dataName,
    const std::string & instanceName )
{
    // Create a unique node id
    const int nodeId( ((m_idInc++) + m_idOffset) * m_idDir );
    
    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [nodeId](const iNode * pNode){ return pNode->getId() == nodeId; });

    // Sanity check that there is no duplicate node id's
    if( iter != m_pNodeVec.end() )
    {
        throw NExcept::CCriticalException("Node Create Error!",
            boost::str( boost::format("Duplicate node id (%s - %d).\n\n%s\nLine: %s")
                % dataName % nodeId % __FUNCTION__ % __LINE__ ));
    }

    const auto & rNodeDataVec = getData( dataName ).getData();

    iNode * pHeadNode(nullptr);

    // Build the node list
    for( auto & iter : rNodeDataVec )
    {
        // Create the node from the factory function
        iNode * pNode = NNodeFactory::Create( iter, nodeId );
        
        if( pHeadNode == nullptr )
            pHeadNode = pNode;
        
        else if( !pHeadNode->addNode( pNode, iter.getNodeName() ) )
            throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Parent node not found or node does not support adding children (%s).\n\n%s\nLine: %s")
                    % dataName % __FUNCTION__ % __LINE__ ));
    }

    // Add the node pointer to the vector for adding to the list
    m_pCreateVec.push_back( pHeadNode );
    
    // If there is an instance name with this node, add it to the map
    if( !instanceName.empty() )
    {
        if( !m_pNodeMap.emplace( instanceName, pHeadNode ).second )
            throw NExcept::CCriticalException("Node create Error!",
            boost::str( boost::format("Duplicate node instance name (%s).\n\n%s\nLine: %s")
                % instanceName % __FUNCTION__ % __LINE__ ));
    }

    return pHeadNode;
}


/************************************************************************
*    DESC:  destroy the node
************************************************************************/
void CActorStrategy::destroy( int id )
{
    m_deleteVec.push_back( id );
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CActorStrategy::update()
{
    for( auto iter : m_pNodeVec )
        iter->update();
    
    // Add new nodes created during the update
    if( !m_pCreateVec.empty() )
    {
        for( auto iter : m_pCreateVec )
        {
            iter->update();
            m_pNodeVec.push_back( iter );
        }
        
        m_pCreateVec.clear();
    }
    
    // Delete nodes destroyed during the update
    if( !m_deleteVec.empty() )
    {
        for( auto id : m_deleteVec )
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
                NGenFunc::PostDebugMsg( boost::str( boost::format("Node id can't be found (%s).\n\n%s\nLine: %s")
                    % id % __FUNCTION__ % __LINE__ ) );
            }
        }
        
        m_deleteVec.clear();
    }
}


/************************************************************************
*    DESC:  Transform the nodes
************************************************************************/
void CActorStrategy::transform()
{
    for( auto iter : m_pNodeVec )
        iter->transform();

    if( m_pCamera )
        m_pCamera->transform();
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CActorStrategy::recordCommandBuffer( uint32_t index, const CCamera & camera )
{
    auto cmdBuf( m_commandBufVec.at(index) );

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
