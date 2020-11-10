/************************************************************************
*    FILE NAME:       rect.h
*
*    DESCRIPTION:     Rect class
************************************************************************/  

#pragma once

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

template <typename type>
class CRect
{
public:

    // size values.
    type x1, y1, x2, y2;

    /************************************************************************                                                             
    *    desc:  Constructor
    ************************************************************************/

    CRect():x1(0),y1(0),x2(0),y2(0)
    { }

    // Copy constructor
    template <typename U>
    CRect( const CRect<U> &rect ):x1(rect.x1),y1(rect.y1),x2(rect.x2),y2(rect.y2)
    { }

    // constructor
    CRect( type _x1, type _y1, type _x2, type _y2 ):x1(_x1),y1(_y1),x2(_x2),y2(_y2)
    { }

    /************************************************************************                                                             
    *    desc:  Get functions
    ************************************************************************/
    type getX1() const
    { return x1; }

    type getY1() const
    { return y1; }

    type getX2() const
    { return x2; }

    type getY2() const
    { return y2; }

    // Sets the rect's x and y value based on an index and three counts
    void setRectXY( int index, int columnCount, int rowCount, type textureHeight, type textureWidth )
    {
        int heightIndex = 0;
        int widthIndex = 0;

        for( int i = 0; i < rowCount; ++i )
        {
            if( columnCount <= index )
            {
                index -= columnCount;
            }
            else
            {
                heightIndex = i;
                widthIndex = index;
                break;
            }
        }

        type frameWidth = textureWidth / columnCount;
        type frameHeight = textureHeight / rowCount ;

        x1 = widthIndex * frameWidth;
        y1 = heightIndex * frameHeight;

        x2 = x1 + frameWidth;
        y2 = y1 + frameHeight;
    }

    /************************************************************************                                                             
    *    desc:  Does this size not have any data? 
    ************************************************************************/
    bool isEmpty() const
    {
        return ( isX1Empty() && isX2Empty() && isY1Empty() && isY2Empty() );
    }

    bool isX1Empty() const
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        // This looks at the bits of the data as an int
        return ( (0 == x1) || (*(int*)&x1 == 0) );
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }

    bool isX2Empty() const
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        // This looks at the bits of the data as an int
        return ( (0 == x2) || (*(int*)&x2 == 0) );
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }

    bool isY1Empty() const
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        // This looks at the bits of the data as an int
        return ( (0 == y1) || (*(int*)&y1 == 0) );
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }

    bool isY2Empty() const
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        // This looks at the bits of the data as an int
        return ( (0 == y2) || (*(int*)&y2 == 0) );
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }

    /************************************************************************                                                             
    *    desc:  Is the point in the rect
    *           Note: Fast but does not work when rect is rotated
    ************************************************************************/
    bool isPointInRect( type x, type y )
    {
        if( ((x > x1) && (x < x2)) && ((y > y1) && (y < y2)) )
            return true;

        return false;
    }
};
