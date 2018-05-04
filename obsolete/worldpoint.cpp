
/************************************************************************
*    FILE NAME:       newworldpoint.cpp
*
*    DESCRIPTION:     World point class
************************************************************************/

// Physical component dependency
#include <common/worldpoint.h>

// Boost lib dependencies
#include <boost/format.hpp>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CWorldPoint::CWorldPoint()
{
}	// Constructer

CWorldPoint::CWorldPoint( const CWorldPoint & point )
{
	*this = point;

}	// Constructer

CWorldPoint::CWorldPoint( const CPoint<int> & point )
{
	m_point_i = point;

}	// Constructer

CWorldPoint::CWorldPoint( const CPoint<float> & point )
{
	m_point_f = point;

}	// Constructer

CWorldPoint::CWorldPoint( const CPoint<int> & point_i, const CPoint<float> & point_f )
{
	m_point_f = point_f;
	m_point_i = point_i;

}	// Constructer

CWorldPoint::CWorldPoint( const CPoint<float> & point_f, const CPoint<int> & point_i )
{
	m_point_f = point_f;
	m_point_i = point_i;

}	// Constructer


/************************************************************************
*    desc:  Get the integer components of the world point
*
*	 ret:	CPointInt - point int
************************************************************************/
CPoint<int> CWorldPoint::GetIntComponent() const
{
	return m_point_i;

}	// IPoint

/************************************************************************
*    desc:  Get the float components of the world point
*
*	 ret:	CPoint - point float
************************************************************************/
CPoint<float> CWorldPoint::GetFloatComponent() const
{
	return m_point_f;

}	// FPoint


/************************************************************************
*    desc:  Get the length of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLength() const
{
	return m_point_f.GetLength();

}	// GetLength


/************************************************************************
*    desc:  Get the length squared of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLengthSquared() const
{
	return m_point_f.GetLengthSquared();

}	// GetLengthSquared


/************************************************************************
*    desc:  Get the length of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLength( const CWorldPoint & point ) const
{
	return m_point_f.GetLength( point.m_point_f );

}	// GetLength


/************************************************************************
*    desc:  Get the length squared of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLengthSquared( const CWorldPoint & point ) const
{
	return m_point_f.GetLengthSquared( point.m_point_f );

}	// GetLengthSquared


/************************************************************************                                                             
*    desc:  Does this point not have any data? 
*
*    ret:	bool
************************************************************************/
bool CWorldPoint::IsEmpty() const
{
    return m_point_f.IsEmpty() && m_point_i.IsEmpty();

}   // IsEmpty


/************************************************************************                                                             
*    desc:  Clear the point
************************************************************************/
void CWorldPoint::Clear()
{
	m_point_f.Clear();
	m_point_i.Clear();

}	// Clear


/************************************************************************                                                             
*    desc:  Get a string of the world point's data
*
*	 ret:	string - string of the data
************************************************************************/
std::string CWorldPoint::Dump() const
{
	std::string printout = "W[ I( " +
							str( boost::format("%d") % m_point_i.x ) + "x, " +
							str( boost::format("%d") % m_point_i.y ) + "y, " +
							str( boost::format("%d") % m_point_i.z ) + "z" +
							" ) F( " +
							str( boost::format("%d") % m_point_f.x ) + "x, " +
							str( boost::format("%d") % m_point_f.y ) + "y, " +
							str( boost::format("%d") % m_point_f.z ) + "z ) ]";

	return printout;

}	// Dump


/***********************************************************************************
*	COMPARISON OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The equality operator 
*
*    param:  const & point - point to check
*
*    return: bool - true or false
************************************************************************/
bool CWorldPoint::operator == ( const CWorldPoint & point ) const
{
	if( m_point_i == point.m_point_i && m_point_f == point.m_point_f )
        return true;
    
    return false;

}   // operator ==

