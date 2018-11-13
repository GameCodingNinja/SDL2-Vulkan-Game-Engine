
/************************************************************************
*    FILE NAME:       visual.h
*
*    DESCRIPTION:     CVisual Class
************************************************************************/

#ifndef __visual_h__
#define __visual_h__

// Game lib dependencies
#include <common/color.h>

class CVisual
{
public:

    // Set/Get additive color
    static void setAdditiveColor( const CColor & color );
    static void setAdditiveColor( float r, float g, float b, float a );
    static const CColor & getAdditiveColor();
    
protected:
    
    // Additive Color
    static CColor m_additive;
};

#endif
