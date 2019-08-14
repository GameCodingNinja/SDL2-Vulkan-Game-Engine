
/************************************************************************
*    FILE NAME:       sector.cpp
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

// Physical component dependency
#include <strategy/sector.h>

// Game lib dependencies
#include <sprite/sprite.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <common/camera.h>
#include <node/nodefactory.h>
#include <node/nodedatalist.h>
#include <node/nodedata.h>
#include <node/inode.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSector::CSector() :
    m_projectionType(CSettings::Instance().getProjectionType()),
    m_sectorSizeHalf(CSettings::Instance().getSectorSizeHalf())
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSector::~CSector()
{
    NDelFunc::DeleteVectorPointers( m_pNodeVec );
}


/************************************************************************
*    DESC:  Load the sector data from node
************************************************************************/
void CSector::loadFromNode( const XMLNode & node )
{
    // open and parse the XML file:
    const std::string filePath = node.getAttribute( "file" );
    const XMLNode sectorNode = XMLNode::openFileHelper( filePath.c_str(), "sector" );
    if( !sectorNode.isEmpty() )
    {
        std::string defObjName, defGroup, defAIName, nodeName;
        int defId(defs_DEFAULT_ID);

        // Check for any defaults
        if( sectorNode.isAttributeSet( "defaultGroup" ) )
            defGroup = sectorNode.getAttribute( "defaultGroup" );

        if( sectorNode.isAttributeSet( "defaultObjectName" ) )
            defObjName = sectorNode.getAttribute( "defaultObjectName" );

        if( sectorNode.isAttributeSet( "defaultAIName" ) )
            defAIName = sectorNode.getAttribute( "defaultAIName" );

        if( sectorNode.isAttributeSet( "defaultId" ) )
            defId = std::atoi(sectorNode.getAttribute( "defaultId" ));
        
        // Create the nodes
        for( int i = 0; i < sectorNode.nChildNode(); ++i )
        {
            const XMLNode childNode = sectorNode.getChildNode( i );
            
            CNodeDataList data( childNode, defGroup, defObjName, defAIName, defId );
            const auto & rNodeDataVec = data.getData();
            
            // See if the head nodes has a name
            nodeName = rNodeDataVec.front().getNodeName();
            
            iNode * pHeadNode = nullptr;
            
            // Build the node list
            for( auto & iter : rNodeDataVec )
            {
                // Create the node from the factory function
                iNode * pNode = NNodeFactory::Create( iter, iter.getSpriteId() );
                
                if( pHeadNode == nullptr )
                    pHeadNode = pNode;
        
                else if( !pHeadNode->addNode( pNode, iter.getNodeName() ) )
                    throw NExcept::CCriticalException("Node Create Error!",
                        boost::str( boost::format("Parent node not found or node does not support adding children (%s).\n\n%s\nLine: %s")
                            % nodeName % __FUNCTION__ % __LINE__ ));
            }

            // Add the node pointer to the vector for rendering
            m_pNodeVec.push_back( pHeadNode );
            
            // If there is a node with a name, add it to the map
            if( !nodeName.empty() )
            {
                if( !m_pNodeMap.emplace( nodeName, pHeadNode ).second )
                    throw NExcept::CCriticalException("Node create Error!",
                    boost::str( boost::format("Duplicate node name (%s).\n\n%s\nLine: %s")
                        % nodeName % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}


/***************************************************************************
*    DESC:  Load the node
****************************************************************************/
void CSector::loadSprite( CSprite * sprite, const CSpriteData & rSpriteData )
{
    // Load the rest from sprite data
    sprite->load( rSpriteData.getXMLNode() );

    // Init the physics
    sprite->initPhysics();

    // Init the sprite
    sprite->init();
}


/************************************************************************
*    DESC:  Update the actor
************************************************************************/
void CSector::update()
{
    for( auto iter : m_pNodeVec )
        iter->update();
}


/************************************************************************
*    DESC:  Transform the actor
************************************************************************/
void CSector::transform()
{
    CObject3D::transform();
    
    for( auto iter : m_pNodeVec )
        iter->transform( *this );
}


/***************************************************************************
*    DESC:  Record the command buffer for all the nodes
*           objects that are to be rendered
****************************************************************************/
void CSector::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    if( inView() )
    {
        for( auto iter : m_pNodeVec )
            iter->recordCommandBuffer( index, cmdBuffer, camera );
    }
}


/************************************************************************
*    DESC:  Render the actor
************************************************************************/
bool CSector::inView()
{
    if( m_projectionType == NDefs::EPT_ORTHOGRAPHIC )
        return inOrthographicView();

    else if( m_projectionType == NDefs::EPT_PERSPECTIVE )
        return inPerspectiveView();

    return true;
}


/************************************************************************
 *    DESC:  Check the sector object is within the orthographic view frustum
 ************************************************************************/
bool CSector::inOrthographicView()
{
    const CSize<float> & defaultSizeHalf = CSettings::Instance().getDefaultSizeHalf();

    // Check against the right side of the screen
    if( std::fabs(m_transPos.x) > (defaultSizeHalf.w + m_sectorSizeHalf) )
        return false;

    // Check against the top of the screen
    if( std::fabs(m_transPos.y) > (defaultSizeHalf.h + m_sectorSizeHalf) )
        return false;

    // If we made it this far, the object is in view
    return true;
}


/************************************************************************
 *    DESC:  Check the sector object is within the perspective view frustum
 ************************************************************************/
bool CSector::inPerspectiveView()
{
    const CSize<float> & aspectRatio = CSettings::Instance().getScreenAspectRatio();

    // Check the right and left sides of the screen
    if( std::fabs(m_transPos.x) > ((std::fabs(getPos().z) * aspectRatio.w) + m_sectorSizeHalf) )
        return false;

    // Check the top and bottom sides of the screen
    if( std::fabs(m_transPos.y) > ((std::fabs(getPos().z) * aspectRatio.h) + m_sectorSizeHalf) )
        return false;

    // if we made it this far, the object was not culled
    return true;
}


/************************************************************************
*    DESC:  Get the pointer to the node
************************************************************************/
iNode * CSector::get( const std::string & nodeName )
{
    auto mapIter = m_pNodeMap.find( nodeName );
    if( mapIter == m_pNodeMap.end() )
        throw NExcept::CCriticalException("Sector Sprite Get Error!",
            boost::str( boost::format("Sector Sprite can't be found (%s).\n\n%s\nLine: %s")
                % nodeName % __FUNCTION__ % __LINE__ ));

    return mapIter->second;
}
