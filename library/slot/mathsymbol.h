
/************************************************************************
*    FILE NAME:       mathsymbol.h
*
*    DESCRIPTION:     Class for holding a math symbol
************************************************************************/

#ifndef __math_symbol_h__
#define __math_symbol_h__

// Standard lib dependencies
#include <string>
#include <vector>

class CMathSymbol
{
public:

    // Constructor
    CMathSymbol( const std::string & symbolID, const std::vector<std::string> & wildMatches );
    
    // Get the string ID
    const std::string & getID() const;
    
    // Is this a wild symbol?
    bool isWild() const;
    
    // Is wild for this symbol?
    bool isWildFor( const std::string & symbolID ) const;
    
    // Does symbol match?
    bool isMatch( const std::string & symbolID ) const;
    
    // Equality operator. The symbols are equal if their id's are
    bool operator == ( const CMathSymbol & mathSymbol ) const;
    bool operator == ( const std::string & symbolID ) const;
    
private:
    
    // Symbol ID
    const std::string m_id;
    
    // ID's of other math symbols that this symbol is wild for
    const std::vector<std::string> m_wildMatches;

};

#endif  // __math_symbol_list_h__


