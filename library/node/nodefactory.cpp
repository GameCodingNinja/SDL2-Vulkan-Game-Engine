/************************************************************************
*    FILE NAME:       nodefactory.cpp
*
*    DESCRIPTION:     Class factory for node creation
************************************************************************/

// Physical component dependency
#include <node/nodefactory.h>

// Game lib dependencies
#include <sprite/sprite.h>
#include <3d/object3d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <node/spritenodemultilist.h>
#include <node/objectnodemultilist.h>
#include <node/spritenode.h>
#include <node/nodedata.h>
#include <node/inode.h>

// Boost lib dependencies
#include <boost/format.hpp>

namespace NNodeFactory
{
    // Declare the function prototypes
    iNode * Create( const CNodeData & rNodeData, const int nodeId );
    void Load( CSprite * pSprite, const CSpriteData & rSpriteData );
    void Load( CObject2D * pObject, const CSpriteData & rSpriteData );
    
    /************************************************************************
    *    DESC:  Create the node from the node data list
    ************************************************************************/
    iNode * Create( const CNodeData & rNodeData, const int nodeId )
    {
        iNode * pNode(nullptr);

        // Single node sprite that doesn't support children. Low overhead for when you only need one sprite with no children
        if( rNodeData.getNodeType() == NDefs::ENT_SPRITE )
        {
            if( rNodeData.hasChildrenNodes() )
                pNode = new CSpriteNodeMultiLst(
                    CObjectDataMgr::Instance().getData( rNodeData.getGroup(), rNodeData.getObjectName() ),
                    nodeId,
                    rNodeData.getNodeId(),
                    rNodeData.getParentNodeId() );
            else
                pNode = new CSpriteNode(
                    CObjectDataMgr::Instance().getData( rNodeData.getGroup(), rNodeData.getObjectName() ),
                    nodeId,
                    rNodeData.getNodeId(),
                    rNodeData.getParentNodeId() );

            Load( pNode->getSprite(), rNodeData );
        }
        else if( rNodeData.getNodeType() == NDefs::ENT_OBJECT )
        {
            pNode = new CObjectNodeMultiLst( nodeId, rNodeData.getNodeId(), rNodeData.getParentNodeId() );

            Load( pNode->getObject(), rNodeData );
        }
        else
        {
            throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Node type not defined (%s).\n\n%s\nLine: %s")
                    % rNodeData.getNodeName() % __FUNCTION__ % __LINE__ ));
        }

        return pNode;
    }

    /***************************************************************************
    *    DESC:  Load the sprite data
    ****************************************************************************/
    void Load( CSprite * pSprite, const CSpriteData & rSpriteData )
    {
        // Load the rest from sprite data
        pSprite->load( rSpriteData );

        // Init the physics
        pSprite->initPhysics();

        // Init the sprite
        pSprite->init();

        // Broadcast the signal to create the sprite AI
        if( !rSpriteData.getAIName().empty() )
            CSignalMgr::Instance().broadcast( rSpriteData.getAIName(), pSprite );
    }

    /************************************************************************
    *    DESC:  Load the object data
    ************************************************************************/
    void Load( CObject2D * pObject, const CSpriteData & rSpriteData )
    {
        // Load the rest from sprite data
        pObject->copyTransform( &rSpriteData );
    }
}
