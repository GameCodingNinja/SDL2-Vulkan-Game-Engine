/************************************************************************
*    FILE NAME:       strategy.cpp
*
*    DESCRIPTION:     Strategy class
************************************************************************/

// Physical component dependency
#include <strategy/strategy.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>
#include <objectdata/objectdatamanager.h>
#include <node/nodefactory.h>
#include <node/nodedatalist.h>
#include <node/nodedata.h>
#include <node/inode.h>
#include <sprite/sprite.h>
#include <physics/iphysicscomponent.h>
#include <system/device.h>
#include <common/camera.h>
#include <managers/cameramanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStrategy::CStrategy() :
    m_pCamera( &CCameraMgr::Instance().getDefault() ),
    m_extraCamera(nullptr)
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CStrategy::~CStrategy()
{
    clearAllNodes();
    NDelFunc::DeleteVectorPointers( m_clearAllVec );
}

/************************************************************************
 *    DESC:  Clear all nodes
 ************************************************************************/
void CStrategy::clearAllNodes()
{
    for( auto & mapIter : m_pNodeMap )
    {
        auto vecIter = std::find( m_clearAllVec.begin(), m_clearAllVec.end(), mapIter.second );
        if( vecIter == m_clearAllVec.end() )
            m_clearAllVec.push_back( mapIter.second );
    }

    for( auto iter : m_pNodeVec )
    {
        auto vecIter = std::find( m_clearAllVec.begin(), m_clearAllVec.end(), iter );
        if( vecIter == m_clearAllVec.end() )
            m_clearAllVec.push_back( iter );
    }

    for( auto iter : m_pActivateVec )
    {
        auto vecIter = std::find( m_clearAllVec.begin(), m_clearAllVec.end(), iter );
        if( vecIter == m_clearAllVec.end() )
            m_clearAllVec.push_back( iter );
    }

    for( auto iter : m_pDeactivateVec )
    {
        auto vecIter = std::find( m_clearAllVec.begin(), m_clearAllVec.end(), iter );
        if( vecIter == m_clearAllVec.end() )
            m_clearAllVec.push_back( iter );
    }

    // Disable the physics
    for( auto iter : m_clearAllVec )
    {
        auto pSprite = iter->getSprite();
        if( pSprite != nullptr )
        {
            auto pPhysics = pSprite->getPhysicsComponent();
            if( pPhysics != nullptr )
                pPhysics->setContactFilter(0);
        }
    }

    m_pNodeMap.clear();
    m_pNodeVec.clear();
    m_pActivateVec.clear();
    m_pDeactivateVec.clear();
    m_deleteVec.clear();
}

/************************************************************************
 *    DESC:  Clear all nodes
 ************************************************************************/
void CStrategy::clear()
{
    m_clearAllNodesFlag = true;
}

/************************************************************************
 *    DESC:  Load the node data from node
 ************************************************************************/
void CStrategy::loadFromFile( const std::string & file )
{
    // open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( file.c_str(), "strategy" );
    if( !node.isEmpty() )
    {
        std::string defGroup, defObjName, nodeName;
        int defUserId = defs_DEFAULT_ID;
        
        // Check for any defaults
        if( node.isAttributeSet( "defaultGroup" ) )
            defGroup = node.getAttribute( "defaultGroup" );

        if( node.isAttributeSet( "defaultObjectName" ) )
            defObjName = node.getAttribute( "defaultObjectName" );

        if( node.isAttributeSet( "defaultId" ) )
            defUserId = std::atoi(node.getAttribute( "defaultId" ));

        if( node.isAttributeSet( "defaultCamera" ) )
            m_pCamera = &CCameraMgr::Instance().get( node.getAttribute( "defaultCamera" ) );
    
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            const XMLNode nodeLst = node.getChildNode( i );

            if( std::string(nodeLst.getName()) == "node" )
            {
                if( !nodeLst.isAttributeSet( "name" ) )
                {
                    throw NExcept::CCriticalException("Strategy Load Error!",
                        boost::str( boost::format("Strategy node requires a name (%s).\n\n%s\nLine: %s")
                            % file % __FUNCTION__ % __LINE__ ));
                }

                // Get the node list name
                const std::string name = nodeLst.getAttribute( "name" );

                // Load the sprite data into the map
                bool duplicate = !m_dataMap.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(nodeLst, defGroup, defObjName, defUserId) ).second;

                // Check for duplicate names
                if( duplicate )
                {
                    throw NExcept::CCriticalException("Sprite Load Error!",
                        boost::str( boost::format("Duplicate sprite name (%s).\n\n%s\nLine: %s")
                            % name % __FUNCTION__ % __LINE__ ));
                }
            }
            else if( std::string(nodeLst.getName()) == "object" )
            {
                loadTransFromNode( nodeLst );
            }
        }
    }
}

