/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class
************************************************************************/

// Physical component dependency
#include <2d/object2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObject2D::CObject2D()
{
}

CObject2D::CObject2D( const CObject2D & obj ) :
    CObject( obj ),
    m_transPos( obj.m_transPos ),
    m_matrix( obj.m_matrix )
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CObject2D::~CObject2D()
{
}


/************************************************************************
*    DESC:  Get the object's translated position
************************************************************************/
const CPoint<float> & CObject2D::getTransPos() const
{
    return m_transPos;
}


/************************************************************************
*    DESC:  Transform the object in local space
************************************************************************/
void CObject2D::transformLocal( CMatrix & matrix )
{
    // Reset the matrices
    matrix.initilizeMatrix();

    // Apply the crop offset
    if( m_parameters.isSet( NDefs::CROP_OFFSET ) )
        matrix.translate( m_cropOffset );

    // Apply the scale
    if( m_parameters.isSet( NDefs::SCALE ) )
        applyScale( matrix );

    // Apply the rotation
    if( m_parameters.isSet( NDefs::ROTATE ) )
        applyRotation( matrix );

    // Apply the translation
    if( m_parameters.isSet( NDefs::TRANSLATE ) )
        matrix.translate( m_pos );

    // Clear the check parameter
    m_parameters.remove( NDefs::TRANSFORM | NDefs::PHYSICS_TRANSFORM );

    // Indicate that translation was done
    m_parameters.add( NDefs::WAS_TRANSFORMED );
}


/************************************************************************
*    DESC:  Transform
************************************************************************/
void CObject2D::transform()
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.isSet( NDefs::TRANSFORM ) )
    {
        transformLocal( m_matrix );
    
        m_transPos = m_pos;
    }
}

void CObject2D::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.isSet( NDefs::TRANSFORM ) || tranformWorldPos )
    {
        CMatrix localMatrix;
    
        transformLocal( localMatrix );
    
        m_matrix = localMatrix * matrix;

        m_matrix.transform( m_transPos, CPoint<float>() );
    }
}

void CObject2D::transform( const CObject2D & object )
{
    CObject2D::transform( object.getMatrix(), object.wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Apply the scale
************************************************************************/
void CObject2D::applyScale( CMatrix & matrix )
{
    matrix.setScale( m_scale );
}


/************************************************************************
*    DESC:  Apply the rotation
************************************************************************/
void CObject2D::applyRotation( CMatrix & matrix )
{
    // Add in the center point prior to rotation
    if( m_parameters.isSet( NDefs::CENTER_POINT ) )
        matrix.translate( m_centerPos );

    matrix.rotate( m_rot );

    // Subtract the center point after rotation to put back in original position
    if( m_parameters.isSet( NDefs::CENTER_POINT ) )
        matrix.translate( -m_centerPos );
}


/************************************************************************
*    DESC:  Get the object's matrix
************************************************************************/
const CMatrix & CObject2D::getMatrix() const
{
    return m_matrix;
}


/************************************************************************
*    DESC:  Was the world position transformed?
************************************************************************/
bool CObject2D::wasWorldPosTranformed() const
{
    return m_parameters.isSet( NDefs::WAS_TRANSFORMED );
}


/************************************************************************
*    DESC:  Force a transform from this point all the way up the line
************************************************************************/
void CObject2D::forceTransform()
{
    m_parameters.add( NDefs::TRANSFORM );
}
