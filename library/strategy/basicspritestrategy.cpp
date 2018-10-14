
/************************************************************************
*    FILE NAME:       basicspritestrategy.cpp
*
*    DESCRIPTION:     Basic sprite strategy 2d class
************************************************************************/

// Physical component dependency
#include <strategy/basicspritestrategy.h>

// Game lib dependencies
#include <common/spritedata.h>
#include <sprite/sprite.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>
#include <managers/cameramanager.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>

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
    for( auto & iter : m_spriteMap )
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

            const std::string tag( spriteNode.getName() );

            bool duplicate(false);

            // Load the sprite data into the map
            if( tag == "sprite2d" )
                duplicate = !m_dataMap.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(new CSpriteData(spriteNode, defGroup, defObjName, defAIName), NDefs::EOT_SPRITE2D) ).second;

            else if( tag == "sprite3d" )
                duplicate = !m_dataMap.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(new CSpriteData(spriteNode, defGroup, defObjName, defAIName), NDefs::EOT_SPRITE3D) ).second;

            /*else if( tag == "actor2d" )
                duplicate = !m_dataMap.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(new CActorData(spriteNode, defGroup, defObjName, defAIName), NDefs::EOT_OBJECT_NODE) ).second;*/

            else
                throw NExcept::CCriticalException("Sprite Load Error!",
                    boost::str( boost::format("Undefined sprite tag (%s).\n\n%s\nLine: %s")
                        % name % __FUNCTION__ % __LINE__ ));

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
CSpriteDataContainer & CBasicSpriteStrategy::getData( const std::string & name )
{
    auto iter = m_dataMap.find( name );
    if( iter == m_dataMap.end() )
        throw NExcept::CCriticalException("Sprite Data Container Error!",
            boost::str( boost::format("Error finding sprite data container (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));

    return iter->second;
}


/************************************************************************
*    DESC:  create the sprite sprite
*           NOTE: Function assumes radians
************************************************************************/
CSprite * CBasicSpriteStrategy::create(
    const std::string & dataName,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    std::string aiName;
    const CSpriteDataContainer & rSpriteDataContainer = getData( dataName );

    // If the sprite defined a unique id then use that
    int spriteId( ((m_spriteInc++) + m_idOffset) * m_idDir );

    std::pair<std::map<const int, CSprite *>::iterator, bool> iter;

    // Create the sprite
    if( rSpriteDataContainer.getType() == NDefs::EOT_SPRITE2D )
    {
        const auto & rData = rSpriteDataContainer.get<CSpriteData>();
        if( rData.getId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.getId();

        // Allocate the sprite
        iter = m_spriteMap.emplace( spriteId, new CSprite( CObjectDataMgr::Instance().getData2D( rData ), spriteId ) );

        // Load the rest from sprite data
        iter.first->second->load( rData );

        aiName = rData.getAIName();
    }
    /*else if( rSpriteDataContainer.getType() == NDefs::EOT_OBJECT_NODE )
    {
        const auto & rData = rSpriteDataContainer.get<CActorData>();
        if( rData.getId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.getId();

        // Allocate the actor sprite
        iter = m_spriteMap.emplace( spriteId, new CActorSprite2D( rData, spriteId ) );

        aiName = rData.getAIName();
    }*/

    // Check for duplicate id's
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % dataName % spriteId % __FUNCTION__ % __LINE__ ));
    }

    // Use passed in transforms if specified
    if( !pos.isEmpty() )
        iter.first->second->getObject()->setPos(pos);

    if( !rot.isEmpty() )
        iter.first->second->getObject()->setRot(rot, false);

    if( scale != CPoint<float>(1,1,1) )
        iter.first->second->getObject()->setScale(scale);

    // Init the physics
    iter.first->second->initPhysics();
    
    // Init the sprite
    iter.first->second->init();

    // Broadcast the signal to create the sprite AI
    if( !aiName.empty() )
        CSignalMgr::Instance().broadcast( aiName, iter.first->second );

    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( iter.first->second );

    return iter.first->second;
}

CSprite * CBasicSpriteStrategy::create(
    const std::string & dataName )
{
    std::string aiName;
    const CSpriteDataContainer & rSpriteDataContainer = getData( dataName );

    // If the sprite defined a unique id then use that
    int spriteId( ((m_spriteInc++) + m_idOffset) * m_idDir );

    std::pair<std::map<const int, CSprite *>::iterator, bool> iter;

    // Create the sprite
    if( rSpriteDataContainer.getType() == NDefs::EOT_SPRITE2D )
    {
        const auto & rData = rSpriteDataContainer.get<CSpriteData>();
        if( rData.getId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.getId();

        // Allocate the sprite
        iter = m_spriteMap.emplace( spriteId, new CSprite( CObjectDataMgr::Instance().getData2D( rData ), spriteId ) );

        // Load the rest from sprite data
        iter.first->second->load( rData );

        aiName = rData.getAIName();
    }
    /*else if( rSpriteDataContainer.getType() == NDefs::EOT_OBJECT_NODE )
    {
        const auto & rData = rSpriteDataContainer.get<CActorData>();
        if( rData.getId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.getId();

        // Allocate the actor sprite
        iter = m_spriteMap.emplace( spriteId, new CActorSprite2D( rData, spriteId ) );

        aiName = rData.getAIName();
    }*/

    // Init the physics
    iter.first->second->initPhysics();
    
    // Init the sprite
    iter.first->second->init();

    // Check for duplicate id's
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % dataName % spriteId % __FUNCTION__ % __LINE__ ));
    }

    // Broadcast the signal to create the sprite AI
    if( !aiName.empty() )
        CSignalMgr::Instance().broadcast( aiName, iter.first->second );

    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( iter.first->second );

    return iter.first->second;
}

