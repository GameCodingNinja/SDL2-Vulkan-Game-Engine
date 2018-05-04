
/************************************************************************
*    FILE NAME:       stripstop.h
*
*    DESCRIPTION:     Math strip stop class
************************************************************************/

#ifndef __strip_stop_h__
#define __strip_stop_h__

// Forward declaration(s)
class CMathSymbol;

class CStripStop
{
public:

    // Constructor
    CStripStop( const CMathSymbol & rMathSymbol, int weight );
    
    // Get the math symbol weight
    int getWeight() const;
    
    // Get the math symbol
    const CMathSymbol & getMathSymbol() const;
    
private:
    
    // Math symbol
    const CMathSymbol & m_rMathSymbol;
    
    // Math symbol weight
    const int m_weight;

};

#endif  // __strip_stop_h__


