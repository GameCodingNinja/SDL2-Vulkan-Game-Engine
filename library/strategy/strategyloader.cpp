
/************************************************************************
*    FILE NAME:       strategyloader.cpp
*
*    DESCRIPTION:     Class for loading strategies
************************************************************************/

// Physical component dependency
#include <strategy/strategyloader.h>

// Game lib dependencies
#include <strategy/strategymanager.h>
#include <strategy/actorstrategy.h>
#include <strategy/stagestrategy.h>
#include <node/inode.h>
#include <sprite/sprite.h>
#include <common/defs.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/genfunc.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

namespace NStrategyloader
{
    void init( const XMLNode & nodeXML, CObject2D * pObject );
    void init( const XMLNode & nodeXML, CSprite * pSprite );

    /************************************************************************
     *    DESC:  Load the XML that describes how to load the strategy
     ************************************************************************/
    void load( const std::string & filePath )
    {
        // Open and parse the XML file:
        const XMLNode xmlNode = XMLNode::openFileHelper( filePath.c_str(), "strategyLst" );

        if( !xmlNode.isEmpty() )
        {
            for( int stratigy = 0; stratigy < xmlNode.nChildNode(); ++stratigy )
            {
                const XMLNode startegyXML = xmlNode.getChildNode( stratigy );

                if( startegyXML.isAttributeSet("name") )
                {
                    const std::string strategyName = startegyXML.getAttribute( "name" );

                    if( !strategyName.empty() && startegyXML.isAttributeSet("type") )
                    {
                        const std::string strategyType = startegyXML.getAttribute( "type" );

                        if( !strategyType.empty() )
                        {
                            iStrategy * pStrategy(nullptr);
                            if( strategyType == "actor" )
                                pStrategy = CStrategyMgr::Instance().addStrategy( strategyName, new CActorStrategy );

                            else if( strategyType == "stage" )
                                pStrategy = CStrategyMgr::Instance().addStrategy( strategyName, new CStageStrategy );

                            else
                                throw NExcept::CCriticalException("Strategy Loader Error!",
                                    boost::str( boost::format("Unknown strategy type (%s, %s).\n\n%s\nLine: %s")
                                        % strategyName % strategyType % __FUNCTION__ % __LINE__ ));

                            // Apply a camera if one is defined
                            if( startegyXML.isAttributeSet("camera") )
                            {
                                const std::string cameraId = startegyXML.getAttribute("camera");

                                if( !cameraId.empty() )
                                    pStrategy->setCamera( cameraId );
                            }

                            // Create the command buffer if defined
                            if( startegyXML.isAttributeSet("cmdBufPool") )
                            {
                                const std::string cmdBufPool = startegyXML.getAttribute( "cmdBufPool" );

                                if( !cmdBufPool.empty() )
                                {
                                    auto cmdBuf = CDevice::Instance().createSecondaryCommandBuffers( cmdBufPool );
                                    pStrategy->setCommandBuffers( cmdBuf );
                                }
                            }

                            // Load the nodes for the startegy
                            for( int node = 0; node < startegyXML.nChildNode(); ++node )
                            {
                                const XMLNode startegyNodeXML = startegyXML.getChildNode( node );

                                if( startegyNodeXML.isAttributeSet("name") )
                                {
                                    const std::string name = startegyNodeXML.getAttribute( "name" );

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

                                    iNode * pHeadNode = pStrategy->create( name, instanceName, active );

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

                                                    if( pChildNode->getType() == NDefs::ENT_OBJECT )
                                                        init( nodeChildNodeXML, pChildNode->getObject() );

                                                    else if( pChildNode->getType() == NDefs::ENT_SPRITE )
                                                        init( nodeChildNodeXML, pChildNode->getSprite() );
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if( pHeadNode->getType() == NDefs::ENT_OBJECT )
                                                init( childNodeXML, pHeadNode->getObject() );

                                            else if( pHeadNode->getType() == NDefs::ENT_SPRITE )
                                                init( childNodeXML, pHeadNode->getSprite() );
                                        }
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
                    }
                }
            }
        }
    }

    /************************************************************************
     *    DESC:  Init the object with the xmlNode data
     ************************************************************************/
    void init( const XMLNode & nodeXML, CObject2D * pObject )
    {
        // Set any transforms
        pObject->loadTransFromNode( nodeXML );
    }

    void init( const XMLNode & nodeXML, CSprite * pSprite )
    {
        // Set any transforms
        pSprite->getObject()->loadTransFromNode( nodeXML );

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
}
