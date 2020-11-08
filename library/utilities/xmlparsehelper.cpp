/************************************************************************
*    FILE NAME:       xmlparsehelper.cpp
*
*    DESCRIPTION:     General xml parse helper functions
************************************************************************/

// Physical component dependency
#include <utilities/xmlparsehelper.h>

// Standard lib dependencies
#include <cstring>

namespace NParseHelper
{
    /************************************************************************
    *    DESC:  Load the vert uv
    ************************************************************************/
    NVertex::vert_uv Load_vert_uv( const XMLNode & node )
    {
        NVertex::vert_uv vert2D;

        if( !node.isEmpty() )
        {
            if( node.isAttributeSet( "x" ) )
                vert2D.vert.x = std::atof( node.getAttribute( "x" ) );

            if( node.isAttributeSet( "y" ) )
                vert2D.vert.y = std::atof( node.getAttribute( "y" ) );

            if( node.isAttributeSet( "z" ) )
                vert2D.vert.z = std::atof( node.getAttribute( "z" ) );

            if( node.isAttributeSet( "u" ) )
                vert2D.uv.u = std::atof( node.getAttribute( "u" ) );

            if( node.isAttributeSet( "v" ) )
                vert2D.uv.v = std::atof( node.getAttribute( "v" ) );
        }

        return vert2D;
    }


    /************************************************************************
    *    DESC:  Load the position
    ************************************************************************/
    CPoint<CWorldValue> LoadPosition( const XMLNode & node, bool & loaded )
    {
        CPoint<CWorldValue> point;

        loaded = false;

        XMLNode positionNode = node.getChildNode( "position" );

        if( !positionNode.isEmpty() )
        {
            loaded = true;

            if( positionNode.isAttributeSet( "xi" ) )
                point.x.i = std::atoi( positionNode.getAttribute( "xi" ) );

            if( positionNode.isAttributeSet( "yi" ) )
                point.y.i = std::atoi( positionNode.getAttribute( "yi" ) );

            if( positionNode.isAttributeSet( "zi" ) )
                point.z.i = std::atoi( positionNode.getAttribute( "zi" ) );

            if( positionNode.isAttributeSet( "xf" ) )
                point.x.f = std::atof( positionNode.getAttribute( "xf" ) );
            else if( positionNode.isAttributeSet( "x" ) )
                point.x.f = std::atof( positionNode.getAttribute( "x" ) );

            if( positionNode.isAttributeSet( "yf" ) )
                point.y.f = std::atof( positionNode.getAttribute( "yf" ) );
            else if( positionNode.isAttributeSet( "y" ) )
                point.y.f = std::atof( positionNode.getAttribute( "y" ) );

            if( positionNode.isAttributeSet( "zf" ) )
                point.z.f = std::atof( positionNode.getAttribute( "zf" ) );
            else if( positionNode.isAttributeSet( "z" ) )
                point.z.f = std::atof( positionNode.getAttribute( "z" ) );
        }

        return point;
    }

    CPoint<CWorldValue> LoadPosition( const XMLNode & node )
    {
        bool dummy;
        return LoadPosition( node, dummy );
    }


    /************************************************************************
    *    DESC:  Load the rotation
    ************************************************************************/
    CPoint<float> LoadRotation( const XMLNode & node, bool & loaded )
    {
        CPoint<float> point;

        loaded = false;

        XMLNode rotationNode = node.getChildNode( "rotation" );

        if( !rotationNode.isEmpty() )
        {
            loaded = true;

            if( rotationNode.isAttributeSet( "x" ) )
                point.x = std::atof( rotationNode.getAttribute( "x" ) );

            if( rotationNode.isAttributeSet( "y" ) )
                point.y = std::atof( rotationNode.getAttribute( "y" ) );

            if( rotationNode.isAttributeSet( "z" ) )
                point.z = std::atof( rotationNode.getAttribute( "z" ) );
        }

        return point;
    }

    CPoint<float> LoadRotation( const XMLNode & node )
    {
        bool dummy;
        return LoadRotation( node, dummy );
    }


    /************************************************************************
    *    DESC:  Load the scale
    ************************************************************************/
    CPoint<float> LoadScale( const XMLNode & node, bool & loaded )
    {
        CPoint<float> point(1,1,1);

        loaded = false;

        XMLNode scaleNode = node.getChildNode( "scale" );
        if( !scaleNode.isEmpty() )
        {
            loaded = true;

            if( scaleNode.isAttributeSet( "uniform" ) )
                point.set( std::atof( scaleNode.getAttribute( "uniform" ) ) );

            else
            {
                if( scaleNode.isAttributeSet( "x" ) )
                    point.x = std::atof( scaleNode.getAttribute( "x" ) );

                if( scaleNode.isAttributeSet( "y" ) )
                    point.y = std::atof( scaleNode.getAttribute( "y" ) );

                if( scaleNode.isAttributeSet( "z" ) )
                    point.z = std::atof( scaleNode.getAttribute( "z" ) );
            }
        }

        return point;
    }