CSprite * CBasicSpriteStrategy::create(
    const std::string & group,
    const std::string & name,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // If the sprite defined a unique id then use that
    int spriteId( ((m_spriteInc++) + m_idOffset) * m_idDir );

    std::pair<std::map<const int, CSprite *>::iterator, bool> iter;

    // Allocate the sprite
    if( CObjectDataMgr::Instance().isData2D( group, name ) )
    {
        auto & objData = CObjectDataMgr::Instance().getData2D( group, name );
        iter = m_spriteMap.emplace( spriteId, new CSprite( objData, spriteId ) );
    }
    /*else if( CObjectDataMgr::Instance().isData3D( group, name ) )
    {
        auto & objData = CObjectDataMgr::Instance().getData3D( group, name );
        iter = m_spriteMap.emplace( spriteId, new CSprite3D( objData, spriteId ) );
    }*/

    // Check for duplicate id's
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % name % spriteId % __FUNCTION__ % __LINE__ ));
    }

    // Use passed in transforms if specified
    if( !pos.isEmpty() )
        iter.first->second->getObject()->setPos(pos);

    if( !rot.isEmpty() )
        iter.first->second->getObject()->setRot(rot, false);

    if( scale != CPoint<float>(1,1,1) )
        iter.first->second->getObject()->setScale(scale);

    // Init the physics
    iter.first->second->initPhysics();
    
    // Init the sprite
    iter.first->second->init();

    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( iter.first->second );

    return iter.first->second;
}

CSprite * CBasicSpriteStrategy::create(
    const std::string & group,
    const std::string & name )
{
    // If the sprite defined a unique id then use that
    int spriteId( ((m_spriteInc++) + m_idOffset) * m_idDir );

    std::pair<std::map<const int, CSprite *>::iterator, bool> iter;

    // Allocate the sprite
    if( CObjectDataMgr::Instance().isData2D( group, name ) )
    {
        auto & objData = CObjectDataMgr::Instance().getData2D( group, name );
        iter = m_spriteMap.emplace( spriteId, new CSprite( objData, spriteId ) );
    }
    /*else if( CObjectDataMgr::Instance().isData3D( group, name ) )
    {
        auto & objData = CObjectDataMgr::Instance().getData3D( group, name );
        iter = m_spriteMap.emplace( spriteId, new CSprite3D( objData, spriteId ) );
    }*/

    // Check for duplicate id's
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % name % spriteId % __FUNCTION__ % __LINE__ ));
    }

    // Init the physics
    iter.first->second->initPhysics();
    
    // Init the sprite
    iter.first->second->init();

    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( iter.first->second );

    return iter.first->second;
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
    const auto iter = m_spriteMap.find( index );
    if( iter != m_spriteMap.end() )
    {
        auto spriteIter = std::find( m_pSpriteVec.begin(), m_pSpriteVec.end(), iter->second );
        if( spriteIter != m_pSpriteVec.end() )
            m_pSpriteVec.erase( spriteIter );

        NDelFunc::Delete( iter->second );
        m_spriteMap.erase( iter );
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
    for( auto iter : m_pSpriteVec )
    {
        iter->update();
        iter->physicsUpdate();
    }
}


/************************************************************************
*    DESC:  Transform the sprites
************************************************************************/
void CBasicSpriteStrategy::transform()
{
    for( auto iter : m_pSpriteVec )
        iter->getObject()->transform();
}


/***************************************************************************
*    DESC:  Render the sprites
****************************************************************************/
void CBasicSpriteStrategy::render( const CMatrix & matrix )
{
    //for( auto iter : m_pSpriteVec )
    //    iter->render( matrix );
}

void CBasicSpriteStrategy::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    //for( auto iter : m_pSpriteVec )
    //    iter->render( matrix, rotMatrix );
}

void CBasicSpriteStrategy::render()
{
    //const auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    //for( auto iter : m_pSpriteVec )
    //    iter->render( camera );
}


/************************************************************************
*    DESC:  Get the pointer to the sprite
************************************************************************/
CSprite * CBasicSpriteStrategy::getSprite( const int id )
{
    // See if this sprite has already been created
    auto iter = m_spriteMap.find( id );
    if( iter == m_spriteMap.end() )
        throw NExcept::CCriticalException("Sprite Request Error!",
            boost::str( boost::format("Requested sprite has not been created! (%d).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;
}


/************************************************************************
 *    DESC:  Find if the sprite exists
 ************************************************************************/
bool CBasicSpriteStrategy::find( CSprite * pSprite )
{
    // See if this sprite has already been created
    auto iter = m_spriteMap.find( pSprite->getId() );
    if( iter != m_spriteMap.end() && (iter->second == pSprite) )
        return true;

    return false;
}
