
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
    const std::string & nodeName,
    const std::string & defGroup,
    const std::string & defObjName ) :
        m_xmlNode( xmlNode ),
        m_group(defGroup),
        m_objectName(defObjName)
{
    // Get the group this sprite belongs to
    if( xmlNode.isAttributeSet( "group" ) )
        m_group = xmlNode.getAttribute( "group" );
    
    // Get the object data name
    // Init with the node name in the event the node and the object data name are the same
    m_objectName = nodeName;
    if( xmlNode.isAttributeSet( "objectName" ) )
        m_objectName = xmlNode.getAttribute( "objectName" );
}

/************************************************************************
*    DESC:  Constructor for dynamic node data generation for sprite creation
************************************************************************/
CSpriteData::CSpriteData( 
    const std::string & group,
    const std::string & objName ) :
        m_group(group),
        m_objectName(objName)
{
}

CSpriteData::CSpriteData( const CSpriteData & data ) :
    m_xmlNode( data.m_xmlNode ),
    m_group( data.m_group ),
    m_objectName( data.m_objectName )
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
