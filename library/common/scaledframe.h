
/************************************************************************
*    FILE NAME:       scaledframe.h
*
*    DESCRIPTION:     Class for holding scaled frame data
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/size.h>

class CScaledFrame
{
public:
    
    // Size of the frame
    CSize<int> m_frame;

    // Is there a center quad?
    bool m_centerQuad = true;
    
    // Is there a bottom frame?
    bool m_bottomFrame = true;
};
