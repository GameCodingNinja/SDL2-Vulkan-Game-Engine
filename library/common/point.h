
/************************************************************************
*    FILE NAME:       point.h
*
*    DESCRIPTION:     Point class
************************************************************************/

#ifndef __point_h__
#define __point_h__

// Standard lib dependencies
#include <complex>

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

template <typename type>
class CPoint
{
public:

    // point values.
    type x, y, z;

    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    CPoint() : x(0), y(0), z(0)
    {
    }

    // Needs to be done like this to avoid recursion
    template <typename U>
    CPoint( const CPoint<U> & obj ) : x(obj.x), y(obj.y), z(obj.z)
    {
    }

    CPoint( type _x, type _y, type _z ) : x(_x), y(_y), z(_z)
    {
    }

    CPoint( type _x, type _y ) : x(_x), y(_y), z(0)
    {
    }


    /************************************************************************
    *    desc:  Get functions for const calls
    ************************************************************************/
    type getX() const
    {
        return x;
    }

    type getY() const
    {
        return y;
    }

    type getZ() const
    {
        return z;
    }


    /************************************************************************
    *    desc:  Clear the x
    ************************************************************************/
    void clearX()
    {
        x = 0.0f;
    }

    /************************************************************************
    *    desc:  Clear the y
    ************************************************************************/
    void clearY()
    {
        y = 0.0f;
    }

    /************************************************************************
    *    desc:  Clear the z
    ************************************************************************/
    void clearZ()
    {
        z = 0.0f;
    }

    /************************************************************************
    *    desc:  Clear the values
    ************************************************************************/
    void clear()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    /************************************************************************
    *    desc:  The equality operator
    ************************************************************************/
    bool operator == ( const CPoint & point ) const
    {
        if( (point.x == x) && (point.y == y) && (point.z == z) )
            return true;

        return false;
    }

    /************************************************************************
    *    desc:  The inequality operator
    ************************************************************************/
    bool operator != ( const CPoint & point ) const
    {
        if( ( point.x != x ) || ( point.y != y ) || ( point.z != z ) )
            return true;

        return false;
    }
    
    
    /************************************************************************
    *    desc:  Greater than operator
    ************************************************************************/
    bool operator > ( const CPoint & point ) const
    {
        if( ( x > point.x ) || ( y > point.y ) || ( z > point.z ) )
            return true;

        return false;
    }
    
    bool operator > ( const type value ) const
    {
        if( ( value > x ) || ( value > y ) || ( value > z ) )
            return true;

        return false;
    }
    
    
    /************************************************************************
    *    desc:  Greater than or equal to operator
    ************************************************************************/
    bool operator >= ( const CPoint & point ) const
    {
        if( ( x >= point.x ) || ( y >= point.y ) || ( z >= point.z ) )
            return true;

        return false;
    }
    
    bool operator >= ( const type value ) const
    {
        if( ( value >= x ) || ( value >= y ) || ( value >= z ) )
            return true;

        return false;
    }
    
    
    /************************************************************************
    *    desc:  Less than or equal to operator
    ************************************************************************/
    bool operator < ( const CPoint & point ) const
    {
        if( ( x < point.x ) || ( y < point.y ) || ( z < point.z ) )
            return true;

        return false;
    }
    
    bool operator < ( const type value ) const
    {
        if( ( value < x ) || ( value < y ) || ( value < z ) )
            return true;

        return false;
    }
    
    
    /************************************************************************
    *    desc:  Less than or equal to operator
    ************************************************************************/
    bool operator <= ( const CPoint & point ) const
    {
        if( ( x <= point.x ) || ( y <= point.y ) || ( z <= point.z ) )
            return true;

        return false;
    }
    
    bool operator <= ( const type value ) const
    {
        if( ( value <= x ) || ( value <= y ) || ( value <= z ) )
            return true;

        return false;
    }
    
    
    /************************************************************************
    *    desc:  The subtraction operator
    ************************************************************************/
    CPoint operator - ( const CPoint & point ) const
    {
        CPoint tmp;
        tmp.x = x - point.x;
        tmp.y = y - point.y;
        tmp.z = z - point.z;

        return tmp;
    }

    CPoint operator - ( type value ) const
    {
        CPoint tmp;
        tmp.x = x - value;
        tmp.y = y - value;
        tmp.z = z - value;

        return tmp;
    }

    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CPoint operator + ( const CPoint & point ) const
    {
        CPoint tmp;
        tmp.x = x + point.x;
        tmp.y = y + point.y;
        tmp.z = z + point.z;

        return tmp;
    }

    CPoint operator + ( type value ) const
    {
        CPoint tmp;
        tmp.x = x + value;
        tmp.y = y + value;
        tmp.z = z + value;

        return tmp;
    }

