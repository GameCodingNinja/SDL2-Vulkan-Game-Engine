
/************************************************************************
*    FILE NAME:       relativeoffset.h
*
*    DESCRIPTION:     Relative Offset class
************************************************************************/

#ifndef __relative_offset_h__
#define __relative_offset_h__

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/bitmask.h>

class CRelativeOffset
{
public:

    enum ERelativeOffset
    {
        ERO_NULL=0,
        EDO_X=1,
        EDO_Y=2,
        EDO_Z=4,
    };

    // And to the bit mask
    void add( ERelativeOffset value )
    { m_parameters.Add( value ); }
    
    // Check if bits were set
    bool isSet( EDynamicOffset value )
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

        if( m_parameters.IsSet( CDynamicOffset::EDO_LEFT ) )
            pos.x = -(screenCenter.w - point.x);

        else if( m_parameters.IsSet( CDynamicOffset::EDO_RIGHT ) )
            pos.x = screenCenter.w - point.x;

        else if( m_parameters.IsSet( CDynamicOffset::EDO_HORZ_CENTER ) )
            pos.x = point.x;

        if( m_parameters.IsSet( CDynamicOffset::EDO_TOP ) )
            pos.y = screenCenter.h - point.y;
            
        else if( m_parameters.IsSet( CDynamicOffset::EDO_BOTTOM ) )
            pos.y = -(screenCenter.h - point.y);

        else if( m_parameters.IsSet( EDO_VERT_CENTER ) )
            pos.y = point.y;

        return pos;
    }
    
private:

    // bit mask parameters
    CBitmask<uint> m_parameters;

    // offset
    CPoint<float> point;

};

#endif  // __relative_offset_h__


