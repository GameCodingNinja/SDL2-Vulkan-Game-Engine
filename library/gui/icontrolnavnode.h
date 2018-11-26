
/************************************************************************
*    FILE NAME:       icontrolnavnode.h
*
*    DESCRIPTION:     iControl Navigation Node
*                     This class does NOT own any of then pointers
************************************************************************/

#ifndef __icontrol_nav_node_h__
#define __icontrol_nav_node_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class iControl;

class iControlNavNode : public boost::noncopyable
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
    iControlNavNode( iControl * pControl )
        : m_piControl( pControl ),
          m_pUpNode(nullptr),
          m_pDownNode(nullptr),
          m_pLeftNode(nullptr),
          m_pRightNode(nullptr)
    {}

    // Get UI Control
    iControl * getControl()
    { return m_piControl; }

    // Set/Get Right Node
    void setNode( ENavNode navNode, iControlNavNode * pNode )
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

    iControlNavNode * getNode( ENavNode navNode )
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
    iControl * m_piControl;

    // Navigation node pointers
    iControlNavNode * m_pUpNode;
    iControlNavNode * m_pDownNode;
    iControlNavNode * m_pLeftNode;
    iControlNavNode * m_pRightNode;
};

#endif