bool CWorldPoint::operator == ( const CPoint<int> & point ) const
{
	if( m_point_i == point )
        return true;
    
    return false;

}   // operator ==

bool CWorldPoint::operator == ( const CPoint<float> & point ) const
{
	if( m_point_f == point )
        return true;
    
    return false;

}   // operator ==

/************************************************************************                                                             
*    desc:  The inequality operator 
*
*    param:  const & point - point to check
*
*    return: bool - true or false
************************************************************************/
bool CWorldPoint::operator != ( const CWorldPoint & point ) const
{
    if( m_point_i != point.m_point_i && m_point_f != point.m_point_f )
        return true;

    return false;

}   // operator !=

bool CWorldPoint::operator != ( const CPoint<int> & point ) const
{
    if( m_point_i != point )
        return true;

    return false;

}   // operator !=

bool CWorldPoint::operator != ( const CPoint<float> & point ) const
{
    if( m_point_f != point )
        return true;

    return false;

}   // operator !=


/***********************************************************************************
*	ASSIGNMENT OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Assignment operator
*
*	 param: const point - point assign
************************************************************************/
CWorldPoint & CWorldPoint::operator = ( const CPoint<int> & point )
{
	m_point_i = point;

	return *this;

}	// operator =

CWorldPoint & CWorldPoint::operator = ( const CPoint<float> & point )
{
	m_point_f = point;

	return *this;

}	// operator =

CWorldPoint & CWorldPoint::operator = ( const float value )
{
	m_point_f.x = value;
	m_point_f.y = value;
	m_point_f.z = value;

	return *this;

}	// operator =



/***********************************************************************************
*	ADDITION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Addition operator
*
*	 param: const point - point to add
*
*	 ret:	CWorldPoint - summation of points
************************************************************************/
CWorldPoint CWorldPoint::operator + ( const CWorldPoint & point ) const
{
	return CWorldPoint( m_point_i + point.m_point_i, m_point_f + point.m_point_f );

}	// operator +

CWorldPoint CWorldPoint::operator + ( const CPoint<int> & point ) const
{
	return CWorldPoint( m_point_i + point, m_point_f );

}	// operator +

CWorldPoint CWorldPoint::operator + ( const CPoint<float> & point ) const
{
	return CWorldPoint( m_point_i, m_point_f + point );

}	// operator +

CWorldPoint CWorldPoint::operator + ( const float value ) const
{
	return CWorldPoint( m_point_i, m_point_f + value );

}	// operator +

/************************************************************************
*    desc:  Addition operator
*
*	 param: const point - point to add
************************************************************************/
void CWorldPoint::operator += ( const CWorldPoint & point )
{
	*this = *this + point;

}	// operator +=

void CWorldPoint::operator += ( const CPoint<int> & point )
{
	*this = *this + point;

}	// operator +=

void CWorldPoint::operator += ( const CPoint<float> & point )
{
	*this = *this + point;

}	// operator += 

void CWorldPoint::operator += ( const float value )
{
	*this = *this + value;

}	// operator +=



/***********************************************************************************
*	SUBTRACTION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Subtraction operator
*
*	 param: const point - point to subtract
*
*	 ret:	CWorldPoint - subtraction of points
************************************************************************/
CWorldPoint CWorldPoint::operator - ( const CWorldPoint & point ) const
{
	return CWorldPoint( m_point_i - point.m_point_i, m_point_f - point.m_point_f );

}	// operator -

CWorldPoint CWorldPoint::operator - ( const CPoint<int> & point ) const
{
	return CWorldPoint( m_point_i - point, m_point_f );

}	// operator -

CWorldPoint CWorldPoint::operator - ( const CPoint<float> & point ) const
{
	return CWorldPoint( m_point_i, m_point_f - point );

}	// operator -

CWorldPoint CWorldPoint::operator - ( const float value ) const
{
	return CWorldPoint( m_point_i, m_point_f - value );

}	// operator -

