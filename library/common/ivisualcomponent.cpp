
/************************************************************************
*    FILE NAME:       ivisualcomponent.cpp
*
*    DESCRIPTION:     iVisualComponent Class
************************************************************************/

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <objectdata/objectvisualdata2d.h>
#include <objectdata/objectvisualdata3d.h>

// Game lib dependencies
#include <common/defs.h>

// Init some statics
std::string iVisualComponent::m_stringDummy;
CSize<float> iVisualComponent::m_sizeDummy;
CColor iVisualComponent::m_additive;

/************************************************************************
*    desc:  Constructor
************************************************************************/
iVisualComponent::iVisualComponent( const CObjectVisualData2D & visualData ) :
    GENERATION_TYPE( visualData.getGenerationType() ),
    m_quadVertScale( visualData.getVertexScale() ),
    m_color( visualData.getColor() ),
    m_rDefaultColor( visualData.getColor() ),
    m_frameIndex(0)
{
}

iVisualComponent::iVisualComponent( const CObjectVisualData3D & visualData ) :
    GENERATION_TYPE( NDefs::EGT_NULL ),
    m_color( visualData.getColor() ),
    m_rDefaultColor( visualData.getColor() ),
    m_frameIndex(0)
{
}


/************************************************************************
*    DESC:  Set/Get the color
************************************************************************/
void iVisualComponent::setColor( const CColor & color )
{
    m_color = color;
}

void iVisualComponent::setColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_color.set( r, g, b, a );
}

const CColor & iVisualComponent::getColor() const
{
    return m_color;
}


/************************************************************************
*    DESC:  Set/Get the additive color
************************************************************************/
void iVisualComponent::setAdditiveColor( const CColor & color )
{
    m_additive = color;
}

void iVisualComponent::setAdditiveColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_additive.set( r, g, b, a );
}

const CColor & iVisualComponent::getAdditiveColor()
{
    return m_additive;
}


/************************************************************************
*    DESC:  Set/Get the default color
************************************************************************/
void iVisualComponent::setDefaultColor()
{
    m_color = m_rDefaultColor;
}

const CColor & iVisualComponent::getDefaultColor() const
{
    return m_rDefaultColor;
}


/************************************************************************
*    DESC:  Set/Get the alpha
************************************************************************/
void iVisualComponent::setAlpha( float alpha, bool allowToExceed )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;
    
    if( allowToExceed || (alpha < m_rDefaultColor.a) )
        m_color.a = alpha;
    else
        m_color.a = m_rDefaultColor.a;
}

float iVisualComponent::getAlpha() const
{
    return m_color.a;
}


/************************************************************************
*    DESC:  Set/Get the default alpha
************************************************************************/
void iVisualComponent::setDefaultAlpha()
{
    m_color.a = m_rDefaultColor.a;
}

float iVisualComponent::getDefaultAlpha() const
{
    return m_rDefaultColor.a;
}


/************************************************************************
*    DESC:  Set the frame ID from index
************************************************************************/
void iVisualComponent::setFrame( uint index )
{
    m_frameIndex = index;
}

uint iVisualComponent::getCurrentFrame() const
{
    return m_frameIndex;
}


/************************************************************************
*    DESC:  Is this a font sprite
************************************************************************/
bool iVisualComponent::isFontSprite() const
{
    return (GENERATION_TYPE == NDefs::EGT_FONT);
}