    /************************************************************************
    *    desc:  The division operator
    ************************************************************************/
    CPoint operator / ( const CPoint & point ) const
    {
        CPoint tmp;
        tmp.x = x / point.x;
        tmp.y = y / point.y;
        tmp.z = z / point.z;

        return tmp;
    }

    CPoint operator / ( type value ) const
    {
        CPoint tmp;
        tmp.x = x / value;
        tmp.y = y / value;
        tmp.z = z / value;

        return tmp;
    }

    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CPoint operator += ( const CPoint & point )
    {
        x += point.x;
        y += point.y;
        z += point.z;

        return *this;
    }

    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CPoint operator += ( type value )
    {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CPoint operator -= ( const CPoint & point )
    {
        x -= point.x;
        y -= point.y;
        z -= point.z;

        return *this;
    }

    /************************************************************************
    *    desc:  The addition operator
    ************************************************************************/
    CPoint operator -= ( type value )
    {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CPoint operator * ( const CPoint & point ) const
    {
        CPoint tmp;
        tmp.x = x * point.x;
        tmp.y = y * point.y;
        tmp.z = z * point.z;

        return tmp;
    }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CPoint operator * ( type value ) const
    {
        CPoint tmp;
        tmp.x = x * value;
        tmp.y = y * value;
        tmp.z = z * value;

        return tmp;
    }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CPoint operator * ( type * pMat ) const
    {
        CPoint tmp;

        tmp.x = x * pMat[0] + y * pMat[4] + z * pMat[8];
        tmp.y = x * pMat[1] + y * pMat[5] + z * pMat[9];
        tmp.z = x * pMat[2] + y * pMat[6] + z * pMat[10];

        return tmp;
    }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CPoint operator *= ( const CPoint & point )
    {
        x *= point.x;
        y *= point.y;
        z *= point.z;

        return *this;
    }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CPoint operator *= ( type value )
    {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    /************************************************************************
    *    desc:  The multiplication operator
    ************************************************************************/
    CPoint operator *= ( type * pMat )
    {
        CPoint tmp;

        tmp.x = x * pMat[0] + y * pMat[4] + z * pMat[8];
        tmp.y = x * pMat[1] + y * pMat[5] + z * pMat[9];
        tmp.z = x * pMat[2] + y * pMat[6] + z * pMat[10];

        x = tmp.x;
        y = tmp.y;
        z = tmp.z;

        return *this;
    }

    /************************************************************************
    *    desc:  The division operator
    ************************************************************************/
    CPoint operator /= ( const CPoint & point )
    {
        x /= point.x;
        y /= point.y;
        z /= point.z;

        return *this;
    }

    /************************************************************************
    *    desc:  The division operator
    ************************************************************************/
    CPoint operator /= ( type value )
    {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }
    

    /************************************************************************
    *    desc:  Set the point data
    ************************************************************************/
    void set( type _x, type _y, type _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set( type value )
    {
        x = value;
        y = value;
        z = value;
    }
    
    
    /************************************************************************
    *    desc:  Inc the point data
    ************************************************************************/
    void inc( type _x, type _y, type _z )
    {
        x += _x;
        y += _y;
        z += _z;
    }

    /************************************************************************
    *    desc:  Cap the value
    ************************************************************************/
    void cap( type value )
    {
        if( value > 0.0f )
        {
            if( x > value )
            {
                x -= value;
            }
            else if ( x < 0.0f )
            {
                x += value;
            }

            if( y > value )
            {
                y -= value;
            }
            else if ( y < 0.0f )
            {
                y += value;
            }

            if( z > value )
            {
                z -= value;
            }
            else if ( z < 0.0f )
            {
                z += value;
            }
        }
        else
        {
            if( x > value )
            {
                x += value;
            }
            else if ( x < 0.0f )
            {
                x -= value;
            }

            if( y > value )
            {
                y += value;
            }
            else if ( y < 0.0f )
            {
                y -= value;
            }

            if( z > value )
            {
                z += value;
            }
            else if ( z < 0.0f )
            {
                z -= value;
            }
        }
    }

    // For AngelScript
    void capFloat( float value )
    {
        cap( value );
    }

    /************************************************************************
    *    desc:  Does this point not have any data?
    ************************************************************************/
    bool isEmpty() const
    {
        // This looks at the bits of the data as an int
        return ( isXEmpty() && isYEmpty() && isZEmpty() );
    }

    bool isXEmpty() const
    {
        // This looks at the bits of the data as an int
        return ( (0 == x) || (*(int*)&x == 0) );
    }

    bool isYEmpty() const
    {
        // This looks at the bits of the data as an int
        return ( (0 == y) || (*(int*)&y == 0) );
    }

    bool isZEmpty() const
    {
        // This looks at the bits of the data as an int
        return ( (0 == z) || (*(int*)&z == 0) );
    }

    /************************************************************************
    *    desc:  Check if two floating points are equil enough
    ************************************************************************/
    bool isEquilEnough( const CPoint & point, type range )
    {
        if( std::fabs(x - point.x) < range )
            if( std::fabs(y - point.y) < range )
                if( std::fabs(z - point.z) < range )
                    return true;

        return false;
    }

    // For AngelScript
    bool isEquilEnoughFloat( const CPoint & point, float range )
    {
        return isEquilEnough( point, range );
    }

    /************************************************************************
    *    desc:  Invert the values of this point
    ************************************************************************/
    void invert()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    /************************************************************************
    *    desc:  Invert a copy of this point and return it
    ************************************************************************/
    CPoint getInvert()
    {
        CPoint tmp(*this);
        tmp.invert();

        return tmp;
    }

    /************************************************************************
    *    desc:  Invert the values of this point
    ************************************************************************/
    void invertX()
    {
        x = -x;
    }

    /************************************************************************
    *    desc:  Invert the values of this point
    ************************************************************************/
    void invertY()
    {
        y = -y;
    }

    /************************************************************************
    *    desc:  Invert the values of this point
    ************************************************************************/
    void invertZ()
    {
        z = -z;
    }

    /************************************************************************
    *    desc:  Get the length of the point from the origin
    ************************************************************************/
    type getLength() const
    {
        return std::sqrt( getLengthSquared() );
    }


    /************************************************************************
    *    desc:  Get the length between two points
    ************************************************************************/
    type getLength( const CPoint & point ) const
    {
        return std::sqrt( getLengthSquared( point ) );
    }


    /************************************************************************
    *    desc:  Get the length of the point from the origin along the x and y
    *			axis
    ************************************************************************/
    type getLength2D() const
    {
        return std::sqrt( getLengthSquared2D() );
    }
    
    type getLength2D( const CPoint & point ) const
    {
        return std::sqrt( getLengthSquared2D( point ) );
    }

    /************************************************************************
    *    desc:  Get the squared length between two points
    ************************************************************************/
    type getLengthSquared( const CPoint & point ) const
    {
        return (*this - point).getLengthSquared();
    }

    /************************************************************************
    *    desc:  Get the squared length of the point from the origin
    ************************************************************************/
    type getLengthSquared() const
    {
        return ( x * x ) +  ( y * y ) + ( z * z );
    }

    /************************************************************************
    *    desc:  Get the squared length of the point from the origin
    ************************************************************************/
    type getLengthSquared2D() const
    {
        return ( x * x ) + ( y * y );
    }
    
    type getLengthSquared2D( const CPoint & point ) const
    {
        return (*this - point).getLengthSquared2D();
    }

    /************************************************************************
    *    desc:  Get the dot product
    *
    *    param:  CPoint & point
    *
    *    return: type - distance between two points
    ************************************************************************/
    type getDotProduct( const CPoint & point ) const
    {
        return ( x * point.x ) + ( y * point.y ) + ( z * point.z );

    }

    type getDotProduct2D( const CPoint & point ) const
    {
        return ( x * point.x ) + ( y * point.y );

    }

    // For AngelScript
    float getDotProductFloat( const CPoint & point ) const
    {
        return getDotProduct( point );
    }
    float getDotProduct2DFloat( const CPoint & point ) const
    {
        return getDotProductFloat( point );
    }

    /************************************************************************
    *    desc:  normalize this point
    ************************************************************************/
    void normalize()
    {
        type length = getLength();

        if( length != 0.0f )
        {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    void normalize2D()
    {
        type length = getLength2D();

        if( length != 0.0f )
        {
            x /= length;
            y /= length;
        }
    }

    /************************************************************************
    *    desc:  Get the cross product
    ************************************************************************/
    CPoint getCrossProduct( const CPoint & point ) const
    {
        CPoint tmp;
        tmp.x = (y * point.z) - (point.y * z);
        tmp.y = (z * point.x) - (point.z * x);
        tmp.z = (x * point.y) - (point.x * y);
        tmp.normalize();

        return tmp;
    }

    /************************************************************************
    *    desc:  Create a displacement vector
    ************************************************************************/
    CPoint getDisplacement( const CPoint & point, type amount ) const
    {
        CPoint displacement( *this - point );
        displacement.normalize();
        CPoint tmp = *this + (displacement * amount);

        return tmp;
    }


    /***********************************************************************************
    *	NEGATIVE OPERATOR
    ***********************************************************************************/
    /************************************************************************
    *    desc:  Return a copy of the point with its signs flipped
    ************************************************************************/
    CPoint operator - () const
    {
        return CPoint(-x,-y,-z);
    }

};

#endif  // __point_h__