/************************************************************************
 *    DESC:  Get the node data container by name
 ************************************************************************/
CNodeDataList & CStrategy::getData( const std::string & name, const std::string & _group )
{
    // Normal senerio is that the data has been loaded for this strategy
    auto iter = m_dataMap.find( name );

    // If the data can't be found, this could be a simple one-off sprite node 
    // which can be generated from the group and object data name
    if( iter == m_dataMap.end() )
    {
        std::string group = _group;

        // If we can't find the data and the group param is empty, see if we can find the group 
        // in the Object Data Manager as a last attemp. 
        if( group.empty() )
        {
            group = CObjectDataMgr::Instance().findGroup( name );

            if( !group.empty() )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Simple Strategy node sprite auto generated from group search (%s, %s)!") % group % name ) );
        }
        else
        {
            NGenFunc::PostDebugMsg( boost::str( boost::format("Simple Strategy node sprite auto generated from group and object name (%s, %s)!") % group % name ) );
        }

        // If we found group that has an object of the same name, create the data and pass it along
        if( !group.empty() )
        {
            iter = m_dataMap.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(name),
                std::forward_as_tuple(group, name) ).first;
        }
        else
        {
            throw NExcept::CCriticalException("Node Data Error!",
                boost::str( boost::format("Error finding node data (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }
    }

    return iter->second;
}

/************************************************************************
*    DESC:  create the node
************************************************************************/
iNode * CStrategy::create(
    const std::string & dataName,
    const std::string & instanceName,
    bool makeActive,
    const std::string & group )
{
    if( instanceName.empty() && !makeActive )
        throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Need to supply an instance name if node is not active when created (%s).\n\n%s\nLine: %s")
                    % dataName % __FUNCTION__ % __LINE__ ));

    auto & rNodeDataVec = getData( dataName, group ).getData();

    iNode * pHeadNode(nullptr);

    // Build the node list
    for( auto & iter : rNodeDataVec )
    {
        // Create the node from the factory function
        iNode * pNode = NNodeFactory::Create( iter );
        
        if( pHeadNode == nullptr )
            pHeadNode = pNode;
        
        else if( !pHeadNode->addNode( pNode ) )
            throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Parent node not found or node does not support adding children (%s, %d).\n\n%s\nLine: %s")
                    % dataName % pNode->getParentId() % __FUNCTION__ % __LINE__ ));
    }

    // Init the head node
    pHeadNode->init();

    // Add the node pointer to the vector for adding to the list
    if( instanceName.empty() || makeActive )
        m_pActivateVec.push_back( pHeadNode );
    
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
 *    DESC:  activate node
 ************************************************************************/
iNode * CStrategy::activateNode( const std::string & instanceName )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pNodeMap.find( instanceName );
    if( mapIter != m_pNodeMap.end() )
    {
        // See if the node is already in the vector
        auto nodeIter = std::find( m_pNodeVec.begin(), m_pNodeVec.end(), mapIter->second );
        if( nodeIter != m_pNodeVec.end() )
            NGenFunc::PostDebugMsg( boost::str( boost::format("Actor Strategy node is already active (%s)!") % instanceName ) );
        
        else
            // Add the node pointer to the activate vector
            m_pActivateVec.push_back( mapIter->second );
    }
    else
    {
        throw NExcept::CCriticalException("Actor Strategy Node Activate Error!",
            boost::str( boost::format("Node instance name can't be found (%s).\n\n%s\nLine: %s")
                % instanceName % __FUNCTION__ % __LINE__ ));
    }
    
    return mapIter->second;
}

