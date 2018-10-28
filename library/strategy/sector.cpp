
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
#include <managers/signalmanager.h>
#include <common/camera.h>
#include <node/spritenodemultilist.h>
#include <node/spritenode.h>
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
    const XMLNode sectorNode = XMLNode::openFileHelper( filePath.c_str(), "node" );
    if( !sectorNode.isEmpty() )
    {
        std::string defObjName, defGroup, defAIName, spriteName;
        int defId(-1);

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
            const auto & rNodeDataList = data.getData();
            
            spriteName = rNodeDataList.front().getName();
            
            iNode * pHeadNode = nullptr;
            
            // Build the node list
            for( auto & iter : rNodeDataList )
            {
                if( iter.getNodeType() == NDefs::ENT_SPRITE )
                {
                    auto * pSpriteNode = new CSpriteNode( CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getObjectName() ), iter.getSpriteId() );

                    loadSprite( pSpriteNode->getSprite(), iter );

                    if( pHeadNode == nullptr )
                        pHeadNode = pSpriteNode;
                }
                else if( iter.getNodeType() == NDefs::ENT_SPRITE_MULTI_LIST )
                {
                    auto * pSpriteNode = new CSpriteNodeMultiLst(
                        CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getName() ),
                        iter.getSpriteId(),
                        iter.getNodeId(),
                        iter.getParentNodeId() );

                    loadSprite( pSpriteNode->getSprite(), iter );

                    if( pHeadNode == nullptr )
                        pHeadNode = pSpriteNode;

                    else if( !pHeadNode->addNode( pSpriteNode ) )
                        throw NExcept::CCriticalException("Node Create Error!",
                            boost::str( boost::format("Parent node not found when adding child node (%s).\n\n%s\nLine: %s")
                                % iter.getSpriteId() % __FUNCTION__ % __LINE__ ));
                }
                else
                {
                    throw NExcept::CCriticalException("Node Create Error!",
                        boost::str( boost::format("Node type not defined (%s).\n\n%s\nLine: %s")
                            % iter.getSpriteId() % __FUNCTION__ % __LINE__ ));
                }
            }

            // Add the node pointer to the vector for rendering
            m_pNodeVec.push_back( pHeadNode );
            
            // If there is a name, add it to the map
            if( !spriteName.empty() )
                m_pNodeMap.emplace( spriteName, pHeadNode );
        }
    }
    
    // open and parse the XML file:
    /*const std::string filePath = node.getAttribute( "file" );
    const XMLNode spriteListNode = XMLNode::openFileHelper( filePath.c_str(), "spriteList" );
    if( !spriteListNode.isEmpty() )
    {
        std::string defObjName, defGroup, defAIName, spriteName;
        int defId(-1);

        // Check for any defaults
        if( spriteListNode.isAttributeSet( "defaultGroup" ) )
            defGroup = spriteListNode.getAttribute( "defaultGroup" );

        if( spriteListNode.isAttributeSet( "defaultObjectName" ) )
            defObjName = spriteListNode.getAttribute( "defaultObjectName" );

        if( spriteListNode.isAttributeSet( "defaultAIName" ) )
            defAIName = spriteListNode.getAttribute( "defaultAIName" );

        if( spriteListNode.isAttributeSet( "defaultId" ) )
            defId = std::atoi(spriteListNode.getAttribute( "defaultId" ));

        // Create the sprites
        for( int i = 0; i < spriteListNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteListNode.getChildNode( i );

            const std::string tag( spriteNode.getName() );

            std::string aiName;

            // Allocate the sprite and add it to the vector
            if( tag == "sprite2d" )
            {
                // Allocate the sprite
                CSpriteData data( spriteNode, defGroup, defObjName, defAIName, defId );
                m_pSpriteVec.push_back( new CSprite( CObjectDataMgr::Instance().getData2D( data ), data.getSpriteId() ) );

                // Load the rest from sprite data
                m_pSpriteVec.back()->load( data );

                aiName = data.getAIName();
                spriteName = data.getName();
            }

            // If there is a name, add it to the map
            if( !spriteName.empty() )
                m_pSpriteMap.emplace( spriteName, m_pSpriteVec.back() );

            // Init the physics
            m_pSpriteVec.back()->initPhysics();

            // Broadcast the signal to create the sprite AI
            if( !aiName.empty() )
                CSignalMgr::Instance().broadcast( aiName, m_pSpriteVec.back() );
        }
    }*/
}


/***************************************************************************
*    DESC:  Load the node
****************************************************************************/
void CSector::loadSprite( CSprite * sprite, const CSpriteData & rSpriteData )
{
    // Load the rest from sprite data
    sprite->load( rSpriteData );

    // Init the physics
    sprite->initPhysics();

    // Init the sprite
    sprite->init();

    // Broadcast the signal to create the sprite AI
    if( !rSpriteData.getAIName().empty() )
        CSignalMgr::Instance().broadcast( rSpriteData.getAIName(), sprite );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CSector::init()
{
    // Create any font strings
    // This allows for delayed VBO create so that the fonts can be allocated during the load screen
    for( auto iter : m_pNodeVec )
        iter->init();
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
        iter->transform( getMatrix(), wasWorldPosTranformed() );
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CSector::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    if( inView() )
    {
        for( auto iter : m_pNodeVec )
            iter->recordCommandBuffer( index, cmdBuffer, viewProj );
    }
}

void CSector::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj )
{
    if( inView() )
    {
        for( auto iter : m_pNodeVec )
            iter->recordCommandBuffer( index, cmdBuffer, rotMatrix, viewProj );
    }
}


/************************************************************************
*    DESC:  Render the actor
************************************************************************/
void CSector::render( const CCamera & camera )
{
    // Render in reverse order
    if( inView() )
    {
        //for( auto it = m_pSpriteVec.rbegin(); it != m_pSpriteVec.rend(); ++it )
        //    (*it)->render( camera.getFinalMatrix(), camera.getRotMatrix() );
    }
}

void CSector::render( const CMatrix & matrix )
{
    // Render in reverse order
    if( inView() )
    {
        //for( auto it = m_pSpriteVec.rbegin(); it != m_pSpriteVec.rend(); ++it )
        //    (*it)->render( matrix );
    }
}

void CSector::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    // Render in reverse order
    if( inView() )
    {
        //for( auto it = m_pSpriteVec.rbegin(); it != m_pSpriteVec.rend(); ++it )
        //    (*it)->render( matrix, rotMatrix );
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
 *    DESC:  Find if the sprite exists
 ************************************************************************/
/*bool CSector::find( CSprite * pSprite )
{
    for( auto iter : m_pSpriteVec )
    {
        if( iter == pSprite )
            return true;
    }

    return false;
}*/


/************************************************************************
*    DESC:  Get the pointer to the sprite
************************************************************************/
/*CSprite * CSector::get( const std::string & spriteName )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pSpriteMap.find( spriteName );
    if( mapIter == m_pSpriteMap.end() )
        throw NExcept::CCriticalException("Sector Sprite Get Error!",
            boost::str( boost::format("Sector Sprite can't be found (%s).\n\n%s\nLine: %s")
                % spriteName % __FUNCTION__ % __LINE__ ));

    return mapIter->second;
}*/