/************************************************************************
*    desc:  Subtraction operator
*
*	 param: const point - point to subtract
************************************************************************/
void CWorldPoint::operator -= ( const CWorldPoint & point )
{
	*this = *this - point;

}	// operator -=

void CWorldPoint::operator -= ( const CPoint<int> & point )
{
	*this = *this - point;

}	// operator -=

void CWorldPoint::operator -= ( const CPoint<float> & point )
{
	*this = *this - point;

}	// operator -= 

void CWorldPoint::operator -= ( const float value )
{
	*this = *this - value;

}	// operator -=


/***********************************************************************************
*	MULTIPLICATION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Multiplication operator
*
*	 param: const point - point to multiply
*
*	 ret:	CWorldPoint - product of points
************************************************************************/
CWorldPoint CWorldPoint::operator * ( const CWorldPoint & point ) const
{
	return CWorldPoint( m_point_i * point.m_point_i, m_point_f * point.m_point_f );

}	// operator *

CWorldPoint CWorldPoint::operator * ( const CPoint<int> & point ) const
{
	return *this * CWorldPoint(  m_point_i * point, m_point_f );

}	// operator *

CWorldPoint CWorldPoint::operator * ( const CPoint<float> & point ) const
{
	return CWorldPoint( m_point_i, m_point_f * point );

}	// operator * 

CWorldPoint CWorldPoint::operator * ( const float value ) const
{
	return CWorldPoint( m_point_i, m_point_f * value );

}	// operator * 

/************************************************************************
*    desc:  Multiplication operator
*
*	 param: const point - point to multiply
************************************************************************/
void CWorldPoint::operator *= ( const CWorldPoint & point )
{
	*this = *this * point;

}	// operator *=

void CWorldPoint::operator *= ( const CPoint<int> & point )
{
	*this = *this * point;

}	// operator *=

void CWorldPoint::operator *= ( const CPoint<float> & point )
{
	*this = *this * point;

}	// operator *= 

void CWorldPoint::operator *= ( const float value )
{
	*this = *this * value;

}	// operator *= 



/***********************************************************************************
*	DIVISION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Division operator
*
*	 param: const point - point to divided
*
*	 ret:	CWorldPoint - division of points
************************************************************************/
CWorldPoint CWorldPoint::operator / ( const CWorldPoint & point ) const
{
	return CWorldPoint( m_point_i / point.m_point_i, m_point_f / point.m_point_f );

}	// operator /

CWorldPoint CWorldPoint::operator / ( const CPoint<int> & point ) const
{
	return *this / CWorldPoint( m_point_i / point, m_point_f );

}	// operator /

CWorldPoint CWorldPoint::operator / ( const CPoint<float> & point ) const
{
	return CWorldPoint( m_point_i, m_point_f / point );

}	// operator / 

CWorldPoint CWorldPoint::operator / ( const float value ) const
{
	return CWorldPoint( m_point_i, m_point_f / value );

}	// operator /

/************************************************************************
*    desc:  Division operator
*
*	 param: const point - point to divided
************************************************************************/
void CWorldPoint::operator /= ( const CWorldPoint & point )
{
	*this = *this / point;

}	// operator /=

void CWorldPoint::operator /= ( const CPoint<int> & point )
{
	*this = *this / point;

}	// operator /=

void CWorldPoint::operator /= ( const CPoint<float> & point )
{
	*this = *this / point;

}	// operator /= 

void CWorldPoint::operator /= ( const float value )
{
	*this = *this / value;

}	// operator /=



/***********************************************************************************
*	NEGATIVE OPERATOR
***********************************************************************************/
/************************************************************************
*    desc:  Return a copy of the point with its signs flipped
*
*	 ret:	CWorldPoint - point with signs flipped
************************************************************************/
CWorldPoint CWorldPoint::operator - () const
{
	return CWorldPoint( -m_point_i, -m_point_f );

}	// operator -
