
/************************************************************************
*    FILE NAME:       quaternion.h
*
*    DESCRIPTION:     3D quaternion class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <math.h>

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#endif

class CQuaternion
{
public:

    // Quaternion values. 
    double x, y, z, w, length;

    // Fast init if class varaibles.
    CQuaternion():length(0)
    {
        identity();
    }

    // Copy constructor
    CQuaternion( const CQuaternion & obj )
    {
        *this = obj;
    }

    // Constructor
    CQuaternion( double _x, double _y, double _z, double _w ):x(_x),y(_y),z(_z),w(_w)
    {
    }

    /************************************************************************                                                             
    *    desc:  Set to it's identity
    ************************************************************************/
    void identity()
    {
        x = 0.;
        y = 0.;
        z = 0.;
        w = 1.;
    }

    /************************************************************************                                                             
    *    desc:  Set a rotation point - Assuming the angles are in radians
    ************************************************************************/
    void setRotation( const CPoint<float> & rot )
    {
        /*double a = rot.x / 2.0;
        double h = rot.y / 2.0;
        double b = rot.z / 2.0;

        // attitude
        double c2 = cos(a);
        double s2 = sin(a);

        // heading
        double c1 = cos(h);
        double s1 = sin(h);

        // bank
        double c3 = cos(b);
        double s3 = sin(b);

        double c1c2 = c1 * c2;
        double s1s2 = s1 * s2;

        w = (c1c2 * c3) - (s1s2 * s3);
        x = (s1 * c2 * c3) + (c1 * s2 * s3);
        y = (c1c2 * s3) + (s1s2 * c3);
        z = (c1 * s2 * c3) - (s1 * c2 * s3);*/

        double p = rot.x / 2.0;
        double y = rot.y / 2.0;
        double r = rot.z / 2.0;
 
        double sinp = sin(p);
        double siny = sin(y);
        double sinr = sin(r);
        double cosp = cos(p);
        double cosy = cos(y);
        double cosr = cos(r);

        x = sinr * cosp * cosy - cosr * sinp * siny;
        y = cosr * sinp * cosy + sinr * cosp * siny;
        z = cosr * cosp * siny - sinr * sinp * cosy;
        w = cosr * cosp * cosy + sinr * sinp * siny;

        normalize();
    }
    
    /************************************************************************                                                             
    *    desc:  Get the Euler angles from the quad
    ************************************************************************/
    CPoint<float> getRotation() const
    {
        CPoint<float> tmp;

        double test = x * y + z * w;

        // singularity at north pole
        if( test > 0.499 )
        {
            // attitude
            tmp.x = M_PI_2;

            // heading
            tmp.y = 2 * atan2(x, w);

            // bank
            tmp.z = 0;
        }
        // singularity at south pole
        else if( test < -0.499 )
        {
            // attitude
            tmp.x = -M_PI_2;

            // heading
            tmp.y = -2 * atan2(x, w);

            // bank
            tmp.z = 0;
        }
        else
        {
            double sqx = x * x;
            double sqy = y * y;
            double sqz = z * z;

            // attitude
            tmp.x = asin(2*test);

            // heading
            tmp.y = atan2( 2*y*w-2*x*z, 1 - 2*sqy - 2*sqz );

            // bank
            tmp.z = atan2( 2*x*w - 2*y*z, 1 - 2*sqx - 2*sqz );
        }

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The equality operator 
    ************************************************************************/
    bool operator == ( const CQuaternion & quat ) const
    {
        if( (quat.x == x) && (quat.y == y) && (quat.z == z) && (quat.w == w) )
            return true;

        return false;
    }

    /************************************************************************                                                             
    *    desc:  The inequality operator 
    ************************************************************************/
    bool operator != ( const CQuaternion & quat ) const
    {
        if( ( quat.x != x ) || ( quat.y != y ) || ( quat.z != z ) || ( quat.w != w ) )
            return true;

        return false;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuaternion operator * ( const CQuaternion & quat ) const
    {
        CQuaternion tmp;

        tmp.x = (w * quat.x) + (x * quat.w) + (y * quat.z) - (z * quat.y);
        tmp.y = (w * quat.y) + (y * quat.w) + (z * quat.x) - (x * quat.z);
        tmp.z = (w * quat.z) + (x * quat.y) - (y * quat.x) + (z * quat.w);
        tmp.w = (w * quat.w) - (x * quat.x) - (y * quat.y) - (z * quat.z);

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    ************************************************************************/
    CQuaternion operator *= ( const CQuaternion & quat )
    {
        x = (w * quat.x) + (x * quat.w) + (y * quat.z) - (z * quat.y);
        y = (w * quat.y) + (y * quat.w) + (z * quat.x) - (x * quat.z);
        z = (w * quat.z) + (x * quat.y) - (y * quat.x) + (z * quat.w);
        w = (w * quat.w) - (x * quat.x) - (y * quat.y) - (z * quat.z);

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuaternion operator + ( const CQuaternion & quat ) const
    {
        CQuaternion tmp;
        tmp.x = x + quat.x;
        tmp.y = y + quat.y;
        tmp.z = z + quat.z;
        tmp.w = w + quat.w;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The addition operator 
    ************************************************************************/
    CQuaternion operator += ( const CQuaternion & quat )
    {
        x += quat.x;
        y += quat.y;
        z += quat.z;
        w += quat.w;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuaternion operator - ( const CQuaternion & quat ) const
    {
        CQuaternion tmp;
        tmp.x = x - quat.x;
        tmp.y = y - quat.y;
        tmp.z = z - quat.z;
        tmp.w = w - quat.w;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    ************************************************************************/
    CQuaternion operator -= ( const CQuaternion & quat )
    {
        x -= quat.x;
        y -= quat.y;
        z -= quat.z;
        w -= quat.w;

        return *this;
    }

    /************************************************************************                                                             
    *    desc:  Set the quaternion data
    ************************************************************************/
    void set( const CPoint<float> & point )
    {
        x = point.x;
        y = point.y;
        z = point.z;

        normalize();
    }

    /************************************************************************                                                             
    *    desc:  Set the quaternion data
    ************************************************************************/
    void set( double _x, double _y, double _z, double _w )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;

        normalize();
    }

    /************************************************************************                                                             
    *    desc:  Invert the values of this quaternion
    ************************************************************************/
    CQuaternion getConjugate() const
    {
        CQuaternion tmp;
        tmp.x = -x;
        tmp.y = -y;
        tmp.z = -z;
        tmp.w = w;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  Invert the values of this quaternion
    ************************************************************************/
    CQuaternion getInverse() const
    {
        CQuaternion tmp;

        double norm2_inv = 1.0 / getLengthSquared();

        tmp.x = -x * norm2_inv;
        tmp.y = -y * norm2_inv;
        tmp.z = -z * norm2_inv;
        tmp.w =  w * norm2_inv;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  Get the length of the quaternion 
    ************************************************************************/
    double getLengthSquared() const
    {
        return (x * x) +  (y * y) + (z * z) + (w * w);
    }

    /************************************************************************                                                             
    *    desc:  Get the point 
    *
    *    return: CPoint - get the point
    ************************************************************************/
    CPoint<float> getPoint() const
    {
        CPoint<float> tmp;

        tmp.x = x * length;
        tmp.y = y * length;
        tmp.z = z * length;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  normalize this quaternion
    ************************************************************************/
    void normalize()
    {
        length = sqrt( getLengthSquared() );

        if( length != 0.0f )
        {
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }
    }
};
