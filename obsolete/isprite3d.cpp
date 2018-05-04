
/************************************************************************
*    FILE NAME:       isprite3d.cpp
*
*    DESCRIPTION:     iSprite3D Class
************************************************************************/

// Physical component dependency
#include <3d/isprite3d.h>

/************************************************************************
*    desc:  Is this a 3D sprite?
************************************************************************/  
bool iSprite3D::IsSprite3D()
{
    return m_parameters.IsSet( NDefs::SPRITE3D );
            
}   // IsSprite3D


/************************************************************************
*    desc:  Is this a 3D actor?
************************************************************************/  
bool iSprite3D::IsActor3D()
{
    return m_parameters.IsSet( NDefs::ACTOR3D );
            
}   // IsActor3D
