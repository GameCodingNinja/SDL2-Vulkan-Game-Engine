
/************************************************************************
*    FILE NAME:       weightedtable.h
*
*    DESCRIPTION:     Class for holding the weighted table
************************************************************************/

#ifndef __weighted_table_h__
#define __weighted_table_h__

// Physical component dependency
#include <slot/valuetable.h>

class CWeightedTable : public CValueTable
{
public:

    // Constructor
    CWeightedTable(
        const int totalWeight,
        const std::vector<int> & weightVec,
        const std::vector<int> & valueVec );

    // Destructor
    virtual ~CWeightedTable();
    
    // Get the value from the weighted table
    int getWeightedValue( const int rngValue ) const;
    
    // Get the total weight value
    int getTotalWeight() const;
    
private:
    
    // total weight
    const int m_totalWeight;
    
    // weight
    const std::vector<int> m_weightVec;
};

#endif  // __weighted_table_h__


