
/************************************************************************
*    FILE NAME:       icontrolnavnode.h
*
*    DESCRIPTION:     iControl Navigation Node
*                     This class does NOT own any of then pointers
************************************************************************/

#pragma once

// Game lib dependencies
#include <gui/uidefs.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class iControl;

class iControlNavNode : public boost::noncopyable
{
public:

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
    void setNode( EAction action, iControlNavNode * pNode )
    {
        if( action == EAction::UP )
            m_pUpNode = pNode;
        else if( action == EAction::DOWN )
            m_pDownNode = pNode;
        else if( action == EAction::LEFT )
            m_pLeftNode = pNode;
        else if( action == EAction::RIGHT )
            m_pRightNode = pNode;
    }

    iControlNavNode * getNode( EAction action )
    {
        if( action == EAction::UP )
            return m_pUpNode;
        else if( action == EAction::DOWN )
            return m_pDownNode;
        else if( action == EAction::LEFT )
            return m_pLeftNode;
        else if( action == EAction::RIGHT )
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
