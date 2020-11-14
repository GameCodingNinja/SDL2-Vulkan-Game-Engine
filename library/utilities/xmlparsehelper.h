/************************************************************************
*    FILE NAME:       xmlparsehelper.h
*
*    DESCRIPTION:     General xml parse helper functions
************************************************************************/           

#pragma once

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>
#include <common/color.h>
#include <common/size.h>
#include <common/rect.h>
#include <common/defs.h>
#include <common/vertex.h>
#include <common/dynamicoffset.h>
#include <utilities/xmlParser.h>
#include <script/scriptpreparefunc.h>

// Standard lib dependencies
#include <assert.h>
#include <utility>
#include <string>
#include <map>

namespace NParseHelper
{
    // Load the vertex
    NVertex::vert_uv Load_vert_uv( const XMLNode & node );

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
    EHorzAlignment LoadHorzAlignment( const XMLNode & node, EHorzAlignment horzAlignment );

    // Load the vertical alignment
    EVertAlignment LoadVertAlignment( const XMLNode & node, EVertAlignment vertAlignment );

    // Load the dynamic offset
    CDynamicOffset LoadDynamicOffset( const XMLNode & node, bool & loaded );

    // Init the script functions and add them to the map
    void initScriptFunctions(
        const XMLNode & node,
        std::map<const std::string, CScriptPrepareFunc> & scriptFunctionMap,
        const std::string & rGroup );
}