/************************************************************************
 *    DESC:  deactivate node
 ************************************************************************/
void CStrategy::deactivateNode( const std::string & instanceName )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pNodeMap.find( instanceName );
    if( mapIter != m_pNodeMap.end() )
    {
        // See if the node is already in the vector
        auto nodeIter = std::find( m_pNodeVec.begin(), m_pNodeVec.end(), mapIter->second );
        if( nodeIter == m_pNodeVec.end() )
            NGenFunc::PostDebugMsg( boost::str( boost::format("Actor Strategy node is not active (%s)!") % instanceName ) );
        
        else
            // Add the node pointer to the deactivate vector
            m_pDeactivateVec.push_back( mapIter->second );
    }
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Actor Strategy node can't be found to deactivate (%s)!") % instanceName ) );
}

/************************************************************************
*    DESC:  Set the world value position
************************************************************************/  
void CStrategy::setPos( const CPoint<CWorldValue> & position )
{
    m_worldValPos = -position;
    CObject::setPos( m_worldValPos );
}

void CStrategy::setPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    m_worldValPos.set( -x, -y, -z );
    CObject::setPos( m_worldValPos );
}

/************************************************************************
*    DESC:  Increment the world value position
************************************************************************/  
void CStrategy::incPos( const CPoint<CWorldValue> & position )
{
    m_worldValPos.inc( -position.x, -position.y, -position.z );
    CObject::incPos( m_worldValPos );
}

void CStrategy::incPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    m_worldValPos.inc( -x, -y, -z );
    CObject::incPos( m_worldValPos );
}

/************************************************************************
*    DESC:  Increment tha active node vector position of all elements
************************************************************************/  
void CStrategy::incActiveVecPos( const float x, const float y, float z )
{
    for( auto iter : m_pNodeVec )
        iter->getObject()->incPos(x, y, z);
}

/************************************************************************
*    DESC:  destroy the node
************************************************************************/
void CStrategy::destroy( const handle16_t handle )
{
    m_deleteVec.push_back( handle );
}

/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CStrategy::update()
{
    // Remove and deleted nodes from the active list and map
    // Deleting it here allows for one cycle to complete before deleting
    deleteFromActiveList();

    for( auto iter : m_pNodeVec )
        iter->update();
    
    if( m_clearAllNodesFlag )
    {
        m_clearAllNodesFlag = false;
        clearAllNodes();
    }

    // Add nodes to the active list
    addToActiveList();
    
    // Remove nodes from the active list
    removeFromActiveList();
}

/************************************************************************
*    DESC:  Transform the nodes
************************************************************************/
void CStrategy::transform()
{
    CObject::transform();

    for( auto iter : m_pNodeVec )
        iter->transform( *this );
}

/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CStrategy::recordCommandBuffer( uint32_t index )
{
    auto cmdBuf( m_commandBufVec.at(index) );

    CDevice::Instance().beginCommandBuffer( index, cmdBuf );

    m_pCamera->recordCommandBuffer( index, cmdBuf, m_pNodeVec );

    if(m_extraCamera != nullptr)
        m_extraCamera->recordCommandBuffer( index, cmdBuf, m_pNodeVec );
    
    CDevice::Instance().endCommandBuffer( cmdBuf );
}

/***************************************************************************
*    DESC:  Update the secondary command buffer vector
****************************************************************************/
void CStrategy::updateSecondaryCmdBuf( uint32_t index )
{
    CDevice::Instance().updateSecondaryCmdBuf( m_commandBufVec.at(index) );
}

