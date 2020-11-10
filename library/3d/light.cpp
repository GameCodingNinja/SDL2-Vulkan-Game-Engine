
/************************************************************************
*    FILE NAME:       light.cpp
*
*    DESCRIPTION:     light class
************************************************************************/

// Physical component dependency
#include <3d/light.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CLight::CLight( ELightType lightType ) :
    m_type(lightType),
    m_radius(0.f),
    m_enable(false)
{
}

/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CLight::~CLight()
{
}

/************************************************************************
*    DESC:  Enable/disable light
************************************************************************/
void CLight::enable( bool value )
{
    m_enable = value;
}

/************************************************************************
*    DESC:  Set/Get the position / direction
************************************************************************/
void CLight::setPosDir( CPoint<float> & value )
{
    m_posDir = value;
}

const CPoint<float> & CLight::getPosDir() const
{
    return m_posDir;
}

/************************************************************************
*    DESC:  Set/Get the color
************************************************************************/
void CLight::setColor( CPoint<float> & value )
{
    m_color = value;
}

const CPoint<float> & CLight::getColor() const
{
    return m_color;
}
