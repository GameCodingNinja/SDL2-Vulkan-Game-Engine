
/************************************************************************
*    FILE NAME:       dynamicoffset.h
*
*    DESCRIPTION:     Dynamic Offset class
************************************************************************/  

// Physical component dependency
#include <common/dynamicoffset.h>

// Game lib dependencies
#include <utilities/settings.h>

/************************************************************************
*    DESC:  Add to the bit mask
************************************************************************/ 
void CDynamicOffset::add( int value )
{
    m_parameters.add( value );
}

/************************************************************************
*    DESC:  Set X
************************************************************************/ 
void CDynamicOffset::setX( float value )
{
    point.x = value;
}

/************************************************************************
*    DESC:  Set Y
************************************************************************/ 
void CDynamicOffset::setY( float value )
{
    point.y = value;
}

/************************************************************************
*    DESC:  Is the dynamic offset being used
************************************************************************/ 
bool CDynamicOffset::isEmpty() const
{
    return m_parameters.isEmpty();
}

/************************************************************************
*    DESC:  Get the dynamic position
************************************************************************/
CPoint<float> CDynamicOffset::getPos( CSize<float> defaultHalfSize )
{
    CPoint<float> pos;

    // Strip out any fractional component for correct rendering
    defaultHalfSize.round();

    if( m_parameters.isSet( NOffset::LEFT ) )
        pos.x = -(defaultHalfSize.w - point.x);

    else if( m_parameters.isSet( NOffset::RIGHT ) )
        pos.x = defaultHalfSize.w - point.x;

    else if( m_parameters.isSet( NOffset::HORZ_CENTER ) )
        pos.x = point.x;

    if( m_parameters.isSet( NOffset::TOP ) )
        pos.y = -(defaultHalfSize.h - point.y);

    else if( m_parameters.isSet( NOffset::BOTTOM ) )
        pos.y = defaultHalfSize.h - point.y;

    else if( m_parameters.isSet( NOffset::VERT_CENTER ) )
        pos.y = point.y;

    return pos;
}

CPoint<float> CDynamicOffset::getPos()
{
    return getPos( CSettings::Instance().getDefaultSizeHalf() );
}