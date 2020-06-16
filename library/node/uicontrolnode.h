
/************************************************************************
*    FILE NAME:       uicontrolnode.h
*
*    DESCRIPTION:     UI Control node that allows for children
************************************************************************/

#ifndef __uicontrol_node_h__
#define __uicontrol_node_h__

// Physical component dependency
#include <node/rendernode.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CMatrix;
class CUIControl;
class CNodeData;

class CUIControlNode : public CRenderNode
{
public:

    // Constructor
    CUIControlNode( std::unique_ptr<CUIControl> pControl, const CNodeData & rNodeData );

    // Destructor
    virtual ~CUIControlNode();

    // Update the nodes
    void update() override;

    // Transform the nodes
    void transform() override;

    // Used to transform object on a sector
    void transform( const CObjectTransform & object ) override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Get the control
    CUIControl * getControl() override;

    // Get the object
    CObjectTransform * getObject() override;

protected:

    // Node data
    std::unique_ptr<CUIControl> m_upControl;
};

#endif
