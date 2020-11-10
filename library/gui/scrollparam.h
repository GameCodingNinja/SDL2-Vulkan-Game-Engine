
/************************************************************************
*    FILE NAME:       scrollparam.h
*
*    DESCRIPTION:     Class for handling scroll parameter data
************************************************************************/

#pragma once

// Standard lib dependencies
#include <map>

// Forward declaration(s)
struct XMLNode;

class CScrollParam
{
public:

    // Constructor
    CScrollParam();
    
    // Load the scroll data from node
    void loadFromNode( const XMLNode & node );

    // Does this menu or control support scrolling this message?
    bool canScroll( int msg );

    // Get/Set Start delay
    int getStartDelay();

    // Get/Set scroll delay
    int getScrollDelay();

    // Get the message
    int getMsg();

private:

    // Map that holds the scroll messages allows by this menu or control
    std::map<int, int> m_scrollTypesMap;

    // The delay of the first scroll message
    int m_startDelay;

    // The delay of the rest of the scroll messages
    int m_scrollDelay;

    // The scroll message to send from the timer
    int m_msg;
};
