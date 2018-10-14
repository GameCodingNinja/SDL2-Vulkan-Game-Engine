
/************************************************************************
*    FILE NAME:       object3d.cpp
*
*    DESCRIPTION:     object class for 3D
************************************************************************/

// Physical component dependency
#include <3d/object3d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObject3D::CObject3D()
{
}

CObject3D::CObject3D( const CObject3D & obj ) :
    CObject2D( obj ),
    m_rotMatrix( obj.m_rotMatrix )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CObject3D::~CObject3D()
{
}


/************************************************************************
*    DESC:  Apply the rotation
************************************************************************/
void CObject3D::applyRotation( CMatrix & matrix )
{
    // Add in the center point prior to rotation
    if( m_parameters.isSet( NDefs::CENTER_POINT ) )
        matrix.translate( m_centerPos );
    
    // Add in the rotation if this is NOT a physics transformation
    if( !m_parameters.isSet( NDefs::PHYSICS_TRANSFORM ) )
    {
        m_rotMatrix.initilizeMatrix();
        m_rotMatrix.rotate( m_rot );
    }
    
    // Since the rotation has already been done, multiply it into the matrix
    matrix.multiply3x3( m_rotMatrix );
    
    // Subtract the center point after rotation to put back in original position
    if( m_parameters.isSet( NDefs::CENTER_POINT ) )
        matrix.translate( -m_centerPos );
}


/************************************************************************
*    DESC:  Get the object's rotation matrix
************************************************************************/
const CMatrix & CObject3D::getRotMatrix() const
{
    return m_rotMatrix;
}


/************************************************************************
*    DESC:  Use a point to set a column
************************************************************************/
void CObject3D::setRotMatrixColumn( const int col, const float x, const float y, const float z )
{
    m_rotMatrix.setColumn( col, x, y, z );
}