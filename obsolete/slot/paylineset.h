
/************************************************************************
*    FILE NAME:       paylineset.h
*
*    DESCRIPTION:     Class for holding the payline and scatter eval symbol 
*                     positions that are allowed for evaluation
************************************************************************/

#ifndef __payline_set_h__
#define __payline_set_h__

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <cstdint>

// Forward declaration(s)
struct XMLNode;

class CPaylineSet
{
public:

    // Constructor
    CPaylineSet( const XMLNode & node );
    
    // Get the line data
    const std::vector<std::vector<int8_t>> & getLineData() const;
    
    // See if the scatter eval symbol index is allowed
    bool indexOfScaterData( int strip, int index ) const;

private:
    
    // Vector holding the payline offsets
    std::vector<std::vector<int8_t>> m_lineVecVec;

    // Vector holding the scatter offsets
    std::vector<std::vector<int8_t>> m_scatterVecVec;
};

#endif
