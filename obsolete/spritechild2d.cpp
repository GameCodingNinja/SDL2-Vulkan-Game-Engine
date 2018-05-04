
/************************************************************************
*    FILE NAME:       spritechild2d.cpp
*
*    DESCRIPTION:     Class specific for child sprites where the parent 
*                     does a lot of movement to optimize matrix translations 
*                     for the child and parent.
************************************************************************/

// Physical component dependency
#include <2d/spritechild2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteChild2D::CSpriteChild2D( const CObjectData2D & objectData, int id ) :
    CSprite2D( objectData, id )
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CSpriteChild2D::~CSpriteChild2D()
{
}


/************************************************************************
*    DESC:  Transform
************************************************************************/
void CSpriteChild2D::transform()
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.isSet( NDefs::TRANSFORM ) )
    {
        transformLocal( m_finalMatrix );
    
        m_transPos = m_pos;
    }
}

void CSpriteChild2D::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.isSet( NDefs::TRANSFORM ) )
        transformLocal( m_matrix );
        
    if( m_parameters.isSet( NDefs::WAS_TRANSFORMED ) || tranformWorldPos )
    {
        m_parameters.add( NDefs::WAS_TRANSFORMED );

        m_finalMatrix.copy( m_matrix );

        m_finalMatrix.mergeMatrix( matrix );
    }
}


/************************************************************************
*    DESC:  do the render                                                            
************************************************************************/
void CSpriteChild2D::render( const CMatrix & matrix )
{
    if( isVisible() )
        m_visualComponent.render( m_finalMatrix, matrix );
}
