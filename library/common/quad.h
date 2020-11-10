/************************************************************************
*    FILE NAME:       quad.h
*
*    DESCRIPTION:     Quad class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/point.h>

class CQuad 
{
public:

    CQuad(){};

    CPoint<float> point[4];

    // Copy constructor
    CQuad( const CQuad & obj )
    {
        *this = obj;
    }

    /************************************************************************                                                             
    *    desc:  The equality operator 
    ************************************************************************/
    bool operator == ( const CQuad & quad ) const
    {
        if( (quad.point[0] == point[0]) && (quad.point[1] == point[1]) && 
            (quad.point[2] == point[2]) && (quad.point[3] == point[3]) )
        {
            return true;
        }
        return false;
    }

    /************************************************************************                                                             
    *    desc:  The inequality operator 
    ************************************************************************/
    bool operator != ( const CQuad & quad ) const
    {
        if( (quad.point[0] != point[0]) || (quad.point[1] != point[1]) || 
            (quad.point[2] != point[2]) || (quad.point[3] != point[3]) )
        {
            return true;
        }
        return false;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator - ( const CQuad & quad ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] - quad.point[i];

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator - ( float value ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] - value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator - ( const CPoint<float> & _point ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] - _point;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuad operator + ( const CQuad & quad ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] + quad.point[i];

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuad operator + ( float value ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] + value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator + ( const CPoint<float> & _point ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] + _point;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuad operator * ( const CQuad & quad ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] * quad.point[i];

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuad operator * ( float value ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] * value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator * ( const CPoint<float> & _point ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] * _point;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuad operator / ( const CQuad & quad ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] / quad.point[i];

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuad operator / ( float value ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] / value;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator / ( const CPoint<float> & _point ) const
    {
        CQuad tmp;

        for( int i = 0; i < 4; ++i )
            tmp.point[i] = point[i] / _point;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuad operator += ( const CQuad & quad )
    {
        for( int i = 0; i < 4; ++i )
            point[i] += quad.point[i];

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuad operator += ( float value )
    {
        for( int i = 0; i < 4; ++i )
            point[i] += value;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuad operator += ( const CPoint<float> & _point )
    {
        for( int i = 0; i < 4; ++i )
            point[i] += _point;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator -= ( const CQuad & quad )
    {
        for( int i = 0; i < 4; ++i )
            point[i] -= quad.point[i];

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator -= ( float value )
    {
        for( int i = 0; i < 4; ++i )
            point[i] -= value;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator -= ( const CPoint<float> & _point )
    {
        for( int i = 0; i < 4; ++i )
            point[i] -= _point;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuad operator *= ( const CQuad & quad )
    {
        for( int i = 0; i < 4; ++i )
            point[i] *= quad.point[i];

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuad operator *= ( float value )
    {
        for( int i = 0; i < 4; ++i )
            point[i] *= value;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuad operator *= ( const CPoint<float> & _point )
    {
        for( int i = 0; i < 4; ++i )
            point[i] *= _point;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    ************************************************************************/
    CQuad operator /= ( const CQuad & quad )
    {
        for( int i = 0; i < 4; ++i )
            point[i] /= quad.point[i];

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    ************************************************************************/
    CQuad operator /= ( float value )
    {
        for( int i = 0; i < 4; ++i )
            point[i] /= value;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    ************************************************************************/
    CQuad operator /= ( const CPoint<float> & _point )
    {
        for( int i = 0; i < 4; ++i )
            point[i] /= _point;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  Is the point in the Quad
    *           Note: Fast but does not work when quad is rotated
    ************************************************************************/
    bool isPointInQuad( float x, float y )
    {
        bool result(false);
        for( int i = 0, j = 3; i < 4; j = i++ )
        {
            if( ((point[i].y > y) != (point[j].y > y)) && 
                (x < (point[j].x - point[i].x) * (y - point[i].y) / (point[j].y - point[i].y) + point[i].x) )
            {
                result = !result;
            }
        }

        return result;
    }
};