    CPoint<float> LoadScale( const XMLNode & node )
    {
        bool dummy;
        return LoadScale( node, dummy );
    }


    /************************************************************************
    *    DESC:  Load the center position
    ************************************************************************/
    CPoint<float> LoadCenterPos( const XMLNode & node, bool & loaded )
    {
        CPoint<float> point;

        loaded = false;

        XMLNode centerPosNode = node.getChildNode( "centerPos" );

        if( !centerPosNode.isEmpty() )
        {
            loaded = true;

            if( centerPosNode.isAttributeSet( "x" ) )
                point.x = std::atof( centerPosNode.getAttribute( "x" ) );

            if( centerPosNode.isAttributeSet( "y" ) )
                point.y = std::atof( centerPosNode.getAttribute( "y" ) );

            if( centerPosNode.isAttributeSet( "z" ) )
                point.z = std::atof( centerPosNode.getAttribute( "z" ) );
        }

        return point;
    }

    CPoint<float> LoadCenterPos( const XMLNode & node )
    {
        bool dummy;
        return LoadCenterPos( node, dummy );
    }


    /************************************************************************
    *    DESC:  Load the generic x, y, z values
    ************************************************************************/
    CPoint<float> LoadXYZ( const XMLNode & node )
    {
        CPoint<float> point;

        if( node.isAttributeSet( "x" ) )
                point.x = std::atof( node.getAttribute( "x" ) );

        if( node.isAttributeSet( "y" ) )
                point.y = std::atof( node.getAttribute( "y" ) );

        if( node.isAttributeSet( "z" ) )
                point.z = std::atof( node.getAttribute( "z" ) );

        return point;
    }


    /************************************************************************
    *    DESC:  Load the color
    ************************************************************************/
    CColor LoadColor( const XMLNode & node, const CColor & currentColor )
    {
        CColor color( currentColor );

        XMLNode colorNode = node.getChildNode( "color" );
        if( !colorNode.isEmpty() )
        {
            if( colorNode.isAttributeSet( "r" ) )
                color.r = std::atof(colorNode.getAttribute( "r" ));

            if( colorNode.isAttributeSet( "g" ) )
                color.g = std::atof(colorNode.getAttribute( "g" ));

            if( colorNode.isAttributeSet( "b" ) )
                color.b = std::atof(colorNode.getAttribute( "b" ));

            if( colorNode.isAttributeSet( "a" ) )
                color.a = std::atof(colorNode.getAttribute( "a" ));

            // Convert if in RGBA format
            color.convert();
        }

        return color;
    }


    /************************************************************************
    *    DESC:  Load the size
    ************************************************************************/
    CSize<int> LoadSize( const XMLNode & node )
    {
        XMLNode sizeNode = node.getChildNode( "size" );
        if( !sizeNode.isEmpty() )
            return LoadSizeFromChild( sizeNode);

        return CSize<int>();
    }
    
    CSize<int> LoadSizeFromChild( const XMLNode & node )
    {
        CSize<int> size;

        if( node.isAttributeSet("width") )
            size.w = std::atoi(node.getAttribute( "width" ));

        if( node.isAttributeSet("height") )
            size.h = std::atoi(node.getAttribute( "height" ));

        return size;
    }


    /************************************************************************
    *    DESC:  Load the rect
    *
    *    param: node - passed in node
    ************************************************************************/
    CRect<int> LoadRect( const XMLNode & node )
    {
        const XMLNode rectNode = node.getChildNode( "rect" );
        if( !rectNode.isEmpty() )
            return LoadRectFromChild( rectNode );

        return CRect<int>();
    }
    
    CRect<int> LoadRectFromChild( const XMLNode & node )
    {
        CRect<int> rect;

        if( node.isAttributeSet("x1") )
            rect.x1 = std::atoi(node.getAttribute( "x1" ));

        if( node.isAttributeSet("y1") )
            rect.y1 = std::atoi(node.getAttribute( "y1" ));

        if( node.isAttributeSet("x2") )
            rect.x2 = std::atoi(node.getAttribute( "x2" ));

        if( node.isAttributeSet("y2") )
            rect.y2 = std::atoi(node.getAttribute( "y2" ));

        return rect;
    }


    /************************************************************************
    *    DESC:  Load the horizontal alignment
    ************************************************************************/
    EHorzAlignment LoadHorzAlignment( const XMLNode & node, EHorzAlignment horzAlignment )
    {
        EHorzAlignment horzAlign = horzAlignment;

        if( node.isAttributeSet( "horzAlign" ) )
        {
            std::string horzAlignStr = node.getAttribute( "horzAlign" );
            if( horzAlignStr == "left" )
                horzAlign = EHorzAlignment::HORZ_LEFT;

            else if( horzAlignStr == "center" )
                horzAlign = EHorzAlignment::HORZ_CENTER;

            else if( horzAlignStr == "right" )
                horzAlign = EHorzAlignment::HORZ_RIGHT;
        }

        return horzAlign;
    }


