
//
//  FILE NAME:       strategyloader.cpp
//
//  DESCRIPTION:     Class for loading strategies
//

// Physical component dependency
#include <strategy/strategyloader.h>

// Game lib dependencies
#include <strategy/strategymanager.h>
#include <strategy/strategy.h>
#include <node/inode.h>
#include <sprite/sprite.h>
#include <common/defs.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/genfunc.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

//
//  DESC: Constructor
//
CStrategyloader::CStrategyloader()
{
}

//
//  DESC: Destructor
//
CStrategyloader::~CStrategyloader()
{
}

//
//  DESC: Load a list of groups
//
void CStrategyloader::loadGroupLst( const std::vector<std::string> & groupVec )
{
    for( auto & iter : groupVec )
        loadGroup( iter );
}

//
//  DESC: Load a group
//
void CStrategyloader::loadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Strategy Loader Group Data Error!",
            boost::str( boost::format("Strategy group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the strategy
    for( auto & iter : listTableIter->second )
        load( iter );
}

//
//  DESC: Load the XML that describes how to load the strategy
//
void CStrategyloader::load( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode xmlNode = XMLNode::openFileHelper( filePath.c_str(), "loader" );

    if( !xmlNode.isEmpty() )
    {
        for( int stratigy = 0; stratigy < xmlNode.nChildNode(); ++stratigy )
        {
            const XMLNode startegyXML = xmlNode.getChildNode( stratigy );

            if( startegyXML.isAttributeSet("name") )
            {
                const std::string strategyName = startegyXML.getAttribute( "name" );

                if( !strategyName.empty() )
                {
                    auto * pStrategy = CStrategyMgr::Instance().addStrategy( strategyName, new CStrategy );

                    // See if a default name has been specified
                    std::string defaultName;
                    if( startegyXML.isAttributeSet("defaultName") )
                        defaultName = startegyXML.getAttribute( "defaultName" );

                    // See if a default group has been specified
                    std::string defaultGroup;
                    if( startegyXML.isAttributeSet("defaultGroup") )
                        defaultGroup = startegyXML.getAttribute( "defaultGroup" );

                    // Apply a camera if one is defined
                    if( startegyXML.isAttributeSet("camera") )
                    {
                        const std::string cameraId = startegyXML.getAttribute("camera");

                        if( !cameraId.empty() )
                            pStrategy->setCamera( cameraId );
                    }

                    // Create the command pool
                    std::string cmdBufPoolName;
                    if( startegyXML.isAttributeSet("cmdBufPool") )
                        cmdBufPoolName = startegyXML.getAttribute( "cmdBufPool" );

                    if( cmdBufPoolName.empty() )
                        cmdBufPoolName = strategyName;
                    
                    auto cmdBuf = CDevice::Instance().createSecondaryCommandBuffers( cmdBufPoolName );
                    pStrategy->setCommandBuffers( cmdBuf );
                    
                    // Load the nodes for the startegy
                    for( int node = 0; node < startegyXML.nChildNode(); ++node )
                    {
                        const XMLNode startegyNodeXML = startegyXML.getChildNode( node );

                        // See if a name has been specified
                        std::string name = defaultName;
                        if( startegyNodeXML.isAttributeSet("name") )
                            name = startegyNodeXML.getAttribute( "name" );

                        if( name.empty() )
                        {
                            throw NExcept::CCriticalException("Strategy Load Error!",
                                boost::str( boost::format("Name or default name is not defined (%s).\n\n%s\nLine: %s")
                                    % filePath % __FUNCTION__ % __LINE__ ));
                        }

                        // See if a group has been specified
                        std::string group = defaultGroup;
                        if( startegyNodeXML.isAttributeSet("group") )
                            group = startegyNodeXML.getAttribute( "group" );

                        // See if there is an instance name associated with this node
                        // Nodes are active by default but can be loaded disabled if it has an instance name
                        std::string instanceName;
                        bool active(true);
                        if( startegyNodeXML.isAttributeSet("instance") )
                        {
                            instanceName = startegyNodeXML.getAttribute( "instance" );

                            if( startegyNodeXML.isAttributeSet("active") )
                                active = ( std::strcmp( startegyNodeXML.getAttribute("active"), "true" ) == 0 );
                        }

                        iNode * pHeadNode = pStrategy->create( name, instanceName, active, group );

                        // Check for any child nodes. Could be object, sprite or node
                        for( int childNode = 0; childNode < startegyNodeXML.nChildNode(); ++childNode )
                        {
                            const XMLNode childNodeXML = startegyNodeXML.getChildNode( childNode );

                            // Check if we are looking for a child node
                            if( std::strcmp( childNodeXML.getName(), "node" ) == 0 )
                            {
                                if( childNodeXML.isAttributeSet("name") )
                                {
                                    const std::string childName = childNodeXML.getAttribute( "name" );

                                    iNode * pChildNode = pHeadNode->getChildNode( childName );
                                    if( pChildNode )
                                    {
                                        const XMLNode nodeChildNodeXML = childNodeXML.getChildNode();

                                        if( pChildNode->getType() == ENodeType::OBJECT || 
                                            pChildNode->getType() == ENodeType::UI_CONTROL )
                                            init( nodeChildNodeXML, pChildNode->getObject() );

                                        else if( pChildNode->getType() == ENodeType::SPRITE )
                                            init( nodeChildNodeXML, pChildNode->getSprite() );
                                    }
                                }
                            }
                            else
                            {
                                if( pHeadNode->getType() == ENodeType::OBJECT ||
                                    pHeadNode->getType() == ENodeType::UI_CONTROL )
                                    init( childNodeXML, pHeadNode->getObject() );

                                else if( pHeadNode->getType() == ENodeType::SPRITE )
                                    init( childNodeXML, pHeadNode->getSprite() );
                            }
                        }
                    }

                    // Activate the strategy if defined
                    if( startegyXML.isAttributeSet("activate") )
                    {
                        if( std::strcmp( startegyXML.getAttribute("activate"), "true" ) == 0 )
                            CStrategyMgr::Instance().activateStrategy( strategyName );
                    }
                }
                else
                {
                    NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy name is empty (%s)!") % filePath ) );
                }
            }
            else
            {
                NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy name is not defined (%s)!") % filePath ) );
            }
        }
    }
}

//
//  DESC: Init the object with the xmlNode data
//
void CStrategyloader::init( const XMLNode & nodeXML, CObject * pObject )
{
    // Set any transforms
    pObject->loadTransFromNode( nodeXML );

    // See if there are any scripts that need to be prepared
    for( int i = 0; i < nodeXML.nChildNode("script"); ++i )
    {
        const XMLNode scriptNodeXML = nodeXML.getChildNode( "script", i );

        if( scriptNodeXML.isAttributeSet("prepare") )
        {
            const std::string scriptToPrepare = scriptNodeXML.getAttribute( "prepare" );
            if( !pObject->prepare( scriptToPrepare ) )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Script function not found in sprite (%s).") % scriptToPrepare ) );
        }
    }
}

void CStrategyloader::init( const XMLNode & nodeXML, CSprite * pSprite )
{
    // Set any transforms
    pSprite->reload( nodeXML );

    // See if there are any scripts that need to be prepared
    for( int i = 0; i < nodeXML.nChildNode("script"); ++i )
    {
        const XMLNode scriptNodeXML = nodeXML.getChildNode( "script", i );

        if( scriptNodeXML.isAttributeSet("prepare") )
        {
            const std::string scriptToPrepare = scriptNodeXML.getAttribute( "prepare" );
            if( !pSprite->prepare( scriptToPrepare ) )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Script function not found in sprite (%s).") % scriptToPrepare ) );
        }
    }
}
