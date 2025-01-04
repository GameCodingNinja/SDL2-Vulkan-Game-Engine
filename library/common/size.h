
/************************************************************************
*    FILE NAME:       size.h
*
*    DESCRIPTION:     Size class
************************************************************************/

#pragma once

// Standard lib dependencies
#include <math.h>
#include <complex>

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

template <typename type>
class CPoint;

template <typename type>
class CSize
{
public:

    // size values.
    type w, h;

    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    CSize():w(0),h(0)
    {
    }

    // Needs to be done like this to avoid recursion
    template <typename U>
    CSize( const CSize<U> &size ) : w(size.w), h(size.h)
    {
    }

    // Copy constructor
    CSize( type _w, type _h ):w(_w),h(_h)
    {
    }

    /************************************************************************
    *    desc:  Get functions
    ************************************************************************/
    type getW() const
    { return w; }

    type getH() const
    { return h; }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CSize operator * ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w * size.w;
        tmp.h = h * size.h;

        return tmp;
    }

    CSize operator * ( float value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w * value);
        tmp.h = static_cast<type>(h * value);

        return tmp;
    }

    CSize operator * ( int value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w * value);
        tmp.h = static_cast<type>(h * value);

        return tmp;
    }

    template <typename U>
    CSize<U> operator * ( const CPoint<U> & value ) const
    {
        CSize tmp;
        tmp.w = w * value.x;
        tmp.h = h * value.y;

        return tmp;
    }
    
    /************************************************************************
    *    desc:  The division operator
    ************************************************************************/
    CSize operator / ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w / size.w;
        tmp.h = h / size.h;

        return tmp;
    }

    CSize operator / ( float value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w / value);
        tmp.h = static_cast<type>(h / value);

        return tmp;
    }

    CSize operator / ( int value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w / value);
        tmp.h = static_cast<type>(h / value);

        return tmp;
    }
    
    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CSize operator + ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w + size.w;
        tmp.h = h + size.h;

        return tmp;
    }
    
    CSize operator + ( float value ) const
    {
        CSize tmp;
        tmp.w = w + static_cast<type>(value);
        tmp.h = h + static_cast<type>(value);

        return tmp;
    }
    
    CSize operator + ( int value ) const
    {
        CSize tmp;
        tmp.w = w + static_cast<type>(value);
        tmp.h = h + static_cast<type>(value);

        return tmp;
    }

    /************************************************************************
    *    desc:  The subtraction operator
    ************************************************************************/
    CSize operator - ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w - size.w;
        tmp.h = h - size.h;

        return tmp;
    }
    
    CSize operator - ( float value ) const
    {
        CSize tmp;
        tmp.w = w - static_cast<type>(value);
        tmp.h = h - static_cast<type>(value);

        return tmp;
    }
    
    CSize operator - ( int value ) const
    {
        CSize tmp;
        tmp.w = w - static_cast<type>(value);
        tmp.h = h - static_cast<type>(value);

        return tmp;
    }

    /************************************************************************
    *    desc:  The multiplication equal operator
    ************************************************************************/
    CSize operator *= ( const CSize & size )
    {
        w *= size.w;
        h *= size.h;

        return *this;
    }

    CSize operator *= ( float value )
    {
        w *= static_cast<type>(value);
        h *= static_cast<type>(value);

        return *this;
    }

    CSize operator *= ( int value )
    {
        w *= static_cast<type>(value);
        h *= static_cast<type>(value);

        return *this;
    }

    /************************************************************************
    *    desc:  The division equal operator
    ************************************************************************/
    CSize operator /= ( const CSize & size )
    {
        w /= size.w;
        h /= size.h;

        return *this;
    }

    CSize operator /= ( float value )
    {
        w /= static_cast<type>(value);
        h /= static_cast<type>(value);

        return *this;
    }

    CSize operator /= ( int value )
    {
        w /= static_cast<type>(value);
        h /= static_cast<type>(value);

        return *this;
    }

    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CSize operator += ( const CSize & size )
    {
        w += size.w;
        h += size.h;

        return *(this);
    }
    
    CSize operator += ( float value )
    {
        w += static_cast<type>(value);
        h += static_cast<type>(value);

        return *(this);
    }
    
    CSize operator += ( int value )
    {
        w += static_cast<type>(value);
        h += static_cast<type>(value);

        return *(this);
    }

    /************************************************************************
    *    desc:  The subtraction operator
    ************************************************************************/
    CSize operator -= ( const CSize & size )
    {
        w -= size.w;
        h -= size.h;

        return *(this);
    }
    
    CSize operator -= ( float value )
    {
        w -= static_cast<type>(value);
        h -= static_cast<type>(value);

        return *(this);
    }
    
    CSize operator -= ( int value )
    {
        w -= static_cast<type>(value);
        h -= static_cast<type>(value);

        return *(this);
    }

    /************************************************************************
    *    desc:  The equate operator
    ************************************************************************/
    bool operator == ( const CSize & size ) const
    {
        if( (w == size.w) && (h == size.h) )
            return true;

        return false;
    }

    /************************************************************************
    *    desc:  The equate operator
    ************************************************************************/
    bool operator != ( const CSize & size ) const
    {
        if( (w == size.w) && (h == size.h) )
            return false;

        return true;
    }
    
    /************************************************************************
    *    desc:  Greater than operator
    ************************************************************************/
    bool operator > ( const CSize & size ) const
    {
        if( ( w > size.w ) || ( h > size.h ) )
            return true;

        return false;
    }
    
    /************************************************************************
    *    desc:  Greater than or equal to operator
    ************************************************************************/
    bool operator >= ( const CSize & size ) const
    {
        if( ( w >= size.w ) || ( h >= size.h ) )
            return true;

        return false;
    }
    
    /************************************************************************
    *    desc:  Less than or equal to operator
    ************************************************************************/
    bool operator < ( const CSize & size ) const
    {
        if( ( w < size.w ) || ( h < size.h ) )
            return true;

        return false;
    }
    
    /************************************************************************
    *    desc:  Less than or equal to operator
    ************************************************************************/
    bool operator <= ( const CSize & size ) const
    {
        if( ( w <= size.w ) || ( h <= size.h ) )
            return true;

        return false;
    }
    
    /************************************************************************
    *    desc:  Does this size not have any data?
    ************************************************************************/
    bool isEmpty() const
    {
        return ( isWEmpty() && isHEmpty() );
    }

    bool isWEmpty() const
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        // This looks at the bits of the data as an int
        return ( (0 == w) || (*(int*)&w == 0) );
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }

    bool isHEmpty() const
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        // This looks at the bits of the data as an int
        return ( (0 == h) || (*(int*)&h == 0) );
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }

    /************************************************************************
    *    desc:  Clear the data
    ************************************************************************/
    void clear()
    {
        w = 0;
        h = 0;
    }
    
    /************************************************************************
    *    desc:  Set the size data
    ************************************************************************/
    void set( type _w, type _h )
    {
        w = _w;
        h = _h;
    }

    /************************************************************************
    *    desc:  Round out the floating point number
    ************************************************************************/
    void round()
    {
        w = floor(w + 0.5);
        h = floor(h + 0.5);
    }

    /************************************************************************
    *    desc:  Get the squared length of the size from the origin
    ************************************************************************/
    type getLengthSquared() const
    {
        return ( w * w ) +  ( h * h );
    }

    /************************************************************************
    *    desc:  Get the length of the size from the origin
    ************************************************************************/
    type getLength() const
    {
        return std::sqrt( getLengthSquared() );
    }
    
    /************************************************************************
    *    desc:  Swap the width and height
    ************************************************************************/
    void swap()
    {
        type tmp = w;
        w = h;
        h = tmp;
    }
};
