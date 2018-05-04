
/************************************************************************
*    FILE NAME:       uicontrolnavnode.h
*
*    DESCRIPTION:     UI Control Navigation Node
*                     This class does NOT own any of then pointers
************************************************************************/

#ifndef __uicontrol_nav_node_h__
#define __uicontrol_nav_node_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CUIControl;

class CUIControlNavNode : public boost::noncopyable
{
public:

    enum ENavNode
    {
        ENAV_NODE_UP = 0,
        ENAV_NODE_DOWN,
        ENAV_NODE_LEFT,
        ENAV_NODE_RIGHT
    };

    // Constructor
    CUIControlNavNode( CUIControl * pControl )
        : m_pUIControl( pControl ),
          m_pUpNode(nullptr),
          m_pDownNode(nullptr),
          m_pLeftNode(nullptr),
          m_pRightNode(nullptr)
    {}

    // Get UI Control
    CUIControl * getControl()
    { return m_pUIControl; }

    // Set/Get Right Node
    void setNode( ENavNode navNode, CUIControlNavNode * pNode )
    {
        if( navNode == ENAV_NODE_UP )
            m_pUpNode = pNode;
        else if( navNode == ENAV_NODE_DOWN )
            m_pDownNode = pNode;
        else if( navNode == ENAV_NODE_LEFT )
            m_pLeftNode = pNode;
        else if( navNode == ENAV_NODE_RIGHT )
            m_pRightNode = pNode;
    }

    CUIControlNavNode * getNode( ENavNode navNode )
    {
        if( navNode == ENAV_NODE_UP )
            return m_pUpNode;
        else if( navNode == ENAV_NODE_DOWN )
            return m_pDownNode;
        else if( navNode == ENAV_NODE_LEFT )
            return m_pLeftNode;
        else if( navNode == ENAV_NODE_RIGHT )
            return m_pRightNode;

        return nullptr;
    }

private:

    // UI Control pointer
    CUIControl * m_pUIControl;

    // Navigation node pointers
    CUIControlNavNode * m_pUpNode;
    CUIControlNavNode * m_pDownNode;
    CUIControlNavNode * m_pLeftNode;
    CUIControlNavNode * m_pRightNode;

};

#endif  // __uicontrol_nav_node_h__


