
/************************************************************************
*    FILE NAME:       spritedata.cpp
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

// Physical component dependency
#include <common/spritedata.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <common/fontdata.h>

// Standard lib dependencies
#include <cstring>
#include <cstdlib>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteData::CSpriteData( 
    const XMLNode & node,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName,
    int defId ) :
        m_group(defGroup),
        m_objectName(defObjName),
        m_aiName(defAIName),
        m_id(defId)
{
    // Get the name of this specific sprite instance
    if( node.isAttributeSet( "name" ) )
        m_name = node.getAttribute( "name" );
    
    // Get the group this sprite belongs to
    if( node.isAttributeSet( "group" ) )
        m_group = node.getAttribute( "group" );
    
    // Get the object data name
    if( node.isAttributeSet( "objectName" ) )
        m_objectName = node.getAttribute( "objectName" );
    
    // Get the sprite's AI name
    if( node.isAttributeSet( "aiName" ) )
        m_aiName = node.getAttribute( "aiName" );
    
    // Get the sprite's unique id number
    if( node.isAttributeSet( "id" ) )
        m_id = std::atoi(node.getAttribute( "id" ));
    
    // Get if the sprite is visible
    if( node.isAttributeSet( "visible" ) )
        setVisible( std::strcmp( node.getAttribute("visible"), "true" ) == 0 );
    
    // See if any font data is specified
    const XMLNode fontNode = node.getChildNode("font");
    if( !fontNode.isEmpty() )
    {
        m_upFontData.reset( new CFontData );
        m_upFontData->loadFromNode( node );
    }

    // Load the transform data from node
    loadTransFromNode( node );
    
    // Load any script functions
    loadScriptFunctions( node );
    
}

CSpriteData::CSpriteData( const CSpriteData & data ) :
    CObject( data ),
    m_name( data.m_name ),
    m_group( data.m_group ),
    m_objectName( data.m_objectName ),
    m_aiName( data.m_aiName ),
    m_id( data.m_id )
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CSpriteData::~CSpriteData()
{
}


/************************************************************************
*    DESC:  Get the name of this specific sprite instance
************************************************************************/
const std::string & CSpriteData::getName() const
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
*    DESC:  Get the ai name
************************************************************************/
const std::string & CSpriteData::getAIName() const
{
    return m_aiName;
}


/************************************************************************
*    DESC:  Get the unique id number
************************************************************************/
int CSpriteData::getId() const
{
    return m_id;
}


/************************************************************************
*    DESC:  Get the font data
************************************************************************/
const CFontData * CSpriteData::getFontData() const
{
    return m_upFontData.get();
}


/************************************************************************
*    DESC:  Get the script functions
************************************************************************/
const std::map<std::string, std::string> & CSpriteData::getScriptFunctions() const
{
    return m_scriptFunctionMap;
}


/************************************************************************
*    DESC:  Load the script functions and add them to the map
************************************************************************/
void CSpriteData::loadScriptFunctions( const XMLNode & node )
{
    // Check for scripting - Add an empty string for scripts not defined
    XMLNode scriptLstNode = node.getChildNode( "scriptLst" );
    if( !scriptLstNode.isEmpty() )
    {
        for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
        {
            const XMLNode scriptNode = scriptLstNode.getChildNode(i);
            
            // Only the first attribute is used
            const XMLAttribute attribute = scriptNode.getAttribute(0);
            const std::string attrName = attribute.lpszName;
            const std::string attrValue = attribute.lpszValue;

            // Add the attribute name and value to the map
            if( !attrValue.empty() )
                m_scriptFunctionMap.emplace( attrName, attrValue );
        }
    }
}
