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
    /***************************************************************************
    *    DESC:  Load the sprite data
    ****************************************************************************/
    void Load(
        CSprite * pSprite,
        const CSpriteData & rSpriteData,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale )
    {
        // Load the rest from sprite data
        pSprite->load( rSpriteData );

        // Use passed in transforms if specified
        if( !pos.isEmpty() )
            pSprite->getObject()->setPos(pos);

        if( !rot.isEmpty() )
            pSprite->getObject()->setRot(rot, false);

        if( scale != CPoint<float>(1,1,1) )
            pSprite->getObject()->setScale(scale);

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
    void Load(
        CObject2D * pObject,
        const CSpriteData & rSpriteData,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale )
    {
        // Load the rest from sprite data
        pObject->copyTransform( &rSpriteData );

        // Use passed in transforms if specified
        if( !pos.isEmpty() )
            pObject->setPos(pos);

        if( !rot.isEmpty() )
            pObject->setRot(rot, false);

        if( scale != CPoint<float>(1,1,1) )
            pObject->setScale(scale);
    }
    
    /************************************************************************
    *    DESC:  Create the node from the node data list
    ************************************************************************/
    iNode * Create(
        const CNodeData & rNodeData,
        const int nodeId,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale )
    {
        iNode * pNode(nullptr);

        // Single node sprite that doesn't support children. Low overhead for when you only need one sprite with no children
        if( rNodeData.getNodeType() == NDefs::ENT_SPRITE )
        {
            if( rNodeData.getSpriteType() == NDefs::EST_SPRITE2D )
                pNode = new CSpriteNode( CObjectDataMgr::Instance().getData2D( rNodeData.getGroup(), rNodeData.getObjectName() ), nodeId );
            
            else if( rNodeData.getSpriteType() == NDefs::EST_SPRITE3D )
                pNode = new CSpriteNode( CObjectDataMgr::Instance().getData3D( rNodeData.getGroup(), rNodeData.getObjectName() ), nodeId );

            Load( pNode->getSprite(), rNodeData, pos, rot, scale );
        }
        else if( rNodeData.getNodeType() == NDefs::ENT_OBJECT_MULTI_LIST )
        {
            pNode = new CObjectNodeMultiLst( nodeId, rNodeData.getNodeId(), rNodeData.getParentNodeId() );

            Load( pNode->getObject(), rNodeData, pos, rot, scale );
        }
        else if( rNodeData.getNodeType() == NDefs::ENT_SPRITE_MULTI_LIST )
        {
            if( rNodeData.getSpriteType() == NDefs::EST_SPRITE2D )
                pNode = new CSpriteNodeMultiLst(
                        CObjectDataMgr::Instance().getData2D( rNodeData.getGroup(), rNodeData.getObjectName() ),
                        nodeId,
                        rNodeData.getNodeId(),
                        rNodeData.getParentNodeId() );
            
            else if( rNodeData.getSpriteType() == NDefs::EST_SPRITE3D )
                pNode = new CSpriteNodeMultiLst(
                        CObjectDataMgr::Instance().getData3D( rNodeData.getGroup(), rNodeData.getObjectName() ),
                        nodeId,
                        rNodeData.getNodeId(),
                        rNodeData.getParentNodeId() );
                
            Load( pNode->getSprite(), rNodeData, pos, rot, scale );
        }
        else
        {
            throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Node type not defined (%s).\n\n%s\nLine: %s")
                    % rNodeData.getNodeName() % __FUNCTION__ % __LINE__ ));
        }

        return pNode;
    }
}
