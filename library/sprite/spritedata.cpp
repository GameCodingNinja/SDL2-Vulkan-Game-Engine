
/************************************************************************
*    FILE NAME:       spritedata.cpp
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

// Physical component dependency
#include <sprite/spritedata.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteData::CSpriteData( 
    const XMLNode & xmlNode,
    const std::string & defGroup,
    const std::string & defObjName,
    int defId ) :
        m_xmlNode( xmlNode ),
        m_group(defGroup),
        m_objectName(defObjName),
        m_spriteId(defId)
{
    // Get the name of this specific sprite instance
    if( xmlNode.isAttributeSet( "name" ) )
        m_name = xmlNode.getAttribute( "name" );
    
    // Get the group this sprite belongs to
    if( xmlNode.isAttributeSet( "group" ) )
        m_group = xmlNode.getAttribute( "group" );
    
    // Get the object data name
    if( xmlNode.isAttributeSet( "objectName" ) )
        m_objectName = xmlNode.getAttribute( "objectName" );
    
    // Get the sprite's unique id number
    if( xmlNode.isAttributeSet( "id" ) )
        m_spriteId = std::atoi(xmlNode.getAttribute( "id" ));
}

/************************************************************************
*    DESC:  Constructor for dynamic node data generation for sprite creation
************************************************************************/
CSpriteData::CSpriteData( 
    const std::string & group,
    const std::string & objName ) :
        m_name(objName),
        m_group(group),
        m_objectName(objName),
        m_spriteId(defs_DEFAULT_ID)
{
}

CSpriteData::CSpriteData( const CSpriteData & data ) :
    m_xmlNode( data.m_xmlNode ),
    m_name( data.m_name ),
    m_group( data.m_group ),
    m_objectName( data.m_objectName ),
    m_spriteId( data.m_spriteId )
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CSpriteData::~CSpriteData()
{
}

/************************************************************************
*    DESC:  Get the XML Node
************************************************************************/
const XMLNode & CSpriteData::getXMLNode() const
{
    return m_xmlNode;
}


/************************************************************************
*    DESC:  Get the name of this specific sprite instance
************************************************************************/
const std::string & CSpriteData::getSpriteName() const
{
    return m_name;
}


/************************************************************************
*    DESC:  Get the group
************************************************************************/
const std::string & CSpriteData::getGroup() const
{
    return m_group;
}


/************************************************************************
*    DESC:  Get the object name
************************************************************************/
const std::string & CSpriteData::getObjectName() const
{
    return m_objectName;
}


/************************************************************************
*    DESC:  Get the unique id number
************************************************************************/
int CSpriteData::getSpriteId() const
{
    return m_spriteId;
}
