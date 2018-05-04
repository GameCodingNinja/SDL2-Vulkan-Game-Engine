
/************************************************************************
*    FILE NAME:       weightedtable.cpp
*
*    DESCRIPTION:     Class for holding the weighted table
************************************************************************/

// Physical component dependency
#include <slot/weightedtable.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CWeightedTable::CWeightedTable(
    const int totalWeight,
    const std::vector<int> & weightVec,
    const std::vector<int> & valueVec ) :
        CValueTable( valueVec ),
            m_totalWeight(totalWeight),
            m_weightVec(weightVec)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CWeightedTable::~CWeightedTable()
{
}


/************************************************************************
 *    DESC:  Get the value from the weighted table
 ************************************************************************/
int CWeightedTable::getWeightedValue( const int rngValue ) const
{
    int index(0);
    int weightCount(0);

    for( int iter : m_weightVec )
    {
        weightCount += iter;

        if( rngValue <= weightCount )
            break;

        ++index;
    }

    return getValue(index);
}


/************************************************************************
 *    DESC:  Get the total weight value
 ************************************************************************/
int CWeightedTable::getTotalWeight() const
{
    return m_totalWeight;
}
