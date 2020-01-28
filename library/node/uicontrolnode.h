
/************************************************************************
*    FILE NAME:       uicontrolnode.h
*
*    DESCRIPTION:     UI Control node class for rendering a ui control
************************************************************************/

#ifndef __uicontrol_node_h__
#define __uicontrol_node_h__

// Physical component dependency
#include <node/inode.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CMatrix;
class CUIControl;

class CUIControlNode : public iNode
{
public:

    // Constructor
    CUIControlNode(
        std::unique_ptr<CUIControl> pControl,
        int nodeId = defs_DEFAULT_ID,
        int parentId = defs_DEFAULT_ID );

    // Destructor
    virtual ~CUIControlNode();

    // Update the nodes
    void update() override;

    // Transform the nodes
    void transform() override;
    // Used to transform object on a sector
    void transform( const CObject2D & object ) override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Get the control
    CUIControl * getControl() override;

private:

    // Update the nodes recursive function
    void transform( iNode * pNode );

    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj );

protected:

    // Node data
    std::unique_ptr<CUIControl> m_upControl;
};

#endif
