/************************************************************************
*    FILE NAME:       nodefactory.cpp
*
*    DESCRIPTION:     Class factory for node creation
************************************************************************/

// Physical component dependency
#include <node/nodefactory.h>

// Game lib dependencies
#include <sprite/sprite.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <node/spriteleafnode.h>
#include <node/objectnode.h>
#include <node/uicontrolleafnode.h>
#include <node/spritenode.h>
#include <node/uicontrolnode.h>
#include <node/nodedata.h>
#include <node/inode.h>
#include <gui/uimeter.h>
#include <gui/uiprogressbar.h>

// Boost lib dependencies
#include <boost/format.hpp>

namespace NNodeFactory
{
    // Declare the function prototypes
    iNode * CreateUIControlNode( const CNodeData & rNodeData );
    
    /************************************************************************
    *    DESC:  Create the node from the node data list
    ************************************************************************/
    iNode * Create( const CNodeData & rNodeData )
    {
        iNode * pNode(nullptr);

        // Single node sprite that doesn't support children. Low overhead for when you only need one sprite with no children
        if( rNodeData.getNodeType() == ENodeType::SPRITE )
        {
            if( rNodeData.hasChildrenNodes() )
                pNode = new CSpriteNode( rNodeData );
            else
                pNode = new CSpriteLeafNode( rNodeData );
        }
        else if( rNodeData.getNodeType() == ENodeType::OBJECT )
        {
            pNode = new CObjectNode( rNodeData );
        }
        else if( rNodeData.getNodeType() == ENodeType::UI_CONTROL )
        {
            pNode = CreateUIControlNode( rNodeData );
        }
        else
        {
            throw NExcept::CCriticalException("Node Create Error!",
                boost::str( boost::format("Node type not defined (%s).\n\n%s\nLine: %s")
                    % rNodeData.getNodeName() % __FUNCTION__ % __LINE__ ));
        }

        return pNode;
    }

    /************************************************************************
    *    DESC:  Create the UI Control node
    ************************************************************************/
    iNode * CreateUIControlNode( const CNodeData & rNodeData )
    {
        std::unique_ptr<CUIControl> upControl;
        iNode * pNode(nullptr);

        if( rNodeData.getControlType() == EControlType::METER )
        {
            upControl.reset( new CUIMeter( rNodeData.getGroup() ) );
        }
        else if( rNodeData.getControlType() == EControlType::PROGRESS_BAR )
        {
            upControl.reset( new CUIProgressBar( rNodeData.getGroup() ) );
        }
        else
        {
            throw NExcept::CCriticalException("Node UIControl Create Error!",
                boost::str( boost::format("Node UIControl type not defined (%s).\n\n%s\nLine: %s")
                    % rNodeData.getNodeName() % __FUNCTION__ % __LINE__ ));
        }

        if( rNodeData.hasChildrenNodes() )
            pNode = new CUIControlNode( std::move(upControl), rNodeData );
        else
            pNode = new CUIControlLeafNode( std::move(upControl), rNodeData );

        return pNode;
    }
}
