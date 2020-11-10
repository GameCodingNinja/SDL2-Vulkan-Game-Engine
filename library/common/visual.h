
/************************************************************************
*    FILE NAME:       visual.h
*
*    DESCRIPTION:     CVisual Class
************************************************************************/

#pragma once

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
