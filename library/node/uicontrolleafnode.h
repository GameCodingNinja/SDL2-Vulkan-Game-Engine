
/************************************************************************
*    FILE NAME:       uicontrolleafnode.h
*
*    DESCRIPTION:     UI Control node class for rendering a UIControl
*                     Node class for handling a UIControl with no children
*                     to keep the overhead low
************************************************************************/

#pragma once

// Physical component dependency
#include <node/inode.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CMatrix;
class CUIControl;
class CNodeData;

class CUIControlLeafNode : public iNode
{
public:

    // Constructor
    CUIControlLeafNode( std::unique_ptr<CUIControl> pControl, const CNodeData & rNodeData );

    // Destructor
    virtual ~CUIControlLeafNode();

    // Update the nodes
    void update() override;

    // Transform the nodes
    void transform() override;
    // Used to transform object on a sector
    void transform( const CObject & object ) override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Get the control
    CUIControl * getControl() override;

    // Get the object
    CObject * getObject() override;

protected:

    // Node data
    std::unique_ptr<CUIControl> m_upControl;
};
