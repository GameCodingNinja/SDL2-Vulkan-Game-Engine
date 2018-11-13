
/************************************************************************
*    FILE NAME:       visual.cpp
*
*    DESCRIPTION:     CVisual Class
************************************************************************/

// Physical component dependency
#include <common/visual.h>

CColor CVisual::m_additive;

/************************************************************************
*    DESC:  Set/Get the additive color
************************************************************************/
void CVisual::setAdditiveColor( const CColor & color )
{
    m_additive = color;
}

void CVisual::setAdditiveColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_additive.set( r, g, b, a );
}

const CColor & CVisual::getAdditiveColor()
{
    return m_additive;
}
