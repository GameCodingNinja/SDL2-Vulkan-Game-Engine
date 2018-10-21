
/************************************************************************
*    FILE NAME:       basicspritestrategy.cpp
*
*    DESCRIPTION:     Basic sprite strategy 2d class
************************************************************************/

// Physical component dependency
#include <strategy/basicspritestrategy.h>

// Game lib dependencies
#include <sprite/spritedata.h>
#include <sprite/sprite.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>
#include <managers/cameramanager.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <sprite/spritenode.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBasicSpriteStrategy::CBasicSpriteStrategy()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CBasicSpriteStrategy::~CBasicSpriteStrategy()
{
    for( auto & iter : m_pNodeMap )
        NDelFunc::Delete( iter.second );
}


/************************************************************************
 *    DESC:  Load the sprite data from node
 ************************************************************************/
void CBasicSpriteStrategy::loadFromFile( const std::string & file )
{
    // open and parse the XML file:
    XMLNode spriteListNode = XMLNode::openFileHelper( file.c_str(), "spriteList" );
    if( !spriteListNode.isEmpty() )
    {
        std::string defGroup, defObjName, defAIName;

        // Check for any defaults
        if( spriteListNode.isAttributeSet( "defaultGroup" ) )
            defGroup = spriteListNode.getAttribute( "defaultGroup" );

        if( spriteListNode.isAttributeSet( "defaultObjectName" ) )
            defObjName = spriteListNode.getAttribute( "defaultObjectName" );

        if( spriteListNode.isAttributeSet( "defaultAIName" ) )
            defAIName = spriteListNode.getAttribute( "defaultAIName" );

        // Load the sprite data
        for( int i = 0; i < spriteListNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteListNode.getChildNode( i );

            // Get the sprite name
            const std::string name = spriteNode.getAttribute( "name" );

            bool duplicate(false);

            // Load the sprite data into the map
            duplicate = !m_dataMap.emplace(
                std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(spriteNode, defGroup, defObjName, defAIName) ).second;

            // Check for duplicate names
            if( duplicate )
            {
                throw NExcept::CCriticalException("Sprite Load Error!",
                    boost::str( boost::format("Duplicate sprite name (%s).\n\n%s\nLine: %s")
                        % name % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}


/************************************************************************
 *    DESC:  Get the sprite data container by name
 ************************************************************************/
CSpriteData & CBasicSpriteStrategy::getData( const std::string & name )
{
    auto iter = m_dataMap.find( name );
    if( iter == m_dataMap.end() )
        throw NExcept::CCriticalException("Sprite Data Container Error!",
            boost::str( boost::format("Error finding sprite data container (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));

    return iter->second;
}


/************************************************************************
*    DESC:  create the node
*           NOTE: Function assumes radians
************************************************************************/
iNode * CBasicSpriteStrategy::create(
    const std::string & dataName,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    const CSpriteData & rSpriteData = getData( dataName );

    // If the sprite defined a unique id then use that
    int spriteId( ((m_spriteInc++) + m_idOffset) * m_idDir );

    // Create the sprite
    if( rSpriteData.getId() != defs_SPRITE_DEFAULT_ID )
        spriteId = rSpriteData.getId();
    
    std::pair<std::map<const int, iNode *>::iterator, bool> iter;

    // Allocate the sprite node
    if( rSpriteData.getType() == NDefs::EOT_SPRITE2D )
    {
        CSpriteNode * pNode = new CSpriteNode(
            rSpriteData.getNodeId(),
            rSpriteData.getParentNodeId(),
            CObjectDataMgr::Instance().getData2D( rSpriteData ),
            spriteId );
    
        iter = m_pNodeMap.emplace( spriteId, pNode );
        // Check for duplicate id's
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Sprite Create Error!",
                boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                    % dataName % spriteId % __FUNCTION__ % __LINE__ ));
        }
    
        loadSprite( pNode->m_sprite, rSpriteData, pos, rot, scale );
    }
    else
        throw NExcept::CCriticalException("Sprite Load Error!",
            boost::str( boost::format("Undefined sprite type (%s).\n\n%s\nLine: %s")
                % dataName % __FUNCTION__ % __LINE__ ));

    // Add the node pointer to the vector for rendering
    m_pNodeVec.push_back( iter.first->second );

    return iter.first->second;
}

iNode * CBasicSpriteStrategy::create(
    const std::string & dataName )
{
    return create( dataName, CPoint<CWorldValue>(), CPoint<float>(), CPoint<float>(1,1,1) );
}


/***************************************************************************
*    DESC:  Load the sprite
****************************************************************************/
void CBasicSpriteStrategy::loadSprite(
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
*    DESC:  Handle the deleting of any sprites
****************************************************************************/
void CBasicSpriteStrategy::createObj( const std::string & name )
{
    create( name, CPoint<float>(), CPoint<float>(), CPoint<float>(1,1,1) );
}


/***************************************************************************
*    DESC:  Handle the deleting of any sprites
*           NOTE: Do not call from a destructor!
****************************************************************************/
void CBasicSpriteStrategy::deleteObj( int index )
{
    // Find the sprite, delete it and remove entry from map
    const auto iter = m_pNodeMap.find( index );
    if( iter != m_pNodeMap.end() )
    {
        auto spriteIter = std::find( m_pNodeVec.begin(), m_pNodeVec.end(), iter->second );
        if( spriteIter != m_pNodeVec.end() )
            m_pNodeVec.erase( spriteIter );

        NDelFunc::Delete( iter->second );
        m_pNodeMap.erase( iter );
    }
    else
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sprite index can't be found (%s).\n\n%s\nLine: %s")
            % index % __FUNCTION__ % __LINE__ ) );
    }
}


/***************************************************************************
*    DESC:  Update the sprites
****************************************************************************/
void CBasicSpriteStrategy::update()
{
    /*for( auto iter : m_pNodeVec )
    {
        iter->update();
        iter->physicsUpdate();
    }*/
}


/************************************************************************
*    DESC:  Transform the sprites
************************************************************************/
void CBasicSpriteStrategy::transform()
{
    //for( auto iter : m_pNodeVec )
    //    iter->getObject()->transform();
}


/***************************************************************************
*    DESC:  Render the sprites
****************************************************************************/
void CBasicSpriteStrategy::render( const CMatrix & matrix )
{
    //for( auto iter : m_pNodeVec )
    //    iter->render( matrix );
}

void CBasicSpriteStrategy::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    //for( auto iter : m_pNodeVec )
    //    iter->render( matrix, rotMatrix );
}

void CBasicSpriteStrategy::render()
{
    //const auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    //for( auto iter : m_pNodeVec )
    //    iter->render( camera );
}


/************************************************************************
*    DESC:  Get the pointer to the sprite
************************************************************************/
/*CSprite * CBasicSpriteStrategy::getSprite( const int id )
{
    // See if this sprite has already been created
    auto iter = m_pNodeMap.find( id );
    if( iter == m_pNodeMap.end() )
        throw NExcept::CCriticalException("Sprite Request Error!",
            boost::str( boost::format("Requested sprite has not been created! (%d).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;
}*/


/************************************************************************
 *    DESC:  Find if the sprite exists
 ************************************************************************/
/*bool CBasicSpriteStrategy::find( CSprite * pSprite )
{
    // See if this sprite has already been created
    auto iter = m_pNodeMap.find( pSprite->getId() );
    if( iter != m_pNodeMap.end() && (iter->second == pSprite) )
        return true;

    return false;
}*/
