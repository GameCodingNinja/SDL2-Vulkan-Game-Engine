
/************************************************************************
*    FILE NAME:       actordata.cpp
*
*    DESCRIPTION:     Actor Data Class
************************************************************************/

// Physical component dependency
#include <common/actordata.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CActorData::CActorData(
    const XMLNode & node,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName,
    int defId ) :
        m_aiName(defAIName),
        m_playerActor(false),
        m_collisionGroup(0),
        m_collisionMask(0),
        m_collisionRadiusScalar(1),
        m_id(defId)
{
    // Get the name of this specific actor sprite instance
    if( node.isAttributeSet( "name" ) )
        m_name = node.getAttribute( "name" );
    
    // Get the actor's AI name
    if( node.isAttributeSet( "aiName" ) )
        m_aiName = node.getAttribute( "aiName" );

    if( node.isAttributeSet( "playerActor" ) )
        m_playerActor = (std::strcmp( node.getAttribute("playerActor"), "true" ) == 0);
    
    // Get the actor's unique id number
    if( node.isAttributeSet( "id" ) )
        m_id = std::atoi(node.getAttribute( "id" ));
    
    // See if we have simple collision setup
    const XMLNode collisionNode = node.getChildNode("collision");
    if( !collisionNode.isEmpty() )
    {
        if( collisionNode.isAttributeSet("group") )
            m_collisionGroup = std::atoi( collisionNode.getAttribute( "group" ) );
        
        if( collisionNode.isAttributeSet("mask") )
            m_collisionMask = std::atoi( collisionNode.getAttribute( "mask" ) );
        
        if( collisionNode.isAttributeSet("radiusScaler") )
            m_collisionRadiusScalar = std::atof( collisionNode.getAttribute( "radiusScaler" ) );
    }
    
    // See if we have a sprite list which we should since it's an actor sprite
    const XMLNode spriteListNode = node.getChildNode("spriteList");
    
    // If there's no sprite data node, throw an exception
    if( spriteListNode.isEmpty() )
        throw NExcept::CCriticalException("Actor Sprite Data Load Error!",
            boost::str( boost::format("Actor Sprite node empty.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    // Reserve the number of vector entries
    m_spriteDataVec.reserve(spriteListNode.nChildNode());

    // Load the sprite data
    for( int i = 0; i < spriteListNode.nChildNode(); ++i )
    {
        XMLNode spriteNode = spriteListNode.getChildNode( i );

        // Allocate the pointer
        m_spriteDataVec.emplace_back( spriteNode, defGroup, defObjName, defAIName );
    }

    // Load the transform data from node
    loadTransFromNode( node );
    
}   // constructor


/************************************************************************
*    DESC:  destructor
************************************************************************/
CActorData::~CActorData()
{
}   // destructor


/************************************************************************
*    DESC:  Get the name of this specific actor sprite instance
************************************************************************/
const std::string & CActorData::getName() const
{
    return m_name;

}   // GetName


/************************************************************************
*    DESC:  Get the ai name
************************************************************************/
const std::string & CActorData::getAIName() const
{
    return m_aiName;
}


/************************************************************************
*    DESC:  Get sprite Data vector
************************************************************************/
const std::vector<CSpriteData> & CActorData::getSpriteData() const
{
    return m_spriteDataVec;
}


/************************************************************************
*    DESC:  Is this actor a player
************************************************************************/
bool CActorData::isPlayerActor() const
{
    return m_playerActor;
}


/************************************************************************
*    DESC:  Get the collision group
************************************************************************/
uint CActorData::getCollisionGroup() const
{
    return m_collisionGroup;
}


/************************************************************************
*    DESC:  Get the collision Mask
************************************************************************/
uint CActorData::getCollisionMask() const
{
    return m_collisionMask;
}


/************************************************************************
*    DESC:  Get the collision Radius Scalar
************************************************************************/
float CActorData::getCollisionRadiusScalar() const
{
    return m_collisionRadiusScalar;
}

/************************************************************************
*    DESC:  Get the unique id number
************************************************************************/
int CActorData::getId() const
{
    return m_id;

}   // GetId
