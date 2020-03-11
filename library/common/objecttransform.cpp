/************************************************************************
*    FILE NAME:       objecttransform.cpp
*
*    DESCRIPTION:     object transform class
************************************************************************/

// Physical component dependency
#include <common/objecttransform.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectTransform::CObjectTransform()
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CObjectTransform::~CObjectTransform()
{
}


/************************************************************************
*    DESC:  Get the object's translated position
************************************************************************/
const CPoint<float> & CObjectTransform::getTransPos() const
{
    return m_transPos;
}


/************************************************************************
*    DESC:  Transform the object in local space
************************************************************************/
void CObjectTransform::transformLocal( CMatrix & matrix )
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
void CObjectTransform::transform()
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.isSet( NDefs::TRANSFORM ) )
    {
        transformLocal( m_matrix );
    
        m_transPos = m_pos;
    }
}

void CObjectTransform::transform( const CObjectTransform & object )
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.isSet( NDefs::TRANSFORM ) || object.wasTranformed() )
    {
        CMatrix localMatrix;
    
        transformLocal( localMatrix );
    
        m_matrix = localMatrix * object.getMatrix();

        m_matrix.transform( m_transPos, CPoint<float>() );
    }
}


/************************************************************************
*    DESC:  Apply the scale
************************************************************************/
void CObjectTransform::applyScale( CMatrix & matrix )
{
    matrix.setScale( m_scale );
}


/************************************************************************
*    DESC:  Apply the rotation
************************************************************************/
void CObjectTransform::applyRotation( CMatrix & matrix )
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
const CMatrix & CObjectTransform::getMatrix() const
{
    return m_matrix;
}


/************************************************************************
*    DESC:  Get the object's rotation matrix
*           NOTE: For 2d, it's the same matrix
************************************************************************/
const CMatrix & CObjectTransform::getRotMatrix() const
{
    return m_matrix;
}

CMatrix & CObjectTransform::getRotMatrix()
{
    return m_matrix;
}


/************************************************************************
*    DESC:  Was this object transformed?
************************************************************************/
bool CObjectTransform::wasTranformed() const
{
    return m_parameters.isSet( NDefs::WAS_TRANSFORMED );
}


/************************************************************************
*    DESC:  Force a transform from this point all the way up the line
************************************************************************/
void CObjectTransform::forceTransform()
{
    m_parameters.add( NDefs::TRANSFORM );
}
