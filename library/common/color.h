/************************************************************************
*    FILE NAME:       color.h
*
*    DESCRIPTION:     Color class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/defs.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <string>

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

/************************************************************************
*	Class: CColor
*
*	Description: Class for holding color values
************************************************************************/
class CColor
{
public:

    float r, g, b, a;

    /************************************************************************                                                             
    *    desc:  Constructor
    ************************************************************************/
    CColor():r(1),g(1),b(1),a(1){};
    CColor(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a)
    { convert(); }

    // Copy constructor
    CColor( const CColor & color ) : r(color.r), g(color.g), b(color.b), a(color.a)
    { convert(); }

    
    /************************************************************************                                                             
    *    desc:  Get functions
    ************************************************************************/
    float getR() const
    { return r; }

    float getG() const
    { return g; }

    float getB() const
    { return b; }

    float getA() const
    { return a; }

    /************************************************************************                                                             
    *    desc:  Convert from integer to decimal
    ************************************************************************/
    void convert()
	{
        // 0.00390625f = 1 / 256;
        if (r > 1.5f)
            r *= defs_RGB_TO_DEC;

        if (g > 1.5f)
            g *= defs_RGB_TO_DEC;

        if (b > 1.5f)
            b *= defs_RGB_TO_DEC;

        if (a > 1.5f)
            a *= defs_RGB_TO_DEC;
    }

    /************************************************************************                                                             
    *    desc:  Clear the values
    ************************************************************************/
    void clear()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    /************************************************************************                                                             
    *    desc:  Reset the values back to 1
    ************************************************************************/
    void reset()
    {
        r = 1;
        g = 1;
        b = 1;
        a = 1;
    }

    /************************************************************************                                                             
    *    desc:  The equality operator 
    ************************************************************************/
    bool operator == ( const CColor & color ) const
    {
        if( (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a) )
        {
            return true;
        }
        return false;
    }

    /************************************************************************                                                             
    *    desc:  The inequality operator 
    ************************************************************************/
    bool operator != ( const CColor & color ) const
    {
        if( ( color.r != r ) || ( color.g != g ) || ( color.b != b ) || ( color.a != a ) )
        {
            return true;
        }
        return false;
    }

    /********************************************************************************************                                                             
    *    SUBTRACTION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CColor operator - ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r - color.r;
        tmp.g = g - color.g;
        tmp.b = b - color.b;
        tmp.a = a - color.a;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CColor operator - ( float value ) const
    {
        CColor tmp;
        tmp.r = r - value;
        tmp.g = g - value;
        tmp.b = b - value;
        tmp.a = a - value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator -= ( const CColor & color )
    {
        r -= color.r;
        g -= color.g;
        b -= color.b;
        a -= color.a;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator -= ( float value )
    {
        r -= value;
        g -= value;
        b -= value;
        a -= value;

        return *this;
    }

    /********************************************************************************************                                                             
    *    ADDITION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator + ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r + color.r;
        tmp.g = g + color.g;
        tmp.b = b + color.b;
        tmp.a = a + color.a;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator + ( float value ) const
    {
        CColor tmp;
        tmp.r = r + value;
        tmp.g = g + value;
        tmp.b = b + value;
        tmp.a = a + value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator += ( const CColor & color )
    {
        r += color.r;
        g += color.g;
        b += color.b;
        a += color.a;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator += ( float value )
    {
        r += value;
        g += value;
        b += value;
        a += value;

        return *this;
    }

    /********************************************************************************************                                                             
    *    DIVISION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The division operator 
    ************************************************************************/
    CColor operator / ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r / color.r;
        tmp.g = g / color.g;
        tmp.b = b / color.b;
        tmp.a = a / color.a;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CColor operator / ( float value ) const
    {
        CColor tmp;
        tmp.r = r / value;
        tmp.g = g / value;
        tmp.b = b / value;
        tmp.a = a / value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    ************************************************************************/
    CColor operator /= ( const CColor & color )
    {
        r /= color.r;
        g /= color.g;
        b /= color.b;
        a /= color.a;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    ************************************************************************/
    CColor operator /= ( float value )
    {
        r /= value;
        g /= value;
        b /= value;
        a /= value;

        return *this;
    }

    /********************************************************************************************                                                             
    *    MULTIPLICATION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CColor operator * ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r * color.r;
        tmp.g = g * color.g;
        tmp.b = b * color.b;
        tmp.a = a * color.a;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CColor operator * ( float value ) const
    {
        CColor tmp;
        tmp.r = r * value;
        tmp.g = g * value;
        tmp.b = b * value;
        tmp.a = a * value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CColor operator *= ( const CColor & color )
    {
        r *= color.r;
        g *= color.g;
        b *= color.b;
        a *= color.a;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CColor operator *= ( float value )
    {
        r *= value;
        g *= value;
        b *= value;
        a *= value;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  Set the point data
    ************************************************************************/
    void set( float _r, float _g, float _b, float _a )
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    /************************************************************************                                                             
    *    desc:  HSV transformation 
    ************************************************************************/
    /*CColor TransformHSV( float hue, float sat, float val )
    {
        float VSU = val * sat * cos(hue * defs_DEG_TO_RAD);
        float VSW = val * sat * sin(hue * defs_DEG_TO_RAD);

        CColor ret(r,g,b,a);

        ret.r = (.299 * val + .701 * VSU + .168 * VSW) * r
                  + (.587 * val - .587 * VSU + .330 * VSW) * g
                  + (.114 * val - .114 * VSU - .497 * VSW) * b;
        ret.g = (.299 * val - .299 * VSU - .328 * VSW) * r
                  + (.587 * val + .413 * VSU + .035 * VSW) * g
                  + (.114 * val - .114 * VSU + .292 * VSW) * b;
        ret.b = (.299 * val - .3 * VSU + 1.25 * VSW) * r
                  + (.587 * val - .588 * VSU - 1.05 * VSW) * g
                  + (.114 * val + .886 * VSU -.203 * VSW) * b;

        return ret;
    }*/

    void transformHSV( float hue, float sat, float val )
    {
        float VSU = val * sat * cos(hue * defs_DEG_TO_RAD);
        float VSW = val * sat * sin(hue * defs_DEG_TO_RAD);

        float _r(r), _g(g), _b(b);

        r = (.299 * val + .701 * VSU + .168 * VSW) * _r
          + (.587 * val - .587 * VSU + .330 * VSW) * _g
          + (.114 * val - .114 * VSU - .497 * VSW) * _b;
        g = (.299 * val - .299 * VSU - .328 * VSW) * _r
          + (.587 * val + .413 * VSU + .035 * VSW) * _g
          + (.114 * val - .114 * VSU + .292 * VSW) * _b;
        b = (.299 * val - .3 * VSU + 1.25 * VSW) * _r
          + (.587 * val - .588 * VSU - 1.05 * VSW) * _g
          + (.114 * val + .886 * VSU -.203 * VSW) * _b;
    }

    /************************************************************************                                                             
    *    desc:  Get a string of the color's data
    ************************************************************************/
    std::string dump() const
    {
        std::string printout = boost::str( boost::format("Color( r: %d, g: %d, b: %d, a: %d )")
            % r
            % g
            % b
            % a );

        return printout;
    }

};
