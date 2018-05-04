
/************************************************************************
*    FILE NAME:       mathsymbol.cpp
*
*    DESCRIPTION:     Class for holding a math symbol
************************************************************************/

// Physical component dependency
#include <slot/mathsymbol.h>

// Standard lib dependencies
#include <algorithm>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMathSymbol::CMathSymbol(
    const std::string & symbolID, const std::vector<std::string> & wildMatches ) :
        m_id( symbolID ),
        m_wildMatches( wildMatches )
{
}


/************************************************************************
*    DESC:  Get the string ID
************************************************************************/
const std::string & CMathSymbol::getID() const
{
    return m_id;
}


/************************************************************************
*    DESC:  Is this a wild symbol?
************************************************************************/
bool CMathSymbol::isWild() const
{
    return !m_wildMatches.empty();
}


/************************************************************************
*    DESC:  Is wild for this symbol?
************************************************************************/
bool CMathSymbol::isWildFor( const std::string & symbolID ) const
{
    return std::find( m_wildMatches.begin(), m_wildMatches.end(), symbolID ) != m_wildMatches.end();
}


/************************************************************************
*    DESC:  Does symbol match?
************************************************************************/
bool CMathSymbol::isMatch( const std::string & symbolID ) const
{
    return ((m_id == symbolID) || isWildFor(symbolID));
}


/************************************************************************
*    DESC:  Equality operator. The symbols are equal if their id's are
************************************************************************/
bool CMathSymbol::operator == ( const CMathSymbol & mathSymbol ) const
{
    if( m_id == mathSymbol.getID() )
        return true;

    return false;
}

bool CMathSymbol::operator == ( const std::string & symbolID ) const
{
    if( m_id == symbolID )
        return true;

    return false;
}