    /************************************************************************
    *    DESC:  Load the vertical alignment
    ************************************************************************/
    EVertAlignment LoadVertAlignment( const XMLNode & node, EVertAlignment vertAlignment )
    {
        EVertAlignment vertAlign = vertAlignment;

        if( node.isAttributeSet( "vertAlign" ) )
        {
            std::string vertAlignStr = node.getAttribute( "vertAlign" );
            if( vertAlignStr == "top" )
                vertAlign = EVertAlignment::VERT_TOP;

            else if( vertAlignStr == "center" )
                vertAlign = EVertAlignment::VERT_CENTER;

            else if( vertAlignStr == "bottom" )
                vertAlign = EVertAlignment::VERT_BOTTOM;
        }

        return vertAlign;
    }


    /************************************************************************
    *    DESC:  Load the dynamic offset
    ************************************************************************/
    CDynamicOffset LoadDynamicOffset( const XMLNode & node, bool & loaded )
    {
        CDynamicOffset dynamicOffset;

        XMLNode dynamicOffsetNode = node.getChildNode( "dynamicOffset" );
        
        loaded = false;

        if( !dynamicOffsetNode.isEmpty() )
        {
            loaded = true;
            
            if( dynamicOffsetNode.isAttributeSet( "left" ) )
            {
                dynamicOffset.add( NOffset::LEFT );
                dynamicOffset.setX( std::atof( dynamicOffsetNode.getAttribute( "left" ) ) );
            }
            else if( dynamicOffsetNode.isAttributeSet( "right" ) )
            {
                dynamicOffset.add( NOffset::RIGHT );
                dynamicOffset.setX( std::atof( dynamicOffsetNode.getAttribute( "right" ) ) );
            }
            else if( dynamicOffsetNode.isAttributeSet( "horzCenter" ) )
            {
                dynamicOffset.add( NOffset::HORZ_CENTER );
                dynamicOffset.setX( std::atof( dynamicOffsetNode.getAttribute( "horzCenter" ) ) );
            }

            if( dynamicOffsetNode.isAttributeSet( "top" ) )
            {
                dynamicOffset.add( NOffset::TOP );
                dynamicOffset.setY( std::atof( dynamicOffsetNode.getAttribute( "top" ) ) );
            }

            else if( dynamicOffsetNode.isAttributeSet( "bottom" ) )
            {
                dynamicOffset.add( NOffset::BOTTOM );
                dynamicOffset.setY( std::atof( dynamicOffsetNode.getAttribute( "bottom" ) ) );
            }
            else if( dynamicOffsetNode.isAttributeSet( "vertCenter" ) )
            {
                dynamicOffset.add( NOffset::VERT_CENTER );
                dynamicOffset.setX( std::atof( dynamicOffsetNode.getAttribute( "vertCenter" ) ) );
            }
        }

        return dynamicOffset;
    }


    /************************************************************************
    *    DESC:  Init the script functions and add them to the map
    *           This function loads the attribute info reguardless of what it is
    ************************************************************************/
    void initScriptFunctions(
        const XMLNode & node,
        std::map<const std::string, CScriptPrepareFunc> & scriptFunctionMap,
        const std::string & rGroup )
    {
        // Check for scripting - Add an empty string for scripts not defined
        XMLNode scriptLstNode = node.getChildNode( "scriptList" );
        if( !scriptLstNode.isEmpty() )
        {
            for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
            {
                const XMLNode scriptNode = scriptLstNode.getChildNode(i);

                // Only the first attribute is used
                const XMLAttribute attribute = scriptNode.getAttribute(0);
                const std::string attrName = attribute.lpszName;
                const std::string attrValue = attribute.lpszValue;

                // Add the attribute name and value to the map
                if( !attrValue.empty() )
                {
                    // Get the group for this script. Default it to the object data group
                    std::string group = rGroup;
                    if( scriptNode.isAttributeSet( "group" ) )
                    {
                        group = scriptNode.getAttribute( "group" );
                        if( group.empty() )
                            group = rGroup;
                    }

                    bool prepareOnInit = false;
                    if( scriptNode.isAttributeSet( "prepareOnInit" ) )
                        prepareOnInit = ( std::strcmp( scriptNode.getAttribute("prepareOnInit"), "true" ) == 0 );

                    bool forceUpdate = false;
                    if( scriptNode.isAttributeSet( "forceUpdate" ) )
                        forceUpdate = ( std::strcmp( scriptNode.getAttribute("forceUpdate"), "true" ) == 0 );

                    scriptFunctionMap.emplace( 
                        std::piecewise_construct, std::forward_as_tuple(attrName), std::forward_as_tuple(group, attrValue, prepareOnInit, forceUpdate) );
                }
            }
        }
    }

}
