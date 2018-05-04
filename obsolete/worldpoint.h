/************************************************************************
*    FILE NAME:       worldpoint.h
*
*    DESCRIPTION:     World point class
************************************************************************/

#ifndef __world_point_h__
#define __world_point_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/point.h>

class CWorldPoint
{
public:

	CPoint<float> m_point_f;
	CPoint<int> m_point_i;

    // Constructor
    CWorldPoint();
	CWorldPoint( const CWorldPoint & point );
	CWorldPoint( const CPoint<int> & point );
	CWorldPoint( const CPoint<float> & point );
	CWorldPoint( const CPoint<int> & point_i, const CPoint<float> & point_f );
	CWorldPoint( const CPoint<float> & point_f, const CPoint<int> & point_i );


	// Get the int and float components separately
	CPoint<int> GetIntComponent() const;
	CPoint<float> GetFloatComponent() const;

	// Get the length and squared length of the line from the origin to the point
	float GetLength() const;
	float GetLengthSquared() const;
	float GetLength( const CWorldPoint & point ) const;
	float GetLengthSquared( const CWorldPoint & point ) const;

	// Is the world point empty
	bool IsEmpty() const;

	// Clear the point
	void Clear();

	// Print out the contents of the world point
	std::string Dump() const;

	// Comparison Operators
	bool CWorldPoint::operator == ( const CWorldPoint & point ) const;
	bool CWorldPoint::operator == ( const CPoint<int> & point ) const;
	bool CWorldPoint::operator == ( const CPoint<float> & point ) const;
	bool CWorldPoint::operator != ( const CWorldPoint & point ) const;
	bool CWorldPoint::operator != ( const CPoint<int> & point ) const;
	bool CWorldPoint::operator != ( const CPoint<float> & point ) const;

	// Assignment Operators
	CWorldPoint & CWorldPoint::operator = ( const CPoint<int> & point );
	CWorldPoint & CWorldPoint::operator = ( const CPoint<float> & point );
	CWorldPoint & CWorldPoint::operator = ( const float value );

	// Addition Operators
	CWorldPoint CWorldPoint::operator + ( const CWorldPoint & point ) const;
	CWorldPoint CWorldPoint::operator + ( const CPoint<int> & point ) const;
	CWorldPoint CWorldPoint::operator + ( const CPoint<float> & point ) const;
	CWorldPoint CWorldPoint::operator + ( const float value ) const;
	void CWorldPoint::operator += ( const CWorldPoint & point );
	void CWorldPoint::operator += ( const CPoint<int> & point );
	void CWorldPoint::operator += ( const CPoint<float> & point );
	void CWorldPoint::operator += ( const float value );

	// Subtraction Operators
	CWorldPoint CWorldPoint::operator - ( const CWorldPoint & point ) const;
	CWorldPoint CWorldPoint::operator - ( const CPoint<int> & point ) const;
	CWorldPoint CWorldPoint::operator - ( const CPoint<float> & point ) const;
	CWorldPoint CWorldPoint::operator - ( const float value ) const;
	void CWorldPoint::operator -= ( const CWorldPoint & point );
	void CWorldPoint::operator -= ( const CPoint<int> & point );
	void CWorldPoint::operator -= ( const CPoint<float> & point );
	void CWorldPoint::operator -= ( const float value );

	// Multiplication Operators
	CWorldPoint CWorldPoint::operator * ( const CWorldPoint & point ) const;
	CWorldPoint CWorldPoint::operator * ( const CPoint<int> & point ) const;
	CWorldPoint CWorldPoint::operator * ( const CPoint<float> & point ) const;
	CWorldPoint CWorldPoint::operator * ( const float value ) const;
	void CWorldPoint::operator *= ( const CWorldPoint & point );
	void CWorldPoint::operator *= ( const CPoint<int> & point );
	void CWorldPoint::operator *= ( const CPoint<float> & point );
	void CWorldPoint::operator *= ( const float value );

	// Division Operators
	CWorldPoint CWorldPoint::operator / ( const CWorldPoint & point ) const;
	CWorldPoint CWorldPoint::operator / ( const CPoint<int> & point ) const;
	CWorldPoint CWorldPoint::operator / ( const CPoint<float> & point ) const;
	CWorldPoint CWorldPoint::operator / ( const float value ) const;
	void CWorldPoint::operator /= ( const CWorldPoint & point );
	void CWorldPoint::operator /= ( const CPoint<int> & point );
	void CWorldPoint::operator /= ( const CPoint<float> & point );
	void CWorldPoint::operator /= ( const float value );

	// Negative Operator
	CWorldPoint CWorldPoint::operator - () const;

};

#endif	// __new_world_point_h__