
/************************************************************************
*    FILE NAME:       worldvalue.h
*
*    DESCRIPTION:     World value class
************************************************************************/

#pragma once

class CWorldValue
{
public:

    // Constructors
    CWorldValue();
    CWorldValue( const CWorldValue & value );
    CWorldValue( int _i );
    CWorldValue( float _f );
    CWorldValue( int _i, float _f );
    CWorldValue( float _f, int _i );

    // Conform the world value to the sector size
    void conformValue();
    CWorldValue getConformedValue() const;

    // Convert the value in single float form
    void toFloat();
    float getFloat() const;
    operator float() const { return getFloat(); }

    // Comparison Operators
    bool operator == ( const CWorldValue & value ) const;
    bool operator == ( const float value ) const;

    bool operator != ( const CWorldValue & value ) const;
    bool operator != ( const float value ) const;

    bool operator < ( const CWorldValue & value ) const;
    bool operator < ( const float value ) const;

    bool operator > ( const CWorldValue & value ) const;
    bool operator > ( const float value ) const;

    bool operator <= ( const CWorldValue & value ) const;
    bool operator <= ( const float value ) const;

    bool operator >= ( const CWorldValue & value ) const;
    bool operator >= ( const float value ) const;

    // Assignment Operator
    CWorldValue & operator = ( const CWorldValue & value );

    // Addition Operators
    CWorldValue operator + ( const CWorldValue & value ) const;
    CWorldValue operator + ( const float value ) const;
    void operator += ( const CWorldValue & value );
    void operator += ( const float value );

    // Subtraction Operators
    CWorldValue operator - ( const CWorldValue & value ) const;
    CWorldValue operator - ( const float value ) const;
    void operator -= ( const CWorldValue & value );
    void operator -= ( const float value );

    // Multiplication Operators
    CWorldValue operator * ( const CWorldValue & value ) const;
    CWorldValue operator * ( const int value ) const;
    CWorldValue operator * ( const float value ) const;
    void operator *= ( const CWorldValue & value );
    void operator *= ( const int value );
    void operator *= ( const float value );

    // Division Operators
    CWorldValue operator / ( const CWorldValue & value ) const;
    CWorldValue operator / ( const float value ) const;
    void operator /= ( const CWorldValue & value );
    void operator /= ( const float value );

    // Negative Operator
    CWorldValue operator - () const;
    
    // Set the sector size
    static void setSectorSize( int size );

public:

    // The float and int values of the world point
    float f;
    int i;

private:
    
    // sector size
    static int SECTOR_SIZE;
    static int HALF_SECTOR_SIZE;
};
