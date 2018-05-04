
/************************************************************************
*    FILE NAME:       isprite2d.cpp
*
*    DESCRIPTION:     iSprite2D Class
************************************************************************/

// Physical component dependency
#include <2d/isprite2d.h>

/************************************************************************
*    desc:  Is this a 2D sprite?
************************************************************************/  
bool iSprite2D::IsSprite2D()
{
    return m_parameters.IsSet( NDefs::SPRITE2D );
            
}   // IsSprite2D


/************************************************************************
*    desc:  Is this a 2D actor?
************************************************************************/  
bool iSprite2D::IsActor2D()
{
    return m_parameters.IsSet( NDefs::ACTOR2D );
            
}   // IsActor2D