/************************************************************************
*    DESC:  Get the pointer to the node
************************************************************************/
iNode * CStrategy::getNode( const std::string & instanceName )
{
    auto iter = m_pNodeMap.find( instanceName );
    
    if( iter == m_pNodeMap.end() )
        throw NExcept::CCriticalException("Get Node Error!",
            boost::str( boost::format("Node can't be found by instance name (%s).\n\n%s\nLine: %s")
                % instanceName % __FUNCTION__ % __LINE__ ));
    
    return iter->second;
}

/************************************************************************
*    DESC:  Find if the node is active
************************************************************************/
bool CStrategy::isActive( const handle16_t handle )
{
    // See if this node has already been created
    const auto iter = std::find_if(
        m_pNodeVec.begin(),
        m_pNodeVec.end(),
        [handle](const iNode * pNode) { return pNode->getHandle() == handle; } );

    if( iter != m_pNodeVec.end() )
        return true;

    return false;
}

/************************************************************************
*    DESC:  Add created nodes to the active list
************************************************************************/
void CStrategy::addToActiveList()
{
    // Add new nodes created during the update
    if( !m_pActivateVec.empty() )
    {
        for( auto iter : m_pActivateVec )
        {
            iter->update();
            m_pNodeVec.push_back( iter );
        }
        
        m_pActivateVec.clear();
    }
}

/************************************************************************
*    DESC:  Remove deactivated nodes from the active list
************************************************************************/
void CStrategy::removeFromActiveList()
{
    if( !m_pDeactivateVec.empty() )
    {
        for( auto pNode : m_pDeactivateVec )
        {
            auto iter = std::find( m_pNodeVec.begin(), m_pNodeVec.end(), pNode );

            if( iter != m_pNodeVec.end() )
                m_pNodeVec.erase( iter );

            else
                NGenFunc::PostDebugMsg( boost::str( boost::format("Node id can't be found to be deactivated (%s).\n\n%s\nLine: %s")
                    % pNode->getId() % __FUNCTION__ % __LINE__ ) );
        }
        
        m_pDeactivateVec.clear();
    }
}

/************************************************************************
*    DESC:  Remove and deleted nodes from the active list and map
************************************************************************/
void CStrategy::deleteFromActiveList()
{
    // Clear all nodes
    if( !m_clearAllVec.empty() )
        NDelFunc::DeleteVectorPointers( m_clearAllVec );

    if( !m_deleteVec.empty() )
    {
        for( auto handle : m_deleteVec )
        {
            const auto vecIter = std::find_if(
            m_pNodeVec.begin(),
            m_pNodeVec.end(),
            [handle](const iNode * pNode) { return pNode->getHandle() == handle;} );

            if( vecIter != m_pNodeVec.end() )
            {
                NDelFunc::Delete( *vecIter );
                m_pNodeVec.erase( vecIter );
            }
            else
            {
                NGenFunc::PostDebugMsg( boost::str( boost::format("Node id can't be found to delete (%s).\n\n%s\nLine: %s")
                    % handle % __FUNCTION__ % __LINE__ ) );
            }
            
            // If this same node is in the map, delete it here too.
            auto mapIter = m_pNodeMap.begin();
            while( mapIter != m_pNodeMap.end() )
            {
                if( mapIter->second->getHandle() == handle )
                {
                    m_pNodeMap.erase( mapIter );
                    break;
                }
                
                mapIter++;
            }
        }
        
        m_deleteVec.clear();
    }
}

/************************************************************************
 *    DESC:  Set the command buffers
 ************************************************************************/
void CStrategy::setCommandBuffers( std::vector<VkCommandBuffer> & commandBufVec )
{
    m_commandBufVec = commandBufVec;
}

/************************************************************************
*    DESC:  Set/Get the camera
************************************************************************/
void CStrategy::setCamera( const std::string & cameraId )
{
    m_pCamera = &CCameraMgr::Instance().get( cameraId );
}

CCamera & CStrategy::getCamera()
{
    return *m_pCamera;
}


/************************************************************************
*    DESC:  Set the extra camera
************************************************************************/
void CStrategy::setExtraCamera( CCamera * pCamera )
{
    m_extraCamera = pCamera;
}
