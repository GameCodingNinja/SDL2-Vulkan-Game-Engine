/************************************************************************
*    FILE NAME:       xmlparsehelper.h
*
*    DESCRIPTION:     General xml parse helper functions
************************************************************************/           

#ifndef __xml_parse_helper_h__
#define __xml_parse_helper_h__

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>
#include <common/color.h>
#include <common/size.h>
#include <common/rect.h>
#include <common/defs.h>
#include <common/vertex2d.h>
#include <common/dynamicoffset.h>
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <assert.h>
#include <utility>
#include <string>

namespace NParseHelper
{
    // Load the 2d vertex
    CVertex2D LoadVertex2d( const XMLNode & node );

    // load position data
    CPoint<CWorldValue> LoadPosition( const XMLNode & node );
    CPoint<CWorldValue> LoadPosition( const XMLNode & node, bool & loaded );

    // Load the rotation
    CPoint<float> LoadRotation( const XMLNode & node );
    CPoint<float> LoadRotation( const XMLNode & node, bool & loaded );

    // Load the scale
    CPoint<float> LoadScale( const XMLNode & node );
    CPoint<float> LoadScale( const XMLNode & node, bool & loaded );
    
    // Load the center position
    CPoint<float> LoadCenterPos( const XMLNode & node );
    CPoint<float> LoadCenterPos( const XMLNode & node, bool & loaded );
    
    // Load the generic x, y, z values
    CPoint<float> LoadXYZ( const XMLNode & node );

    // Load the color
    CColor LoadColor(const  XMLNode & node, const CColor & currentColor );

    // Load the size
    CSize<int> LoadSize( const XMLNode & node );
    CSize<int> LoadSizeFromChild( const XMLNode & node );

    // Load the rect
    CRect<int> LoadRect( const XMLNode & node );
    CRect<int> LoadRectFromChild( const XMLNode & node );

    // Load the horizontal alignment
    NDefs::EHorzAlignment LoadHorzAlignment( const XMLNode & node, NDefs::EHorzAlignment horzAlignment );

    // Load the vertical alignment
    NDefs::EVertAlignment LoadVertAlignment( const XMLNode & node, NDefs::EVertAlignment vertAlignment );

    // Load the dynamic offset
    CDynamicOffset LoadDynamicOffset( const XMLNode & node );
}

#endif  // __xml_parse_helper_h__
