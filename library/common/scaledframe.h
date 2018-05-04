
/************************************************************************
*    FILE NAME:       scaledframe.h
*
*    DESCRIPTION:     Class for holding scaled frame data
************************************************************************/

#ifndef __scaled_frame_h__
#define __scaled_frame_h__

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

#endif  // __scaled_frame_h__


