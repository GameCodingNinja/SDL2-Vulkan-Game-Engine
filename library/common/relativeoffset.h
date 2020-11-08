
/************************************************************************
*    FILE NAME:       relativeoffset.h
*
*    DESCRIPTION:     Relative Offset class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/bitmask.h>

class CRelativeOffset
{
public:

    // And to the bit mask
    void add( int value )
    { m_parameters.Add( value ); }
    
    // Check if bits were set
    bool isSet( int value )
    { return m_parameters.IsSet( value ); }

    // Is the dynamic offset being used
    bool isEmpty() const
    { return !m_parameters.IsEmpty(); }
    
    // Get the position based on relative offset
    CPoint<float> getPos( const CPoint<float> & curPos, CSize<float> defaultHalfSize )
    {
        CPoint<float> pos( curPos );
        
        // Strip out any fractional component for correct rendering
        defaultHalfSize.Round();

        if( m_parameters.IsSet( NOffset::LEFT ) )
            pos.x = -(screenCenter.w - point.x);

        else if( m_parameters.IsSet( NOffset::RIGHT ) )
            pos.x = screenCenter.w - point.x;

        else if( m_parameters.IsSet( NOffset::HORZ_CENTER ) )
            pos.x = point.x;

        if( m_parameters.IsSet( NOffset::TOP ) )
            pos.y = screenCenter.h - point.y;
            
        else if( m_parameters.IsSet( NOffset::BOTTOM ) )
            pos.y = -(screenCenter.h - point.y);

        else if( m_parameters.IsSet( NOffset::VERT_CENTER ) )
            pos.y = point.y;

        return pos;
    }
    
private:

    // bit mask parameters
    CBitmask<uint> m_parameters;

    // offset
    CPoint<float> point;

};
