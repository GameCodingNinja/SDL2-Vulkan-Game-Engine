
/************************************************************************
*    FILE NAME:       valuetable.h
*
*    DESCRIPTION:     Class for holding the value table
************************************************************************/

#ifndef __value_table_h__
#define __value_table_h__

// Standard lib dependencies
#include <vector>

class CValueTable
{
public:

    // Constructor
    CValueTable( const std::vector<int> & valueVec );

    // Destructor
    virtual ~CValueTable();
    
    // Get the value from the value table
    int getValue( const int index ) const;
    
private:
    
    // value
    const std::vector<int> m_valueVec;
};

#endif  // __value_table_h__
