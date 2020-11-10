
/************************************************************************
*    FILE NAME:       icontrol.cpp
*
*    DESCRIPTION:     Control Interface class
************************************************************************/

// Physical component dependency
#include <gui/icontrol.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/xmlpreloader.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
iControl::iControl( const std::string & group ) :
    m_group(group),
    m_type(EControlType::_NULL),
    m_alpha(0.f)
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
iControl::~iControl()
{
}

/************************************************************************
*    DESC:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void iControl::loadFromNode( const XMLNode & node )
{
    // Set the controls name
    if( node.isAttributeSet( "name" ) )
        m_name = node.getAttribute( "name" );

    // Set the faction name
    if( node.isAttributeSet( "faction" ) )
        m_faction = node.getAttribute( "faction" );

    // Load the transform data
    loadTransFromNode( node );

    // Load the dynamic offset from node
    loadDynamicOffsetFromNode( node );

    // See if we have a list of strings
    XMLNode stringLstNode = node.getChildNode( "fontStringList" );
    if( !stringLstNode.isEmpty() )
    {
        for( int i = 0; i < stringLstNode.nChildNode(); ++i )
            m_stringVec.push_back( stringLstNode.getChildNode( "string", i ).getAttribute( "text" ) );
    }

    // Load the control specific xml file
    // Get the file path node to the control specific xml code
    XMLNode filePathNode = node.getChildNode( "filePath" );
    if( !filePathNode.isEmpty() )
    {
        // Get the control's file path
        const std::string controlFilePath = filePathNode.getAttribute( "file" );

        // Load xml specific control code
        // Use the preloaded since many controls reuse xml files
        loadControlFromNode( CXMLPreloader::Instance().load( controlFilePath.c_str(), "UIControl" ) );
    }
}

/************************************************************************
*    DESC:  Load the dynamic offset data from node
************************************************************************/
void iControl::loadDynamicOffsetFromNode( const XMLNode & node )
{
    // Load the dynamic offset
    bool dummy;
    m_dynamicOffset = NParseHelper::LoadDynamicOffset( node, dummy );

    // Set the dynamic position
    setDynamicPos();
}

/************************************************************************
*    DESC:  Set the dynamic position
************************************************************************/
void iControl::setDynamicPos()
{
    // Position the menu based on the dynamic offset
    if( !m_dynamicOffset.isEmpty() )
        setPos( m_dynamicOffset.getPos() );
}

/************************************************************************
*    DESC:  Get the object group name
************************************************************************/
const std::string & iControl::getGroup() const
{
    return m_group;
}

/************************************************************************
*    DESC:  Get the control name
************************************************************************/
const std::string & iControl::getName() const
{
    return m_name;
}

/************************************************************************
*    DESC:  Get the control type
************************************************************************/
EControlType iControl::getType() const
{
    return m_type;
}

/************************************************************************
*    DESC:  Get the faction name
************************************************************************/
const std::string & iControl::getFaction() const
{
    return m_faction;
}

/************************************************************************
*    DESC:  Get the alpha value of this control
************************************************************************/
float iControl::getAlpha() const
{
    return m_alpha;
}
